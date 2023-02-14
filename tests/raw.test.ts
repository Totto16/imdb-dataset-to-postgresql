import { expect } from "@jest/globals"
function fail(reason = "fail was called in a test."): never {
    throw new Error(reason)
}

global.fail = fail

const imdbDatasetParser = require("bindings")("imdb-dataset-parser")

describe("wrong raw parameters", () => {
    it("should return an error, when no first argument was given", async () => {
        try {
            imdbDatasetParser.parseFile()

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "Error: You must pass a first argument"
            )
        }
    })

    it("should return an error, when the first argument is not a string", async () => {
        try {
            imdbDatasetParser.parseFile(1)

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "Error: The first argument must be a string"
            )
        }
    })

    it("should return an error, when no second argument was given", async () => {
        try {
            imdbDatasetParser.parseFile("s")

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "Error: You must pass a second argument"
            )
        }
    })

    it("should return an error, when the second argument is not a string", async () => {
        try {
            imdbDatasetParser.parseFile("s", 1)

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "Error: The second argument must be a string"
            )
        }
    })

    it("should return an error, when no third argument was given", async () => {
        try {
            imdbDatasetParser.parseFile("s", "s")

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "Error: You must pass a third argument"
            )
        }
    })

    it("should return an error, when the third argument is not a function", async () => {
        try {
            imdbDatasetParser.parseFile("s", "s", 1)

            fail("it should not reach here")
        } catch (e) {
            expect((e as any).toString()).toEqual(
                "Error: The third argument must be a callback function"
            )
        }
    })

    it("should return no error, when all arguments are correct", async () => {
        try {
            imdbDatasetParser.parseFile(
                "s",
                "s",
                (err: string, test: null | Record<string, any>) => {
                    expect((err as any).toString()).toEqual(
                        "Not a valid type: 's'"
                    )
                }
            )
        } catch (e) {
            fail("it should not reach here")
        }
    })
})
