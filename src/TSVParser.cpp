
#include <csv/datasource/utf8/DataSource.hpp>
#include <csv/parser.hpp>
#include <expected.hpp>
#include <map>
#include <memory>
#include <nan.h>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

#include "Models.hpp"
#include "ParserStructure.hpp"
#include "TSVParser.hpp"
#include "helper.hpp"

using namespace std;
using namespace v8;

TSVParser::TSVParser(string path, string type, OmitHeadType hasHead,
                     shared_ptr<ParserStructure> structure)
    : m_path(path), m_type(type), m_hasHead{hasHead}, m_structure(structure){};

MaybeParser makeParser(string path, string type, OmitHeadType hasHead) {

  auto parserMap = TSVParser::getParserMap();

  for (auto const &[key, structure] : parserMap) {
    if (type == key) {
      return TSVParser{path, type, hasHead, structure};
    }
  }

  return tl::make_unexpected("Not a valid type: '" + type + "'");
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
      m_structure->m_order.begin(), m_structure->m_order.end(), std::string(),
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

ParseResult TSVParser::parseData(const ExecutionProgressSender *sender,
                                 EventEmitterFunctionReentrant emitter) {
  csv::utf8::FileDataSource input;
  input.separator = '\t';

  if (!input.open(m_path.c_str())) {
    return tl::make_unexpected("Filepath was invalid: '" + m_path + "'");
  }

  int32_t parsedLines = 0;
  bool skippedHeader = false;

  csv::parse(input, nullptr,
             [&](const csv::record &record, double progress) -> bool {
               if (parsedLines == 0 && !skippedHeader) {
                 if (m_hasHead == OmitHeadType::True) {
                   skippedHeader = true;
                   return true;
                 } else if (m_hasHead == OmitHeadType::Auto) {
                   if (isHeader(record)) {
                     skippedHeader = true;
                     return true;
                   }
                 }
               }

               auto parserResult = m_structure->parse(record);
               std::shared_ptr<Constructable> parsedLine =
                   std::make_shared<ObjectConstructable>(parserResult);
               while (!emitter(sender, "parsedLine", parsedLine)) {
                 std::this_thread::yield();
               }

               ++parsedLines;
               // TODO: here error handling!
               return true;
             });

  return parsedLines;
}
