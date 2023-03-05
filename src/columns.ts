import {
    alternativeTitleParser,
    arrayParser,
    asIs,
    booleanParser,
    floatParser,
    genreParser,
    imdbIdParser,
    intParser,
    languageParser,
    nameIDParser,
    orNullParser,
    regionParser,
    titleTypeParser,
} from "./internalParser"
import {
    IMappedTypes,
    INameBasic,
    ITitleAlternate,
    ITitleBasic,
    ITitleCrew,
    ITitleEpisode,
    ITitlePrincipal,
    ITitleRating,
} from "./types"

export const mappedTitleRating: IMappedTypes<ITitleRating> = {
    order: ["tconst", "averageRating", "numVotes"],
    declaration: {
        tconst: imdbIdParser,
        averageRating: floatParser,
        numVotes: intParser,
    },
}

export const mappedTitleAlternate: IMappedTypes<ITitleAlternate> = {
    order: [
        "titleId",
        "ordering",
        "title",
        "region",
        "language",
        "types",
        "attributes",
        "isOriginalTitle",
    ],
    declaration: {
        titleId: imdbIdParser,
        ordering: parseInt,
        title: asIs,
        region: orNullParser(regionParser),
        language: orNullParser(languageParser),
        types: arrayParser(alternativeTitleParser),
        attributes: arrayParser(asIs),
        isOriginalTitle: orNullParser(booleanParser),
    },
}

export const mappedTitleBasic: IMappedTypes<ITitleBasic> = {
    order: [
        "tconst",
        "titleType",
        "primaryTitle",
        "originalTitle",
        "isAdult",
        "startYear",
        "endYear",
        "runtimeMinutes",
        "genres",
    ],
    declaration: {
        tconst: imdbIdParser,
        titleType: titleTypeParser,
        primaryTitle: asIs,
        originalTitle: asIs,
        isAdult: booleanParser,
        startYear: orNullParser(intParser),
        endYear: orNullParser(intParser),
        runtimeMinutes: orNullParser(intParser),
        genres: arrayParser(genreParser),
    },
}

export const mappedTitleCrew: IMappedTypes<ITitleCrew> = {
    order: ["tconst", "directors", "writers"],
    declaration: {
        tconst: imdbIdParser,
        directors: arrayParser(nameIDParser),
        writers: arrayParser(nameIDParser),
    },
}

export const mappedTitleEpisode: IMappedTypes<ITitleEpisode> = {
    order: ["tconst", "parentTconst", "seasonNumber", "episodeNumber"],
    declaration: {
        tconst: imdbIdParser,
        parentTconst: asIs,
        seasonNumber: orNullParser(intParser),
        episodeNumber: orNullParser(intParser),
    },
}

export const mappedTitlePrincipal: IMappedTypes<ITitlePrincipal> = {
    order: ["tconst", "ordering", "nconst", "category", "job", "characters"],
    declaration: {
        tconst: imdbIdParser,
        ordering: intParser,
        nconst: nameIDParser,
        category: asIs,
        job: orNullParser(asIs),
        characters: orNullParser(asIs),
    },
}

export const mappedNameBasic: IMappedTypes<INameBasic> = {
    order: [
        "nconst",
        "primaryName",
        "birthYear",
        "deathYear",
        "primaryProfession",
        "knownForTitles",
    ],
    declaration: {
        nconst: nameIDParser,
        primaryName: asIs,
        birthYear: orNullParser(intParser),
        deathYear: orNullParser(intParser),
        primaryProfession: arrayParser(asIs),
        knownForTitles: arrayParser(imdbIdParser),
    },
}

export type ImdbDataType =
    | "name.basics"
    | "title.akas"
    | "title.basics"
    | "title.crew"
    | "title.episode"
    | "title.principals"
    | "title.ratings"

export type DataTypeToInterface = {
    "name.basics": INameBasic
    "title.akas": ITitleAlternate
    "title.basics": ITitleBasic
    "title.crew": ITitleCrew
    "title.episode": ITitleEpisode
    "title.principals": ITitlePrincipal
    "title.ratings": ITitleRating
}

export type AvailableInterfaces =
    | INameBasic
    | ITitleAlternate
    | ITitleBasic
    | ITitleCrew
    | ITitleEpisode
    | ITitleEpisode
    | ITitlePrincipal
    | ITitleRating

export type DataTypeMap = {
    [key in ImdbDataType]: IMappedTypes<DataTypeToInterface[key]>
}

export const dataTypeMap: DataTypeMap = {
    "name.basics": mappedNameBasic,
    "title.akas": mappedTitleAlternate,
    "title.basics": mappedTitleBasic,
    "title.crew": mappedTitleCrew,
    "title.episode": mappedTitleEpisode,
    "title.principals": mappedTitlePrincipal,
    "title.ratings": mappedTitleRating,
}
