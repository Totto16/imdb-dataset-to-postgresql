
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "InternalParsers.hpp"
#include "Models.hpp"
#include "ParserStructure.hpp"
#include "types.hpp"

std::shared_ptr<Parseable> Model::mappedTitleBasic() {
  return make_shared<ParserStructure<pg_tables::TitleBasic>>(
      std::vector<AdvancedParser<pg_tables::TitleBasic>>{
          MAKE_PARSER(pg_tables::TitleBasic, tconst, StaticParsers::asIs),
          MAKE_PARSER(pg_tables::TitleBasic, titleType,
                      StaticParsers::enumParser<pg_enums::TitleType>),
          MAKE_PARSER(pg_tables::TitleBasic, primaryTitle, StaticParsers::asIs),
          MAKE_PARSER(pg_tables::TitleBasic, originalTitle,
                      StaticParsers::asIs),
          MAKE_PARSER(pg_tables::TitleBasic, isAdult,
                      StaticParsers::booleanParser),
          MAKE_PARSER(pg_tables::TitleBasic, startYear,
                      StaticParsers::orNullParser<pg_types::Int4>(
                          StaticParsers::intParser<pg_types::Int4>)),
          MAKE_PARSER(pg_tables::TitleBasic, endYear,
                      StaticParsers::orNullParser<pg_types::Int4>(
                          StaticParsers::intParser<pg_types::Int4>)),
          MAKE_PARSER(pg_tables::TitleBasic, runtimeMinutes,
                      StaticParsers::orNullParser<pg_types::Int4>(
                          StaticParsers::intParser<pg_types::Int4>)),
          MAKE_PARSER(pg_tables::TitleBasic, genres,
                      StaticParsers::arrayParser<pg_enums::Genre>(
                          StaticParsers::enumParser<pg_enums::Genre>))});
}

std::shared_ptr<Parseable> Model::mappedNameBasic() {
  return make_shared<ParserStructure<pg_tables::NameBasic>>(
      std::vector<AdvancedParser<pg_tables::NameBasic>>{
          MAKE_PARSER(pg_tables::NameBasic, nconst, StaticParsers::asIs),
          MAKE_PARSER(pg_tables::NameBasic, primaryName, StaticParsers::asIs),
          MAKE_PARSER(pg_tables::NameBasic, birthYear,
                      StaticParsers::orNullParser<pg_types::Int4>(
                          StaticParsers::intParser<pg_types::Int4>)),
          MAKE_PARSER(pg_tables::NameBasic, deathYear,
                      StaticParsers::orNullParser<pg_types::Int4>(
                          StaticParsers::intParser<pg_types::Int4>)),
          MAKE_PARSER(pg_tables::NameBasic, primaryProfession,
                      StaticParsers::arrayParser<pg_enums::GeneralJob>(
                          StaticParsers::enumParser<pg_enums::GeneralJob>)),
          MAKE_PARSER(pg_tables::NameBasic, knownForTitles,
                      StaticParsers::arrayParser<pg_alias::ImdbID>(
                          StaticParsers::asIs))});
}

std::shared_ptr<Parseable> Model::mappedTitleAlternate() {
  return make_shared<ParserStructure<pg_tables::TitleAlternate>>(
      std::vector<AdvancedParser<pg_tables::TitleAlternate>>{
          MAKE_PARSER(pg_tables::TitleAlternate, titleId,
                      StaticParsers::asIs), //
          MAKE_PARSER(pg_tables::TitleAlternate, ordering,
                      StaticParsers::intParser<pg_types::Int4>),              //
          MAKE_PARSER(pg_tables::TitleAlternate, title, StaticParsers::asIs), //
          MAKE_PARSER(pg_tables::TitleAlternate, region,
                      StaticParsers::asIs), //
          MAKE_PARSER(pg_tables::TitleAlternate, language,
                      StaticParsers::asIsNullable), //
          MAKE_PARSER(
              pg_tables::TitleAlternate, types,
              StaticParsers::arrayParser<pg_enums::AlternativeType>(
                  StaticParsers::enumParser<pg_enums::AlternativeType>)), //
          MAKE_PARSER(pg_tables::TitleAlternate, attributes,
                      StaticParsers::arrayParser<pg_types::Text>(
                          StaticParsers::asIs)), //
          MAKE_PARSER(pg_tables::TitleAlternate, isOriginalTitle,
                      StaticParsers::booleanParser)});
}

std::shared_ptr<Parseable> Model::mappedTitleCrew() {
  return make_shared<ParserStructure<pg_tables::TitleCrew>>(
      std::vector<AdvancedParser<pg_tables::TitleCrew>>{
          MAKE_PARSER(pg_tables::TitleCrew, tconst, StaticParsers::asIs),
          MAKE_PARSER(pg_tables::TitleCrew, directors,
                      StaticParsers::arrayParser<pg_alias::NameID>(
                          StaticParsers::asIs)),
          MAKE_PARSER(pg_tables::TitleCrew, writers,
                      StaticParsers::arrayParser<pg_alias::NameID>(
                          StaticParsers::asIs))});
}

std::shared_ptr<Parseable> Model::mappedTitleEpisode() {
  return make_shared<ParserStructure<pg_tables::TitleEpisode>>(
      std::vector<AdvancedParser<pg_tables::TitleEpisode>>{
          MAKE_PARSER(pg_tables::TitleEpisode, tconst, StaticParsers::asIs),
          MAKE_PARSER(pg_tables::TitleEpisode, parentTconst,
                      StaticParsers::asIs),
          MAKE_PARSER(pg_tables::TitleEpisode, seasonNumber,
                      StaticParsers::orNullParser<pg_types::Int4>(
                          StaticParsers::intParser<pg_types::Int4>)),
          MAKE_PARSER(pg_tables::TitleEpisode, episodeNumber,
                      StaticParsers::orNullParser<pg_types::Int4>(
                          StaticParsers::intParser<pg_types::Int4>)),
      });
}

std::shared_ptr<Parseable> Model::mappedTitlePrincipal() {
  return make_shared<ParserStructure<pg_tables::TitlePrincipal>>(
      std::vector<AdvancedParser<pg_tables::TitlePrincipal>>{
          MAKE_PARSER(pg_tables::TitlePrincipal, tconst, StaticParsers::asIs),
          MAKE_PARSER(pg_tables::TitlePrincipal, ordering,
                      StaticParsers::intParser<pg_types::Int4>),
          MAKE_PARSER(pg_tables::TitlePrincipal, nconst, StaticParsers::asIs),
          MAKE_PARSER(pg_tables::TitlePrincipal, category,
                      StaticParsers::enumParser<pg_enums::GeneralJob>),
          MAKE_PARSER(
              pg_tables::TitlePrincipal, job,
              StaticParsers::orNullParser<pg_types::Text>(StaticParsers::asIs)),
          MAKE_PARSER(pg_tables::TitlePrincipal, characters,
                      StaticParsers::arrayParser<pg_types::Text>(
                          StaticParsers::asIs))});
}

std::shared_ptr<Parseable> Model::mappedTitleRating() {
  return make_shared<ParserStructure<pg_tables::TitleRating>>(
      std::vector<AdvancedParser<pg_tables::TitleRating>>{
          MAKE_PARSER(pg_tables::TitleRating, tconst, StaticParsers::asIs),
          MAKE_PARSER(pg_tables::TitleRating, averageRating,
                      StaticParsers::floatParser),
          MAKE_PARSER(pg_tables::TitleRating, numVotes,
                      StaticParsers::intParser<pg_types::Int4>),
      });
}
