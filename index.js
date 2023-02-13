const test = require("bindings")("imdb-dataset-parser")

async function parseFile({ filename, type }) {
    return new Promise((resolve, reject) =>
        test.parseFile(filename, type, (err, res) => {
            if (err) {
                //TODO: better reject Error!
                return reject(err)
            }
            resolve(res)
        })
    )
}

module.exports = {
    parseFile,
}
