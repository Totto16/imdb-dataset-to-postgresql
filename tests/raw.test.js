const { iterate } = require("@token-cjg/leakage")
function fail(reason = "fail was called in a test.") {
    throw new Error(reason)
}

global.fail = fail

const imdbDatasetParser = require("bindings")("imdb-dataset-parser")

describe("Verify EventEmitter Memory Is Reclaimed", function () {
    describe("Verify callback memory is reclaimed, even if callback is not waited on", function () {
        it("Should not increase memory usage over time", function (done) {
            iterate(
                () => {
                    const parser = new imdbDatasetParser.NativeParser()

                    let v = Buffer.alloc(1000)

                    parser.on("error", (err) => {
                        throw new Error(err)
                    })

                    parser.on("parsedLine", function (res) {
                        v.compare(Buffer.alloc(1000))
                    })
                },
                { iterations: 15, gcollections: 3 }
            )
            done()
        }, 5000)
    })

    /*function getFilePath(path: string): string {
        return `${__dirname}/files/${path}`
    }

    describe("Verify memory allocated in the class NativeParser is reclaimed", function () {
        it("Should not increase memory usage over time", function (done) {
            return iterate.async(
                () => {
                    return new Promise((resolve) => {
                        const parser = new imdbDatasetParser.NativeParser()

                        let v = Buffer.alloc(1000)

                        parser.on("parsedLine", function () {
                            v.compare(Buffer.alloc(1000))
                        })

                        parser.on("error", (err) => {
                            throw new Error(err)
                        })

                        parser.on("end", function () {
                            console.log("end")
                            return resolve()
                        })

                        parser.run(
                            getFilePath("title.ratings.large.tsv"),
                            "title.ratings",
                            "auto"
                        )
                    })
                },
                { iterations: 30, gcollections: 2 }
            )
        }, 15000)
    }) */
})
