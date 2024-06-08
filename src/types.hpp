
#pragma once

#include <optional>
#include <string>
#include <vector>

using ImdbID = std::string;

struct ITitleRating {
  ImdbID tconst;
  float averageRating;
  int numVotes;
};

using RegionString = std::string;

using LanguageString = std::string;

using AlternativeType = std::string;

struct ITitleAlternate {
  ImdbID titleId;
  int ordering;
  std::string title;
  std::optional<RegionString> region;
  std::optional<LanguageString> language;
  std::vector<AlternativeType> types;
  std::vector<std::string> attributes;
  std::optional<bool> isOriginalTitle;
};

using Genre = std::string; // TODO use enum

using TitleType = std::string;

struct ITitleBasic {
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

struct ITitleCrew {
  ImdbID tconst;
  std::vector<NameID> directors;
  std::vector<NameID> writers;
};

struct ITitleEpisode {
  ImdbID tconst;
  std::string parentTconst;
  std::optional<int> seasonNumber;
  std::optional<int> episodeNumber;
};

struct ITitlePrincipal {
  ImdbID tconst;
  int ordering;
  NameID nconst;
  std::string category;
  std::optional<std::string> job;
  std::optional<std::string> characters;
};

struct INameBasic {
  NameID nconst;
  std::string primaryName;
  std::optional<int> birthYear;
  std::optional<int> deathYear;
  std::vector<std::string> primaryProfession;
  std::vector<ImdbID> knownForTitles;
};
