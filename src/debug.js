const imdbDatasetParser = require("bindings")("imdb-dataset-parser")
const _path = require("path")

const nativeParser = new imdbDatasetParser.NativeParser()

nativeParser.on("parsedLine", (line) => {
    console.log(line)
})
nativeParser.on("error", (err) => {
    if (err instanceof Error) {
        throw err
    } else {
        throw new Error(`Received error while reading: ${err}`)
    }
})
nativeParser.on("end", () => {
    console.log("end")
})

function getFilePath(path) {
    return _path.join(__dirname, "..", `tests/files/${path}`)
}

nativeParser.run(getFilePath("title.ratings.tsv"), "title.ratings", "auto")
