
#include <csv/datasource/utf8/DataSource.hpp>
#include <csv/parser.hpp>
#include <expected.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Models.hpp"
#include "ParserStructure.hpp"
#include "TSVParser.hpp"

using namespace std;
using namespace v8;

TSVParser::TSVParser(string path, string type,
                     shared_ptr<ParserStructure> structure)
    : m_path(path), m_type(type), m_structure(structure){};

/* TSVParser::TSVParser(const TSVParser &old)
    : m_path(old.m_path), m_type(old.m_type), m_structure{} {
  m_structure = make_unique(*old.m_structure);
};
 */
MaybeParser makeParser(string path, string type) {

  auto parserMap = TSVParser::getParserMap();

  for (auto const &[key, value] : parserMap) {
    if (type == key) {
      return TSVParser{path, type, move(value)};
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

ParseResult TSVParser::parseData() {

  csv::utf8::FileDataSource input;

  if (!input.open(m_path.c_str())) {
    return tl::make_unexpected("Filepath was invalid: '" + m_path + "'");
  }

  vector<int> vec = vector<int>{};
  /*   csv::parse(
        input,
        [](const csv::field &field) -> bool {
          // Do something with 'field'
          return true;
        },
        [](const csv::record &record, double progress) -> bool {
          // Do something with 'record'
          return true;
        }); */

  vec.push_back(2);

  v8::Local<v8::Array> results = Nan::New<v8::Array>(vec.size());
  int i = 0;
  for_each(vec.begin(), vec.end(), [&](int value) {
    Nan::Set(results, i, Nan::New<v8::Number>(value));
    i++;
  });

  return results;
}
