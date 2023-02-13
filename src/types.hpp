
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

export interface ITitleRating {
    tconst: ImdbID
    averageRating: Float
    numVotes: Int
}

export type ANY = "A" | "B" | "C" | "TODO"

export type RegionString = symbol //`${ANY}${ANY}`

export type LanguageString = symbol */

typedef string ImdbID;

typedef string RegionString;

typedef string LanguageString;

struct ITitleAlternate {
  titleId : ImdbID;
  ordering : int;
  title : string;
  region : optional<RegionString>;
  language : optional<LanguageString>;
  types : vector<AlternativeType>;
  attributes : vector<string>;
  isOriginalTitle : optional<bool>;
};

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

typedef string Genre; // TODO use enum

typedef string TitleType;

struct ITitleBasic {
  tconst : ImdbID;
  titleType : TitleType;
  primaryTitle : string;
  originalTitle : string;
  isAdult : bool;
  startYear : optional<int>;
  endYear : optional<int>;
  runtimeMinutes : optional<int>;
  genres : vector<Genre>
}

typedef string NameID;

struct ITitleCrew {
  tconst : ImdbID;
  directors : vector<NameID>;
  writers : vector<NameID>;
}

struct ITitleEpisode {
  tconst : ImdbID;
  parentTconst : string;
  seasonNumber : optional<int>;
  episodeNumber : optional<int>;
}

struct ITitlePrincipal {
  tconst : ImdbID;
  ordering : int;
  nconst : NameID;
  category : string;
  job : optional<string>;
  characters : optional<string>;
}

struct INameBasic {
  nconst : NameID;
  primaryName : string;
  birthYear : optional<int>;
  deathYear : optional<int>;
  primaryProfession : vector<string>;
  knownForTitles : vector<ImdbID>
}
