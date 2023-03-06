import { IMappedTypes, INameBasic, ITitleAlternate, ITitleBasic, ITitleCrew, ITitleEpisode, ITitlePrincipal, ITitleRating } from "./types";
export type ImdbDataType = "name.basics" | "title.akas" | "title.basics" | "title.crew" | "title.episode" | "title.principals" | "title.ratings";
export type DataTypeToInterface = {
    "name.basics": INameBasic;
    "title.akas": ITitleAlternate;
    "title.basics": ITitleBasic;
    "title.crew": ITitleCrew;
    "title.episode": ITitleEpisode;
    "title.principals": ITitlePrincipal;
    "title.ratings": ITitleRating;
};
export type AvailableInterfaces = INameBasic | ITitleAlternate | ITitleBasic | ITitleCrew | ITitleEpisode | ITitlePrincipal | ITitleRating;
export type DataTypeMap = {
    [key in ImdbDataType]: IMappedTypes<DataTypeToInterface[key]>;
};
