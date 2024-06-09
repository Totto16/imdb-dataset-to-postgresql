
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "InternalParsers.hpp"
#include "Models.hpp"
#include "ParserStructure.hpp"
#include "types.hpp"

std::shared_ptr<Parseable> Model::mappedTitleAlternate() {
  return make_shared<ParserStructure<TitleAlternate>>(
      std::vector<AdvancedParser<TitleAlternate>>{
          MAKE_PARSER(TitleAlternate, titleId, StaticParsers::asIs),       //
          MAKE_PARSER(TitleAlternate, ordering, StaticParsers::intParser), //
          MAKE_PARSER(TitleAlternate, title, StaticParsers::asIs),         //
          MAKE_PARSER(TitleAlternate, region,
                      StaticParsers::asIsNullable), //
          MAKE_PARSER(TitleAlternate, language,
                      StaticParsers::asIsNullable), //
          MAKE_PARSER(
              TitleAlternate, types,
              StaticParsers::arrayParser<std::string>(StaticParsers::asIs)), //
          MAKE_PARSER(
              TitleAlternate, attributes,
              StaticParsers::arrayParser<std::string>(StaticParsers::asIs)), //
          MAKE_PARSER(TitleAlternate, isOriginalTitle,
                      StaticParsers::orNullParser<bool>(
                          StaticParsers::booleanParser))});
}
/*
std::shared_ptr<Parseable> Model::mappedNameBasic() {
  return make_shared<ParserStructure>(
      std::vector<std::string>{
          "nconst",
          "primaryName",
          "birthYear",
          "deathYear",
          "primaryProfession",
          "knownForTitles",
      },
      std::vector<ParserFunction>{
          StaticParsers::nameIDParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::intParser),
          StaticParsers::orNullParser(StaticParsers::intParser),
          StaticParsers::arrayParser(StaticParsers::asIs),
          StaticParsers::arrayParser(StaticParsers::imdbIdParser),
      });
}

std::shared_ptr<Parseable> Model::mappedTitleBasic() {
  return make_shared<ParserStructure>(
      std::vector<std::string>{
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
      std::vector<ParserFunction>{
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

std::shared_ptr<Parseable> Model::mappedTitleCrew() {
  return make_shared<ParserStructure>(
      std::vector<std::string>{"tconst", "directors", "writers"},
      std::vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::arrayParser(StaticParsers::nameIDParser),
          StaticParsers::arrayParser(StaticParsers::nameIDParser)});
}

std::shared_ptr<Parseable> Model::mappedTitleEpisode() {
  return make_shared<ParserStructure>(
      std::vector<std::string>{"tconst", "parentTconst", "seasonNumber",
                               "episodeNumber"},
      std::vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::intParser),
          StaticParsers::orNullParser(StaticParsers::intParser),
      });
}

std::shared_ptr<Parseable> Model::mappedTitlePrincipal() {
  return make_shared<ParserStructure>(
      std::vector<std::string>{"tconst", "ordering", "nconst", "category",
                               "job", "characters"},
      std::vector<ParserFunction>{
          StaticParsers::imdbIdParser, StaticParsers::intParser,
          StaticParsers::nameIDParser, StaticParsers::asIs,
          StaticParsers::orNullParser(StaticParsers::asIs),
          StaticParsers::orNullParser(StaticParsers::asIs)});
}

std::shared_ptr<Parseable> Model::mappedTitleRating() {
  return make_shared<ParserStructure>(
      std::vector<std::string>{"tconst", "averageRating", "numVotes"},
      std::vector<ParserFunction>{
          StaticParsers::imdbIdParser,
          StaticParsers::floatParser,
          StaticParsers::intParser,
      });
}
 */
