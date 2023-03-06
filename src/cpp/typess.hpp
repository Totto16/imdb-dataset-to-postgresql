
#pragma once

#include <optional>
#include <string>
#include <vector>

using namespace std;

/*
export type IMappedTypes<T extends Record<string, any>> = {
    order: Array<keyof T>
    declaration: MappedParsers<T>
}

export type Parser<T> = (value: string) => T

export type MappedParsers<T extends Record<string, any>> = {
    [key in keyof T]: Parser<T[key]>
}

type digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"

export type ImdbID = string // symbol
//;`tt${digit}${digit}${digit}${digit}${digit}${digit}${digit}$`

export type Float = number // symbol
export type Int = number // symbol
*/
typedef string ImdbID;

typedef struct {
  ImdbID tconst;
  float averageRating;
  int numVotes;
} ITitleRating;

/*
export type ANY = "A" | "B" | "C" | "TODO"

export type RegionString = symbol //`${ANY}${ANY}`

export type LanguageString = symbol */

typedef string RegionString;

typedef string LanguageString;

typedef string AlternativeType;

typedef struct {
  ImdbID titleId;
  int ordering;
  string title;
  optional<RegionString> region;
  optional<LanguageString> language;
  vector<AlternativeType> types;
  vector<string> attributes;
  optional<bool> isOriginalTitle;
} ITitleAlternate;

/* enum Genre {
  Action = "Action",
  Adventure = "Adventure",
  Animation = "Animation",
  Biography = "Biography",
  Comedy = "Comedy",
  Crime = "Crime",
  Documentary = "Documentary",
  Drama = "Drama",
  Family = "Family",
  Fantasy = "Fantasy",
  FilmNoir = "Film Noir",
  History = "History",
  Horror = "Horror",
  Music = "Music",
  Musical = "Musical",
  Mystery = "Mystery",
  Romance = "Romance",
  SciFi = "Sci-Fi",
  Short = "Short",
  Sport = "Sport",
  Superhero = "Superhero",
  Thriller = "Thriller",
  War = "War",
  Western = "Western",
}

enum TitleType {
  Short = "short",
  Movie = "movie",
  TVEpisode = "tvEpisode",
  TVMovie = "tvMovie",
  TVSeries = "tvSeries",
  TVShort = "tvShort",
  Video = "video",
  TVMiniSeries = "tvMiniSeries",
}

// ATTENTION: . New values may be added in the future without warning
enum AlternativeType {
  "Alternative" = "alternative",
  "DVD" = "dvd",
  "Festival" = "festival",
  "TV" = "tv",
  "Video" = "video",
  "Working" = "working",
  "Original" = "original",
  "ImdbDisplay" = "imdbDisplay",
}
 */

//TODO is this header needed

typedef string Genre; // TODO use enum

typedef string TitleType;

typedef struct {
  ImdbID tconst;
  TitleType titleType;
  string primaryTitle;
  string originalTitle;
  bool isAdult;
  optional<int> startYear;
  optional<int> endYear;
  optional<int> runtimeMinutes;
  vector<Genre> genres;
} ITitleBasic;

typedef string NameID;

typedef struct {
  ImdbID tconst;
  vector<NameID> directors;
  vector<NameID> writers;
} ITitleCrew;

typedef struct {
  ImdbID tconst;
  string parentTconst;
  optional<int> seasonNumber;
  optional<int> episodeNumber;
} ITitleEpisode;

typedef struct {
  ImdbID tconst;
  int ordering;
  NameID nconst;
  string category;
  optional<string> job;
  optional<string> characters;
} ITitlePrincipal;

typedef struct {
  NameID nconst;
  string primaryName;
  optional<int> birthYear;
  optional<int> deathYear;
  vector<string> primaryProfession;
  vector<ImdbID> knownForTitles;
} INameBasic;

/* enum ParseType {
  INameBasic,
  ITitleAlternate,
  ITitleBasic,
  ITitleCrew,
  ITitleEpisode,
  ITitlePrincipal,
  ITitleRating
}; */
/*
template <typename T> struct ParseTypeMap {
  static const std::string type;
};

template <> const std::string ParseTypeMap<INameBasic>::type = "name.basics";

template <>
const std::string ParseTypeMap<ITitleAlternate>::type = "title.akas";

template <> const std::string ParseTypeMap<ITitleBasic>::type = "title.basics";

template <> const std::string ParseTypeMap<ITitleCrew>::type = "title.crew";

template <>
const std::string ParseTypeMap<ITitleEpisode>::type = "title.episode";

template <>
const std::string ParseTypeMap<ITitlePrincipal>::type = "title.principals";

template <>
const std::string ParseTypeMap<ITitleRating>::type = "title.ratings";
 */
// template <> const std::string ParseTypeMap<string>::type = "";