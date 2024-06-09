

#include <csv/datasource/utf8/DataSource.hpp>
#include <csv/parser.hpp>
#include <exception>
#include <filesystem>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

#include "Models.hpp"
#include "ParserStructure.hpp"
#include "TSVParser.hpp"
#include "helper/expected.hpp"
#include "postgres/Error.h"

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

ParseResult TSVParser::parseData(postgres::Connection &connection) {
  csv::utf8::FileDataSource input;
  input.separator = '\t';

  if (!input.open(m_file.string().c_str())) {
    return helper::unexpected<std::string>{"Filepath was invalid: '" +
                                           m_file.string() + "'"};
  }

  int32_t parsedLines = 0;
  bool skippedHeader = false;

  try {
    m_structure->setup_prepared_statement(connection);

    csv::parse(input, nullptr,
               [&](const csv::record &record, double progress) -> bool {
                 // TODO: display progress
                 (void)progress;

                 if (parsedLines == 0 && !skippedHeader) {
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

                 m_structure->insert_record(connection, record);

                 ++parsedLines;

                 return true;
               });

  } catch (postgres::Error &error) {
    return helper::unexpected<std::string>{error.what()};
  } catch (std::exception &exc) {
    return helper::unexpected<std::string>{exc.what()};
  }

  return parsedLines;
}
