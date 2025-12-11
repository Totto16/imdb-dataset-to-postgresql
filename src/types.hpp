
#pragma once

#include <optional>
#include <postgres/Postgres.h>
#include <string>
#include <vector>

// TODO: use better types everywhere(c++, sql scripts)
// TODO: support those type aliases in the library

namespace pg_types {

using Int4 = std::int32_t;

using Serial4 = Int4;

using Text = std::string;

using Timestamp = postgres::Time;

template <typename T> using Nullable = std::optional<T>;

using Bool = bool;

template <typename T> using Array = std::vector<T>;

using Real = float;

using DoublePrecision = double;

} // namespace pg_types

namespace pg_enums {

POSTGRES_CXX_ENUM(TitleType, "title_basics_title_type");

POSTGRES_CXX_ENUM(Genre, "title_basics_genres");

POSTGRES_CXX_ENUM(GeneralJob, "general_job");

POSTGRES_CXX_ENUM(AlternativeType, "title_akas_type");

} // namespace pg_enums

namespace pg_alias {
using ImdbID = pg_types::Text;

using NameID = pg_types::Text;

using RegionString = pg_types::Text;

using LanguageString = pg_types::Text;

} // namespace pg_alias

namespace pg_tables {

struct IMDBImporterMetadata {
  pg_types::Serial4 id;
  pg_types::Int4 version;
  pg_types::Text versionString;
  pg_types::Text description;
  pg_types::Timestamp timestamp;
  pg_types::Nullable<pg_types::Text> options;

  // primary key: id

  POSTGRES_CXX_TABLE("imdb_importer_metadata", id, version, versionString,
                     description, timestamp, options);
};

struct TitleBasic {
  pg_alias::ImdbID tconst;
  pg_enums::TitleType titleType;
  pg_types::Text primaryTitle;
  pg_types::Text originalTitle;
  pg_types::Bool isAdult;
  pg_types::Nullable<pg_types::Int4> startYear;
  pg_types::Nullable<pg_types::Int4> endYear;
  pg_types::Nullable<pg_types::Int4> runtimeMinutes;
  pg_types::Array<pg_enums::Genre> genres;

  // primary key: tconst

  POSTGRES_CXX_TABLE("title_basics", tconst, titleType, primaryTitle,
                     originalTitle, isAdult, startYear, endYear, runtimeMinutes,
                     genres);
};

struct NameBasic {
  pg_alias::NameID nconst;
  pg_types::Text primaryName;
  pg_types::Nullable<pg_types::Int4> birthYear;
  pg_types::Nullable<pg_types::Int4> deathYear;
  pg_types::Array<pg_enums::GeneralJob> primaryProfession;
  pg_types::Array<pg_alias::ImdbID> knownForTitles;

  // primary key: nconst

  POSTGRES_CXX_TABLE("name_basics", nconst, primaryName, birthYear, deathYear,
                     primaryProfession, knownForTitles);
};

struct TitleAlternate {
  pg_alias::ImdbID titleId;
  pg_types::Int4 ordering;
  pg_types::Text title;
  pg_alias::RegionString region;
  pg_types::Nullable<pg_alias::LanguageString> language;
  pg_types::Array<pg_enums::AlternativeType> types;
  pg_types::Array<pg_types::Text> attributes;
  pg_types::Bool isOriginalTitle;

  // primary keys: (titleId, ordering)
  // foreign key: titleId REFERENCES title_basics(tconst)

  POSTGRES_CXX_TABLE("title_akas", titleId, ordering, title, region, language,
                     types, attributes, isOriginalTitle);
};

struct TitleCrew {
  pg_alias::ImdbID tconst;
  pg_types::Array<pg_alias::NameID> directors;
  pg_types::Array<pg_alias::NameID> writers;

  // primary key: tconst
  // foreign key: tconst REFERENCES title_basics(tconst)

  POSTGRES_CXX_TABLE("title_crew", tconst, directors, writers);
};

struct TitleEpisode {
  pg_alias::ImdbID tconst;
  pg_alias::ImdbID parentTconst;
  pg_types::Nullable<pg_types::Int4> seasonNumber;
  pg_types::Nullable<pg_types::Int4> episodeNumber;

  // primary key: tconst
  // foreign keys:
  //                tconst REFERENCES title_basics(tconst)
  //                parentTconst REFERENCES title_basics(tconst)

  POSTGRES_CXX_TABLE("title_episode", tconst, parentTconst, seasonNumber,
                     episodeNumber);
};

struct TitlePrincipal {
  pg_alias::ImdbID tconst;
  pg_types::Int4 ordering;
  pg_alias::NameID nconst;
  pg_enums::GeneralJob category;
  pg_types::Nullable<pg_types::Text> job;
  pg_types::Array<pg_types::Text> characters;

  // primary keys: (tconst, ordering)
  // foreign keys:
  //                nconst REFERENCES name_basics(nconst)
  //                tconst REFERENCES title_basics(tconst)

  POSTGRES_CXX_TABLE("title_principals", tconst, ordering, nconst, category,
                     job, characters);
};

struct TitleRating {
  pg_alias::ImdbID tconst;
  pg_types::Real averageRating;
  pg_types::Int4 numVotes;

  // primary key: tconst
  // foreign key: tconst REFERENCES title_basics(tconst)

  POSTGRES_CXX_TABLE("title_ratings", tconst, averageRating, numVotes);
};

} // namespace pg_tables

// TODO: support serials in the postgres_cxx client
// TODO. create / deleted truncate all from c++, no .sql scripts needed
