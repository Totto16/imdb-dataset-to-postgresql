const test = require("bindings")("imdb-dataset-parser")

async function parseFile(x) {
    return new Promise((resolve, reject) =>
        test.parseFile(x, (err, res) => {
            if (err) {
                return reject(err)
            }
            resolve(res)
        })
    )
}

module.exports = {
    parseFile,
}
