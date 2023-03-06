import { TSVParser } from "../src/ts/TSVParser"
import { expect } from "@jest/globals"
import { ImdbDataType } from "../src/ts/columns"
function fail(reason = "fail was called in a test."): never {
    throw new Error(reason)
}

global.fail = fail

function getFilePath(path: string): string {
    return `${__dirname}/files/${path}`
}

function isImdbId(str: string): boolean {
    return /tt(\d){7}/.test(str)
}

describe("wrong parameters", () => {
    it("should return an error, when the type is wrong", async () => {
        try {
            const parser = new TSVParser({
                filePath: getFilePath("title.ratings.tsv"),
                type: "unknown" as ImdbDataType,
            })

            fail("it should not reach here")
        } catch (e) {
            expect(e).toBeInstanceOf(Error)
            expect((e as Error).message).toEqual("Not a valid type: 'unknown'")
        }
    })

    it("should return an error, when the filePath isn't present", async () => {
        try {
            const parser = new TSVParser({
                type: "title.ratings",
                filePath: getFilePath("s"),
            })

            fail("it should not reach here")
        } catch (e) {
            expect(e).toBeInstanceOf(Error)
            expect((e as Error).message).toEqual(
                `Filepath was invalid: '${getFilePath("s")}'`
            )
        }
    })
})

describe("imdb dataset", () => {
    const lines = 8

    it("should parse the ratings dataset", async () => {
        const parser = new TSVParser({
            filePath: getFilePath("title.ratings.tsv"),
            type: "title.ratings",
        })

        let i = 0
        for await (const rating of parser) {
            expect(rating.tconst).not.toBeUndefined()
            expect(rating.averageRating).not.toBeUndefined()
            expect(rating.numVotes).not.toBeUndefined()

            expect(typeof rating.tconst).toBe("string")
            expect(isImdbId(rating.tconst)).toBe(true)
            expect(typeof rating.averageRating).toBe("number")
            expect(typeof rating.numVotes).toBe("number")
            expect(Number.isInteger(rating.numVotes)).toBe(true)
            i++
        }

        expect(lines).toBe(parser.getLineCount())
        expect(lines).toBe(i)
    })

    it("should parse the ratings dataset with header", async () => {
        const parser = new TSVParser({
            filePath: getFilePath("title.ratings.header.tsv"),
            type: "title.ratings",
        })

        const lines = 10
        let i = 0
        for await (const rating of parser) {
            expect(rating.tconst).not.toBeUndefined()
            expect(rating.averageRating).not.toBeUndefined()
            expect(rating.numVotes).not.toBeUndefined()

            expect(typeof rating.tconst).toBe("string")
            expect(isImdbId(rating.tconst)).toBe(true)
            expect(typeof rating.averageRating).toBe("number")
            expect(typeof rating.numVotes).toBe("number")
            expect(Number.isInteger(rating.numVotes)).toBe(true)
            i++
        }

        expect(lines).toBe(parser.getLineCount())
        expect(lines).toBe(i)
    })

    it("should parse the basics dataset", async () => {
        const parser = new TSVParser({
            filePath: getFilePath("title.basics.tsv"),
            type: "title.basics",
        })

        let i = 0
        for await (const basic of parser) {
            expect(basic.tconst).not.toBeUndefined()
            expect(basic.titleType).not.toBeUndefined()
            expect(basic.primaryTitle).not.toBeUndefined()
            expect(basic.originalTitle).not.toBeUndefined()
            expect(basic.isAdult).not.toBeUndefined()
            expect(basic.startYear).not.toBeUndefined()
            expect(basic.endYear).not.toBeUndefined()
            expect(basic.runtimeMinutes).not.toBeUndefined()
            expect(basic.genres).not.toBeUndefined()

            expect(typeof basic.tconst).toBe("string")
            expect(isImdbId(basic.tconst)).toBe(true)
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })

    it("should parse the akas dataset", async () => {
        const parser = new TSVParser({
            filePath: getFilePath("title.akas.tsv"),
            type: "title.akas",
        })

        let i = 0
        for await (const alternate of parser) {
            expect(alternate.titleId).not.toBeUndefined()
            expect(alternate.ordering).not.toBeUndefined()
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })

    it("should parse the crew dataset", async () => {
        const parser = new TSVParser({
            filePath: getFilePath("title.crew.tsv"),
            type: "title.crew",
        })

        let i = 0
        for await (const crew of parser) {
            expect(crew.tconst).not.toBeUndefined()
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })

    it("should parse the episode dataset", async () => {
        const parser = new TSVParser({
            filePath: getFilePath("title.episode.tsv"),
            type: "title.episode",
        })

        let i = 0
        for await (const episode of parser) {
            expect(episode.tconst).not.toBeUndefined()
            expect(episode.parentTconst).not.toBeUndefined()
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })

    it("should parse the principal dataset", async () => {
        const parser = new TSVParser({
            filePath: getFilePath("title.principals.tsv"),
            type: "title.principals",
        })

        let i = 0
        for await (const principal of parser) {
            expect(principal.tconst).not.toBeUndefined()
            expect(principal.ordering).not.toBeUndefined()
            expect(principal.nconst).not.toBeUndefined()
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })

    it("should parse the name dataset", async () => {
        const parser = new TSVParser({
            filePath: getFilePath("name.basics.tsv"),
            type: "name.basics",
        })

        let i = 0
        for await (const principal of parser) {
            expect(principal.primaryName).not.toBeUndefined()
            expect(principal.birthYear).not.toBeUndefined()
            expect(principal.nconst).not.toBeUndefined()
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })
})
