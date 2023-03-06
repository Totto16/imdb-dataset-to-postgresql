const imdbDatasetParser = require("bindings")("imdb-dataset-parser")
const _path = require("path")

function getFilePath(path) {
    return _path.join(__dirname, "..", `tests/files/${path}`)
}

class TSVParser {
    constructor({ filePath, type }) {
        this.nativeParser = new imdbDatasetParser.NativeParser()

        this.nativeParser.on("parsedLine", (line) => {
            console.log(line)
        })
        this.nativeParser.on("error", (err) => {
            if (err instanceof Error) {
                throw err
            } else {
                throw new Error(`Received error while reading: ${err}`)
            }
        })
        this.nativeParser.on("end", () => {
            this.status = "end"
        })

        this.status = "start"

        console.log({ filePath, type })
        this.nativeParser.run(filePath, type, "auto")
    }

    async waitForEnd(checkInterval = 100) {
        return new Promise((resolve) => {
            const ID = setInterval(() => {
                if (this.status === "end") {
                    clearInterval(ID)
                    return resolve()
                }
            }, checkInterval)
        })
    }
}

;(async () => {
    {
        const parser = new TSVParser({
            filePath: getFilePath("title.ratings.tsv"),
            type: "title.ratings",
        })

        await parser.waitForEnd()
    }

    {
        const parser = new TSVParser({
            filePath: getFilePath("title.basics.tsv"),
            type: "title.basics",
        })
        await parser.waitForEnd()
    }

    {
        const parser = new TSVParser({
            filePath: getFilePath("title.akas.tsv"),
            type: "title.akas",
        })
        await parser.waitForEnd()
    }

    {
        const parser = new TSVParser({
            filePath: getFilePath("title.crew.tsv"),
            type: "title.crew",
        })
        await parser.waitForEnd()
    }

    {
        const parser = new TSVParser({
            filePath: getFilePath("title.episode.tsv"),
            type: "title.episode",
        })
        await parser.waitForEnd()
    }

    {
        const parser = new TSVParser({
            filePath: getFilePath("title.principals.tsv"),
            type: "title.principals",
        })
        await parser.waitForEnd()
    }

    {
        const parser = new TSVParser({
            filePath: getFilePath("name.basics.tsv"),
            type: "name.basics",
        })
        await parser.waitForEnd()
    }
})()
