

#include <csv/datasource/utf8/DataSource.hpp>
#include <csv/parser.hpp>
#include <exception>
#include <expected>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include "./LogWriter.hpp"
#include "Models.hpp"
#include "ParseMetadata.hpp"
#include "ParserStructure.hpp"
#include "TSVParser.hpp"
#include "postgres/Error.h"
#include "source/DataSource.hpp"

TSVParser::TSVParser(std::filesystem::path file, std::string type,
                     OmitHeadType hasHead, std::shared_ptr<Parseable> structure,
                     off_t offset, std::size_t length)
    : m_file{file}, m_type{type}, m_hasHead{hasHead}, m_structure{structure},
      m_offset{offset}, m_length{length} {};

MaybeParser makeParser(std::filesystem::path file,
                       std::optional<std::string> optionalType,
                       OmitHeadType hasHead, off_t offset, std::size_t length,
                       std::optional<std::uint32_t> transactionSize) {

  if (!std::filesystem::exists(file)) {
    return std::unexpected<std::string>{"File doesn't exist: '" +
                                        file.string() + "'"};
  }

  auto parserMap = TSVParser::getParserMap();

  if (optionalType.has_value()) {
    auto type = optionalType.value();

    for (auto const &[key, structure] : parserMap) {
      if (type == key) {
        structure->setTransactionSize(transactionSize);

        return TSVParser{file, type, hasHead, structure, offset, length};
      }
    }

    return std::unexpected<std::string>{"Not a valid type: '" + type + "'"};
  }

  auto filename = file.filename().string();

  for (auto const &[key, structure] : parserMap) {
    if (filename.contains(key)) {
      structure->setTransactionSize(transactionSize);
      return TSVParser{file, key, hasHead, structure, offset, length};
    }
  }

  return std::unexpected<std::string>{"Couldn't detect type based on file"};
}

ParserMap TSVParser::getParserMap() {
  auto parserMap = ParserMap{};

  parserMap.insert({"title.akas", Model::mappedTitleAlternate()});
  parserMap.insert({"name.basics", Model::mappedNameBasic()});
  parserMap.insert({"title.basics", Model::mappedTitleBasic()});
  parserMap.insert({"title.crew", Model::mappedTitleCrew()});
  parserMap.insert({"title.episode", Model::mappedTitleEpisode()});
  parserMap.insert({"title.principals", Model::mappedTitlePrincipal()});
  parserMap.insert({"title.ratings", Model::mappedTitleRating()});

  return parserMap;
}

[[nodiscard]] bool TSVParser::isHeader(const csv::record &record) {

  auto headLineString = std::accumulate(
      m_structure->names().begin(), m_structure->names().end(), std::string(),
      [](const std::string &a, const std::string &b) -> std::string {
        return a + "\t" + b;
      });

  auto line = std::accumulate(
      record.content.begin(), record.content.end(), std::string(),
      [](const std::string &a, const csv::field &b) -> std::string {
        return a + "\t" + b.content;
      });

  return headLineString == line;
}

namespace {
// partially from here:
// https://www.reddit.com/r/cpp_questions/comments/11wlf49/whats_the_most_efficient_way_to_get_the_line/
std::uint64_t countLines(std::ifstream &in_stream) {
  auto iterator = std::istreambuf_iterator<char>{in_stream};
  return std::count_if(iterator, std::istreambuf_iterator<char>(),
                       [](char c) { return c == '\n'; });
}

std::uint64_t countLines(const std::filesystem::path &file) {
  std::ifstream fileStream{file, std::ios::binary};
  return countLines(fileStream);
}

} // namespace

