
#include <csv/datasource/utf8/DataSource.hpp>
#include <csv/parser.hpp>
#include <expected.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <nan.h>
#include <string>
#include <thread>
#include <vector>

#include "Models.hpp"
#include "ParserStructure.hpp"
#include "TSVParser.hpp"
#include "helper.hpp"

using namespace std;
using namespace v8;

TSVParser::TSVParser(string path, string type,
                     shared_ptr<ParserStructure> structure)
    : m_path(path), m_type(type), m_structure(structure){};

MaybeParser makeParser(string path, string type) {

  auto parserMap = TSVParser::getParserMap();

  for (auto const &[key, structure] : parserMap) {
    if (type == key) {
      return TSVParser{path, type, structure};
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

ParseResult TSVParser::parseData(const ExecutionProgressSender *sender,
                                 EventEmitterFunctionReentrant emitter) {
  csv::utf8::FileDataSource input;
	input.separator = '\t';

  if (!input.open(m_path.c_str())) {
    return tl::make_unexpected("Filepath was invalid: '" + m_path + "'");
  }

  csv::parse(
      input,
      [](const csv::field &field) -> bool {
        // Do something with 'field'
        return true;
      },
      [&](const csv::record &record, double progress) -> bool {
        auto parserResult = m_structure->parse(record);
        std::shared_ptr<Constructable> parsedLine =
            std::make_shared<ObjectConstructable>(parserResult);
        while (!emitter(sender, "parsedLine", parsedLine)) {
          std::this_thread::yield();
        }
        return true;
      });

  return true;
}
