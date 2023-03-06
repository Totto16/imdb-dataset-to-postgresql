
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
          "nconst",
          "primaryName",
          "birthYear",
          "deathYear",
          "primaryProfession",
          "knownForTitles",
      },
      vector<ParserFunction>{
          StaticParsers::nameIDParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::intParser),
          StaticParsers::orNullParser(StaticParsers::intParser),
          StaticParsers::arrayParser(StaticParsers::asIs),
          StaticParsers::arrayParser(StaticParsers::imdbIdParser),
      });
}

shared_ptr<ParserStructure> Model::mappedTitleBasic() {
  return make_shared<ParserStructure>(
      vector<string>{
          "tconst",
          "titleType",
          "primaryTitle",
          "originalTitle",
          "isAdult",
          "startYear",
          "endYear",
          "runtimeMinutes",
          "genres",
      },
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::titleTypeParser,
          StaticParsers::asIs,
          StaticParsers::asIs,
          StaticParsers::booleanParser,
          StaticParsers::orNullParser(StaticParsers::intParser),
          StaticParsers::orNullParser(StaticParsers::intParser),
          StaticParsers::orNullParser(StaticParsers::intParser),
          StaticParsers::arrayParser(StaticParsers::genreParser),
      });
}

shared_ptr<ParserStructure> Model::mappedTitleCrew() {
  return make_shared<ParserStructure>(
      vector<string>{"tconst", "directors", "writers"},
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::arrayParser(StaticParsers::nameIDParser),
          StaticParsers::arrayParser(StaticParsers::nameIDParser)});
}

shared_ptr<ParserStructure> Model::mappedTitleEpisode() {
  return make_shared<ParserStructure>(
      vector<string>{"tconst", "parentTconst", "seasonNumber", "episodeNumber"},
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::intParser),
          StaticParsers::orNullParser(StaticParsers::intParser),
      });
}

shared_ptr<ParserStructure> Model::mappedTitlePrincipal() {
  return make_shared<ParserStructure>(
      vector<string>{"tconst", "ordering", "nconst", "category", "job",
                     "characters"},
      vector<ParserFunction>{
          StaticParsers::imdbIdParser, StaticParsers::intParser,
          StaticParsers::nameIDParser, StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::asIs),
          StaticParsers::orNullParser(StaticParsers::asIs)});
}

shared_ptr<ParserStructure> Model::mappedTitleRating() {
  return make_shared<ParserStructure>(
      vector<string>{"tconst", "averageRating", "numVotes"},
      vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::floatParser,
          StaticParsers::intParser,
      });
}
