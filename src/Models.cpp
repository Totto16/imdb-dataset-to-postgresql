
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
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::intParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::regionParser),
          StaticParsers::orNullParser(StaticParsers::languageParser),
          StaticParsers::arrayParser(StaticParsers::alternativeTitleParser),
          StaticParsers::arrayParser(StaticParsers::asIs),
          StaticParsers::orNullParser(StaticParsers::booleanParser),
      });
}

shared_ptr<ParserStructure> Model::mappedNameBasic() {
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
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::intParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::regionParser),
          StaticParsers::orNullParser(StaticParsers::languageParser),
          StaticParsers::arrayParser(StaticParsers::alternativeTitleParser),
          StaticParsers::arrayParser(StaticParsers::asIs),
          StaticParsers::orNullParser(StaticParsers::booleanParser),
      });
}

shared_ptr<ParserStructure> Model::mappedTitleBasic() {
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
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::intParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::regionParser),
          StaticParsers::orNullParser(StaticParsers::languageParser),
          StaticParsers::arrayParser(StaticParsers::alternativeTitleParser),
          StaticParsers::arrayParser(StaticParsers::asIs),
          StaticParsers::orNullParser(StaticParsers::booleanParser),
      });
}

shared_ptr<ParserStructure> Model::mappedTitleCrew() {
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
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::intParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::regionParser),
          StaticParsers::orNullParser(StaticParsers::languageParser),
          StaticParsers::arrayParser(StaticParsers::alternativeTitleParser),
          StaticParsers::arrayParser(StaticParsers::asIs),
          StaticParsers::orNullParser(StaticParsers::booleanParser),
      });
}

shared_ptr<ParserStructure> Model::mappedTitleEpisode() {
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
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::intParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::regionParser),
          StaticParsers::orNullParser(StaticParsers::languageParser),
          StaticParsers::arrayParser(StaticParsers::alternativeTitleParser),
          StaticParsers::arrayParser(StaticParsers::asIs),
          StaticParsers::orNullParser(StaticParsers::booleanParser),
      });
}

shared_ptr<ParserStructure> Model::mappedTitlePrincipal() {
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
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::intParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::regionParser),
          StaticParsers::orNullParser(StaticParsers::languageParser),
          StaticParsers::arrayParser(StaticParsers::alternativeTitleParser),
          StaticParsers::arrayParser(StaticParsers::asIs),
          StaticParsers::orNullParser(StaticParsers::booleanParser),
      });
}

shared_ptr<ParserStructure> Model::mappedTitleRating() {
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
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::intParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::regionParser),
          StaticParsers::orNullParser(StaticParsers::languageParser),
          StaticParsers::arrayParser(StaticParsers::alternativeTitleParser),
          StaticParsers::arrayParser(StaticParsers::asIs),
          StaticParsers::orNullParser(StaticParsers::booleanParser),
      });
}
