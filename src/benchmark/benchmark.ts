import { join } from "path"
import { ImdbDataType, TSVParser } from "imdb-dataset-parser"

const toSearch = "tt17371078"

//TODO: npm i git+https://github.com/Totto16/imdb-dataset-parser.git

//TODO automate
// to test this, download all datasets from https://datasets.imdbws.com/ and unpack them into "files/${type}.tsv/data.tsv"
function getFilePath(path: string): string {
    return join(__dirname, "..", `/files/${path}/data.tsv`)
}

async function run() {
    const parser = new TSVParser({
        filePath: getFilePath("title.ratings.tsv"),
        type: "title.ratings",
    })

    let i = 0
    for await (const rating of parser) {
        ++i
        if (rating.tconst === toSearch) {
            console.log(rating)
        }
    }

    console.log(`${i} lines!`)
}

async function runGeneric(type: ImdbDataType) {
    const parser = new TSVParser({
        filePath: getFilePath(`${type}.tsv`),
        type,
    })

    for await (const rating of parser) {
        //
    }

    console.log(`${type} ${parser.getLineCount()} lines!`)
}

async function start() {
    //TODO measure both with leakage!
    const types: ImdbDataType[] = [
        /*         "name.basics",
        "title.akas",
        "title.basics",
        "title.crew",
        "title.episode",
        "title.principals", // only js: 3:55.121 (m:ss.mmm)  -- cpp:  */
        "title.ratings", // only js: 2.646s  -- cpp: 2.956s SLOWER
    ]

    for (const type of types) {
        const label = `parseTime-${type}`

        console.time(label)
        console.log(`Now starting parsing of: ${type}`)
        await runGeneric(type)

        console.timeEnd(label)
    }
}

start()
