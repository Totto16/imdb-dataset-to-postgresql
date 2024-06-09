
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "postgres.hpp"

using ImdbID = std::string;

struct TitleRating {
  ImdbID tconst;
  float averageRating;
  int numVotes;
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

using Genre = std::string; // TODO use enum

using TitleType = std::string;

struct TitleBasic {
  ImdbID tconst;
  TitleType titleType;
  std::string primaryTitle;
  std::string originalTitle;
  bool isAdult;
  std::optional<int> startYear;
  std::optional<int> endYear;
  std::optional<int> runtimeMinutes;
  std::vector<Genre> genres;
};

using NameID = std::string;

struct TitleCrew {
  ImdbID tconst;
  std::vector<NameID> directors;
  std::vector<NameID> writers;
};

struct TitleEpisode {
  ImdbID tconst;
  std::string parentTconst;
  std::optional<int> seasonNumber;
  std::optional<int> episodeNumber;
};

struct TitlePrincipal {
  ImdbID tconst;
  int ordering;
  NameID nconst;
  std::string category;
  std::optional<std::string> job;
  std::optional<std::string> characters;
};

struct NameBasic {
  NameID nconst;
  std::string primaryName;
  std::optional<int> birthYear;
  std::optional<int> deathYear;
  std::vector<std::string> primaryProfession;
  std::vector<ImdbID> knownForTitles;
};
