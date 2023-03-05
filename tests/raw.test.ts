import { expect } from "@jest/globals"
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
            parser.run(
                "s",
                "s",
                "auto"
                /*  (err: string, test: null | Record<string, any>) => {
                    expect((err as any).toString()).toEqual(
                        "Not a valid type: 's'"
                    )
                } */
            )
        } catch (e) {
            fail("it should not reach here")
        }
    })

    it("should return no error, when all arguments are correct", async () => {
        try {
            const parser = new imdbDatasetParser.NativeParser()
            parser.run(
                "s",
                "s",
                true
                /*  (err: string, test: null | Record<string, any>) => {
                    expect((err as any).toString()).toEqual(
                        "Not a valid type: 's'"
                    )
                } */
            )
        } catch (e) {
            fail("it should not reach here")
        }
    })
})
