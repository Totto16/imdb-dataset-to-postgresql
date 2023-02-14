import {
    INameBasic,
    ITitleAlternate,
    ITitleBasic,
    ITitleCrew,
    ITitleEpisode,
    ITitlePrincipal,
    ITitleRating,
} from "./types"

const test = require("bindings")("imdb-dataset-parser") //require("./build/Release/imdb-dataset-parser.node")

export type ImdbDataType =
    | "name.basics"
    | "title.akas"
    | "title.basics"
    | "title.crew"
    | "title.episode"
    | "title.principals"
    | "title.ratings"

export interface ParseOptions<T extends ImdbDataType> {
    filePath: string
    type: T
}

export type DataTypeToInterface = {
    "name.basics": INameBasic
    "title.akas": ITitleAlternate
    "title.basics": ITitleBasic
    "title.crew": ITitleCrew
    "title.episode": ITitleEpisode
    "title.principals": ITitlePrincipal
    "title.ratings": ITitleRating
}

export async function parseFile<T extends ImdbDataType>({
    filePath,
    type,
}: ParseOptions<T>): Promise<Array<DataTypeToInterface[T]>> {
    return new Promise((resolve, reject) =>
        test.parseFile(
            filePath,
            type,
            (err: string | null, res: Array<DataTypeToInterface[T]> | null) => {
                if (err !== null) {
                    return reject(new Error(err))
                }
                if (res === null) {
                    return reject(
                        new Error(
                            `FATAL: Internal Error: No result was returned`
                        )
                    )
                }
                console.log("res")
                console.log(res)
                resolve(res)
            }
        )
    )
}
