

#include <csv/datasource/utf8/DataSource.hpp>
#include <csv/parser.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "Models.hpp"
#include "ParseMetadata.hpp"
#include "ParserStructure.hpp"
#include "TSVParser.hpp"
#include "helper/expected.hpp"
#include "postgres/Error.h"
#include "source/DataSource.hpp"

TSVParser::TSVParser(std::filesystem::path file, std::string type,
                     OmitHeadType hasHead, std::shared_ptr<Parseable> structure)
    : m_file{file}, m_type{type}, m_hasHead{hasHead}, m_structure{structure} {};

MaybeParser makeParser(std::filesystem::path file,
                       std::optional<std::string> optionalType,
                       OmitHeadType hasHead) {

  if (!std::filesystem::exists(file)) {
    return helper::unexpected<std::string>{"File doesn't exist: '" +
                                           file.string() + "'"};
  }

  auto parserMap = TSVParser::getParserMap();

  if (optionalType.has_value()) {
    auto type = optionalType.value();

    for (auto const &[key, structure] : parserMap) {
      if (type == key) {
        return TSVParser{file, type, hasHead, structure};
      }
    }

    return helper::unexpected<std::string>{"Not a valid type: '" + type + "'"};
  }

  auto filename = file.filename().string();

  for (auto const &[key, structure] : parserMap) {
    if (filename.contains(key)) {
      return TSVParser{file, key, hasHead, structure};
    }
  }

  return helper::unexpected<std::string>{"Couldn't detect type based on file"};
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

size_t getFilesize(const char *filename) {
  struct stat st {};
  ::stat(filename, &st);
  return st.st_size;
}

} // namespace

ParseResult TSVParser::parseData(postgres::Connection &connection,
                                 ParseOptions options) {
  source::MemoryMappedDataSource input;
  input.separator = '\t';

  if (!input.open(m_file, 0, getFilesize(m_file.string().c_str()))) {
    return helper::unexpected<std::string>{"Filepath was invalid: '" +
                                           m_file.string() + "'"};
  }

  ParseMetadata result{};

  std::uint64_t availableLines = 0;

  if (options.verbose) {
    availableLines = countLines(m_file);
    std::cout << "There are " << availableLines << " lines to scan\n";
  }

  bool skippedHeader = false;
  double lastProgress = 0.0;

  const auto printProgress = [&](double progress) {
    std::cout << "Progress: " << (progress * 100.0) << " %\n";
  };

  try {
    m_structure->setup_prepared_statement(connection);

    csv::parse(input, nullptr,
               [&](const csv::record &record, double progress) -> bool {
                 if (result.lines() == 0 && !skippedHeader) {
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

                 if (options.ignoreErrors) {
                   try {
                     m_structure->insert_record(connection, record);

                     result.addLine();
                   } catch (std::exception &exc) {
                     if (options.verbose) {
                       std::cerr << "An error occurred, but was ignored: "
                                 << exc.what() << "\n";
                     }

                     result.addError();
                   }
                 } else {
                   m_structure->insert_record(connection, record);

                   result.addLine();
                 }

                 if (progress - lastProgress >= 0.01) {
                   printProgress(progress);
                   lastProgress = progress;
                 } else if (result.lines() % 10000 == 0) {
                   printProgress(progress);
                 }

                 return true;
               });

  } catch (postgres::Error &error) {
    return helper::unexpected<std::string>{error.what()};
  } catch (std::exception &exc) {
    return helper::unexpected<std::string>{exc.what()};
  }

  return result;
}
