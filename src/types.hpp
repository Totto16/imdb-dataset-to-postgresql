
#pragma once

#include <optional>
#include <string>
#include <vector>
#include <postgres/Postgres.h>

using ImdbID = std::string;

struct TitleRating {
  ImdbID tconst;
  float averageRating;
  std::int64_t numVotes;

  POSTGRES_CXX_TABLE("title.ratings", tconst, averageRating, numVotes);
};

using RegionString = std::string;

using LanguageString = std::string;

using AlternativeType = std::string;

struct TitleAlternate {
  ImdbID titleId;
  std::int64_t ordering;
  std::string title;
  std::optional<RegionString> region;
  std::optional<LanguageString> language;
  std::vector<AlternativeType> types;
  std::vector<std::string> attributes;
  std::optional<bool> isOriginalTitle;

  POSTGRES_CXX_TABLE("title.akas", titleId, ordering, title, region, language,
                     types, attributes, isOriginalTitle);
};

using Genre = std::string;

using TitleType = std::string;

struct TitleBasic {
  ImdbID tconst;
  TitleType titleType;
  std::string primaryTitle;
  std::string originalTitle;
  bool isAdult;
  std::optional<std::int64_t> startYear;
  std::optional<std::int64_t> endYear;
  std::optional<std::int64_t> runtimeMinutes;
  std::vector<Genre> genres;

  POSTGRES_CXX_TABLE("title.basics", tconst, titleType, primaryTitle,
                     originalTitle, isAdult, startYear, endYear, runtimeMinutes,
                     genres);
};

using NameID = std::string;

struct TitleCrew {
  ImdbID tconst;
  std::vector<NameID> directors;
  std::vector<NameID> writers;

  POSTGRES_CXX_TABLE("title.crew", tconst, directors, writers);
};

struct TitleEpisode {
  ImdbID tconst;
  std::string parentTconst;
  std::optional<std::int64_t> seasonNumber;
  std::optional<std::int64_t> episodeNumber;

  POSTGRES_CXX_TABLE("title.episode", tconst, parentTconst, seasonNumber,
                     episodeNumber);
};

struct TitlePrincipal {
  ImdbID tconst;
  std::int64_t ordering;
  NameID nconst;
  std::string category;
  std::optional<std::string> job;
  std::optional<std::string> characters;

  POSTGRES_CXX_TABLE("title.principals", tconst, ordering, nconst, category,
                     job, characters);
};

struct NameBasic {
  NameID nconst;
  std::string primaryName;
  std::optional<std::int64_t> birthYear;
  std::optional<std::int64_t> deathYear;
  std::vector<std::string> primaryProfession;
  std::vector<ImdbID> knownForTitles;

  POSTGRES_CXX_TABLE("name.basics", nconst, primaryName, birthYear, deathYear,
                     primaryProfession, knownForTitles);
};