ParseResult TSVParser::parseData(postgres::Connection &connection,
                                 ParseOptions options) {
  std::unique_ptr<csv::utf8::DataSource> input{};

  ParseResult result{};

  const auto isMultithreaded = m_length != 0;

  try {
    if (isMultithreaded) {
      input = std::make_unique<source::MemoryMappedDataSource>(m_file, m_offset,
                                                               m_length);
    } else {
      input = std::make_unique<source::FileDataSource>(m_file.string().c_str());
    }
  } catch (const csv::file_exception &error) {
    result.set_error("Filepath was invalid: '" + m_file.string() +
                     "': unable to open file");
    return result;
  } catch (const std::exception &error) {
    result.set_error("Failed to create data source: '" + m_file.string() +
                     "': " + error.what());
    return result;
  }

  input->separator = '\t';

  ParseMetadata metadata{};

  if (options.verbose && !isMultithreaded) {
    std::uint64_t availableLines = countLines(m_file);
    std::cout << "There are " << availableLines << " lines to scan\n";
  }

  bool skippedHeader = false;
  double lastProgress = 0.0;

  std::function<void(double progress)> printProgress;
  if (!isMultithreaded) {
    const auto printProgressImpl = [&](double progress) -> void {
      std::cout << "Progress: " << (progress * 100.0) << " %\n";
    };

    printProgress = [&lastProgress, &printProgressImpl,
                     &metadata](double progress) -> void {
      if (progress - lastProgress >= 0.01) {
        printProgressImpl(progress);
        lastProgress = progress;
      } else if (metadata.lines() % 10000 == 0) {
        printProgressImpl(progress);
      }
    };
  } else {
    printProgress = [](double) -> void {
      // ignore, noop
    };
  }

  // store here, sot hat RAII cleans this up!
  std::unique_ptr<LogWriter> log_writer = nullptr;

  std::function<void(const csv::record &record)> insertRecord;

  if (options.ignoreErrors.has_value()) {
    const std::optional<std::string> ignoreErrors =
        options.ignoreErrors.value();

    if (ignoreErrors.has_value()) {
      const auto file = ignoreErrors.value();

      if (isMultithreaded) {
        log_writer = std::make_unique<LogWriterFileMultithreaded>(file);
      } else {
        log_writer = std::make_unique<LogWriterFile>(file);
      }
    } else {
      log_writer = std::make_unique<LogWriterDummy>();
    }

    std::function<void(const std::string &)> addAndPrintError;

    if (options.verbose) {
      addAndPrintError = [&log_writer,
                          &metadata](const std::string &message) -> void {
        std::cerr << "An error occurred, but was ignored: " << message << "\n";

        log_writer->write_error(message);
        metadata.addError();
      };
    } else {
      addAndPrintError = [&log_writer,
                          &metadata](const std::string &message) -> void {
        log_writer->write_error(message);
        metadata.addError();
      };
    }

    insertRecord = [&connection, this, &metadata,
                    &addAndPrintError](const csv::record &record) -> void {
      try {
        this->m_structure->insert_record(connection, record);

        metadata.addLine();
      } catch (const std::exception &exc) {
        addAndPrintError(exc.what());
      }
    };

  } else {

    log_writer = std::make_unique<LogWriterDummy>();

    insertRecord = [this, &connection,
                    &metadata](const csv::record &record) -> void {
      this->m_structure->insert_record(connection, record);

      metadata.addLine();
    };
  }

  try {
    m_structure->setup_prepared_statement(connection);

    csv::parse(*input, nullptr,
               [&metadata, &skippedHeader, this, &printProgress, &insertRecord](
                   const csv::record &record, double progress) -> bool {
                 // skip header
                 // TODO: this is the hot path, make this faster!
                 if (metadata.lines() == 0 && !skippedHeader) {
                   if (m_hasHead == true) {
                     skippedHeader = true;
                     return true;
                   } else if (not m_hasHead.has_value()) {
                     if (isHeader(record)) {
                       skippedHeader = true;
                       return true;
                     }
                   }
                 }

                 insertRecord(record);

                 // print progress
                 printProgress(progress);

                 return true;
               });

  } catch (const postgres::Error &error) {
    m_structure->finish();
    result.set_error(error.what());
    return result;
  } catch (const std::exception &exc) {
    m_structure->finish();
    result.set_error(exc.what());
    return result;
  }

  m_structure->finish();

  result.set_value(metadata);
  return result;
}
