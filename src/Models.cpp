
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
          MAKE_PARSER(TitleAlternate, titleId, StaticParsers::asIs), //
          MAKE_PARSER(TitleAlternate, ordering,
                      StaticParsers::intParser<std::int64_t>),     //
          MAKE_PARSER(TitleAlternate, title, StaticParsers::asIs), //
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

std::shared_ptr<Parseable> Model::mappedNameBasic() {
  return make_shared<ParserStructure<NameBasic>>(
      std::vector<AdvancedParser<NameBasic>>{
          MAKE_PARSER(NameBasic, nconst, StaticParsers::asIs),
          MAKE_PARSER(NameBasic, primaryName, StaticParsers::asIs),
          MAKE_PARSER(NameBasic, birthYear,
                      StaticParsers::orNullParser<std::int64_t>(
                          StaticParsers::intParser<std::int64_t>)),
          MAKE_PARSER(NameBasic, deathYear,
                      StaticParsers::orNullParser<std::int64_t>(
                          StaticParsers::intParser<std::int64_t>)),
          MAKE_PARSER(
              NameBasic, primaryProfession,
              StaticParsers::arrayParser<std::string>(StaticParsers::asIs)),
          MAKE_PARSER(
              NameBasic, knownForTitles,
              StaticParsers::arrayParser<std::string>(StaticParsers::asIs))});
}

std::shared_ptr<Parseable> Model::mappedTitleBasic() {
  return make_shared<ParserStructure<TitleBasic>>(
      std::vector<AdvancedParser<TitleBasic>>{
          MAKE_PARSER(TitleBasic, tconst, StaticParsers::asIs),
          MAKE_PARSER(TitleBasic, titleType, StaticParsers::asIs),
          MAKE_PARSER(TitleBasic, primaryTitle, StaticParsers::asIs),
          MAKE_PARSER(TitleBasic, originalTitle, StaticParsers::asIs),
          MAKE_PARSER(TitleBasic, isAdult, StaticParsers::booleanParser),
          MAKE_PARSER(TitleBasic, startYear,
                      StaticParsers::orNullParser<std::int64_t>(
                          StaticParsers::intParser<std::int64_t>)),
          MAKE_PARSER(TitleBasic, endYear,
                      StaticParsers::orNullParser<std::int64_t>(
                          StaticParsers::intParser<std::int64_t>)),
          MAKE_PARSER(TitleBasic, runtimeMinutes,
                      StaticParsers::orNullParser<std::int64_t>(
                          StaticParsers::intParser<std::int64_t>)),
          MAKE_PARSER(
              TitleBasic, genres,
              StaticParsers::arrayParser<std::string>(StaticParsers::asIs))});
}

std::shared_ptr<Parseable> Model::mappedTitleCrew() {
  return make_shared<ParserStructure<TitleCrew>>(
      std::vector<AdvancedParser<TitleCrew>>{
          MAKE_PARSER(TitleCrew, tconst, StaticParsers::asIs),
          MAKE_PARSER(
              TitleCrew, directors,
              StaticParsers::arrayParser<std::string>(StaticParsers::asIs)),
          MAKE_PARSER(
              TitleCrew, writers,
              StaticParsers::arrayParser<std::string>(StaticParsers::asIs))});
}

std::shared_ptr<Parseable> Model::mappedTitleEpisode() {
  return make_shared<ParserStructure<TitleEpisode>>(
      std::vector<AdvancedParser<TitleEpisode>>{
          MAKE_PARSER(TitleEpisode, tconst, StaticParsers::asIs),
          MAKE_PARSER(TitleEpisode, parentTconst, StaticParsers::asIs),
          MAKE_PARSER(TitleEpisode, seasonNumber,
                      StaticParsers::orNullParser<std::int64_t>(
                          StaticParsers::intParser<std::int64_t>)),
          MAKE_PARSER(TitleEpisode, episodeNumber,
                      StaticParsers::orNullParser<std::int64_t>(
                          StaticParsers::intParser<std::int64_t>)),
      });
}

std::shared_ptr<Parseable> Model::mappedTitlePrincipal() {
  return make_shared<ParserStructure<TitlePrincipal>>(
      std::vector<AdvancedParser<TitlePrincipal>>{
          MAKE_PARSER(TitlePrincipal, tconst, StaticParsers::asIs),
          MAKE_PARSER(TitlePrincipal, ordering,
                      StaticParsers::intParser<std::int64_t>),
          MAKE_PARSER(TitlePrincipal, nconst, StaticParsers::asIs),
          MAKE_PARSER(TitlePrincipal, category, StaticParsers::asIs),
          MAKE_PARSER(
              TitlePrincipal, job,
              StaticParsers::orNullParser<std::string>(StaticParsers::asIs)),
          MAKE_PARSER(
              TitlePrincipal, characters,
              StaticParsers::orNullParser<std::string>(StaticParsers::asIs))});
}

std::shared_ptr<Parseable> Model::mappedTitleRating() {
  return make_shared<ParserStructure<TitleRating>>(
      std::vector<AdvancedParser<TitleRating>>{
          MAKE_PARSER(TitleRating, tconst, StaticParsers::asIs),
          MAKE_PARSER(TitleRating, averageRating, StaticParsers::doubleParser),
          MAKE_PARSER(TitleRating, numVotes,
                      StaticParsers::intParser<std::int64_t>),
      });
}
