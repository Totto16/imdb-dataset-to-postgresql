import { expect } from "@jest/globals"
import { iterate } from "@token-cjg/leakage"

function fail(reason = "fail was called in a test."): never {
    throw new Error(reason)
}

global.fail = fail

const imdbDatasetParser = require("bindings")("imdb-dataset-parser")

describe("wrong raw parameters", () => {
    it("should return an error, when no first argument was given", async () => {
        try {
            const parser = new imdbDatasetParser.NativeParser()
            parser.run()

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "TypeError: Wrong number of arguments: expected 2 or 3"
            )
        }
    })

    it("should return an error, when the first argument is not a string", async () => {
        try {
            const parser = new imdbDatasetParser.NativeParser()
            parser.run(1)

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "TypeError: Wrong number of arguments: expected 2 or 3"
            )
        }
    })

    it("should return an error, when no second argument was given", async () => {
        try {
            const parser = new imdbDatasetParser.NativeParser()
            parser.run("s")

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "TypeError: Wrong number of arguments: expected 2 or 3"
            )
        }
    })

    it("should return an error, when the second argument is not a string", async () => {
        try {
            const parser = new imdbDatasetParser.NativeParser()
            parser.run("s", 1)

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "TypeError: The second argument must be a string"
            )
        }
    })

    it("should return no error, when all arguments are correct and only 2 arguments are given", async () => {
        try {
            const parser = new imdbDatasetParser.NativeParser()
            parser.run("s", "s")
        } catch (e) {
            fail("it should not reach here")
        }
    })

    it("should return an error, when the third argument is not a boolean", async () => {
        try {
            const parser = new imdbDatasetParser.NativeParser()
            parser.run("s", "s", 1)

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "TypeError: The third argument must be boolean or 'auto'"
            )
        }
    })

    it("should return an error, when the third argument is not 'auto'", async () => {
        try {
            const parser = new imdbDatasetParser.NativeParser()
            parser.run("s", "s", "test")

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "TypeError: The third argument must be the string 'auto', not another string"
            )
        }
    })

    it("should return no error, when all arguments are correct", async () => {
        try {
            const parser = new imdbDatasetParser.NativeParser()
            parser.run("s", "s", "auto")
        } catch (e) {
            fail("it should not reach here")
        }
    })

    it("should return no error, when all arguments are correct", async () => {
        try {
            const parser = new imdbDatasetParser.NativeParser()
            parser.run("s", "s", true)
        } catch (e) {
            fail("it should not reach here")
        }
    })
})

// this 2 tests don't work with ts, for some reason, they even do't fail at all after the timeout!
/* describe("Verify EventEmitter Memory Is Reclaimed", function () {
    describe("Verify callback memory is reclaimed, even if callback is not waited on", function () {
        it("Should not increase memory usage over time", function (done) {
            iterate(
                () => {
                    const parser = new imdbDatasetParser.NativeParser()

                    let v = Buffer.alloc(1000)

                    parser.on("error", (err: string) => {
                        throw new Error(err)
                    })

                    parser.on("parsedLine", function (e: unknown) {
                        console.log(e)
                        v.compare(Buffer.alloc(1000))
                    })
                },
                { iterations: 15, gcollections: 3 }
            )
            done()
        }, 5000)
    })

    function getFilePath(path: string): string {
        return `${__dirname}/files/${path}`
    } */

/*
    describe("Verify memory allocated in the test-class EmitterThing is reclaimed", function () {
        it("Should not increase memory usage over time", function () {

             return iterate.async(() => {
                return new Promise<void>((resolve, reject) => {
                    const parser = new imdbDatasetParser.NativeParser()

                    let v = Buffer.alloc(1000)

                    parser.on("parsedLine", function () {
                        v.compare(Buffer.alloc(1000))
                    })

                    parser.on("end", function () {
                        return resolve()
                    })

                    parser.run(
                        getFilePath("title.ratings.tsv"),
                        "title.ratings",
                        "auto"
                    )
                })
            }) 
        })
    })
})*/
