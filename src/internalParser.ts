import {
    AlternativeType,
    Float,
    Genre,
    ImdbID,
    Int,
    LanguageString,
    NameID,
    Parser,
    RegionString,
    TitleType,
} from "./types"

export const nulledValues = ["\\N"]

function isNulledValue(str: string): boolean {
    return nulledValues.includes(str)
}

export function imdbIdParser(str: string): ImdbID {
    //TODO validate
    return str as unknown as ImdbID
}

export function asIs(str: string): string {
    if (isNulledValue(str)) {
        throw new Error(`NOT nullable type was null!`)
    }
    return str
}

export function booleanParser(str: string): boolean {
    if (str === "0") {
        return false
    } else if (str === "1") {
        return true
    } else {
        throw new Error(`Couldn't parse boolean of '${str}'`)
    }
}

export function floatParser(str: string): Float {
    const float = parseFloat(str)
    if (isNaN(float)) {
        throw new Error(`Couldn't parse float of '${str}'`)
    }
    return float
}

export function intParser(str: string): Int {
    const int = parseInt(str)
    if (isNaN(int)) {
        throw new Error(`Couldn't parse int of '${str}'`)
    }
    return int
}

export function regionParser(str: string): RegionString {
    //TODO validate
    return str as unknown as RegionString
}

export function languageParser(str: string): LanguageString {
    //TODO validate
    return str as unknown as LanguageString
}

export function arrayParser<T>(parser: Parser<T>): Parser<Array<T>> {
    return (str: string): Array<T> => {
        if (isNulledValue(str)) {
            return [] as T[]
        }

        let parts = str.split(",").filter((a) => a !== "")
        return parts.map(parser)
    }
}

export function orNullParser<T>(parser: Parser<T>): Parser<T | null> {
    return (str: string): T | null => {
        if (isNulledValue(str)) {
            return null
        }

        return parser(str)
    }
}

export function alternativeTitleParser(str: string): AlternativeType {
    //TODO validate
    return str as unknown as AlternativeType
}

export function titleTypeParser(str: string): TitleType {
    //TODO validate
    return str as unknown as TitleType
}

export function genreParser(str: string): Genre {
    //TODO validate
    return str as unknown as Genre
}

export function nameIDParser(str: string): NameID {
    //TODO validate
    return str as unknown as NameID
}
