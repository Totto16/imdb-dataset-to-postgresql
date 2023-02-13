
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "InternalParsers.hpp"
#include "Models.hpp"
#include "ParserStructure.hpp"

using namespace std;

shared_ptr<ParserStructure> Model::mappedTitleAlternate() {
  return make_shared<ParserStructure>(
      vector<string>{
          "titleId",
          "ordering",
          "title",
          "region",
          "language",
          "types",
          "attributes",
          "isOriginalTitle",
      },
      vector<ParseFunction>{
          Parser::imdbIdParser,
          /*  Parser::intParser,
           Parser::asIs,
           Parser::orNullParser(Parser::regionParser),
           Parser::orNullParser(Parser::languageParser),
           Parser::arrayParser(Parser::alternativeTitleParser),
           Parser::arrayParser(Parser::asIs),
           Parser::orNullParser(Parser::booleanParser), */
      });
}
