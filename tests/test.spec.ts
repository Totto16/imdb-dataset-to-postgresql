import { ImdbDataType, parseFile } from "../module/index"
import { expect } from "@jest/globals"
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
            const result = await parseFile({
                type: "unknown" as ImdbDataType,
                filePath: getFilePath("title.ratings.tsv"),
            })
            fail("it should not reach here")
        } catch (e) {
            expect(e).toBeInstanceOf(Error)
            expect((e as Error).message).toEqual("Not a valid type: 'unknown'")
        }
    })

    it("should return an error, when the filePath isn't present", async () => {
        try {
            const result = await parseFile({
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

/* describe("imdb dataset", () => {
    const lines = 8

    it("should parse the ratings dataset", async () => {
        const result = await parseFile({
            type: "title.ratings",
            filePath: getFilePath("title.ratings.tsv"),
        })

        console.log(result)
        let i = 0
        for (const rating of result) {
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

        expect(lines).toBe(i)
    })

    it("should parse the basics dataset", async () => {
        const result = await parseFile({
            type: "title.basics",
            filePath: getFilePath("title.basics.tsv"),
        })

        let i = 0
        for (const basic of result) {
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
        const result = await parseFile({
            type: "title.akas",
            filePath: getFilePath("title.akas.tsv"),
        })

        let i = 0
        for (const alternate of result) {
            expect(alternate.titleId).not.toBeUndefined()
            expect(alternate.ordering).not.toBeUndefined()
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })

    it("should parse the crew dataset", async () => {
        const result = await parseFile({
            type: "title.crew",
            filePath: getFilePath("title.crew.tsv"),
        })

        let i = 0
        for (const crew of result) {
            expect(crew.tconst).not.toBeUndefined()
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })

    it("should parse the episode dataset", async () => {
        const result = await parseFile({
            filePath: getFilePath("title.episode.tsv"),
            type: "title.episode",
        })

        let i = 0
        for (const episode of result) {
            expect(episode.tconst).not.toBeUndefined()
            expect(episode.parentTconst).not.toBeUndefined()
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })

    it("should parse the principal dataset", async () => {
        const result = await parseFile({
            type: "title.principals",
            filePath: getFilePath("title.pricipals.tsv"),
        })

        let i = 0
        for (const principal of result) {
            expect(principal.tconst).not.toBeUndefined()
            expect(principal.ordering).not.toBeUndefined()
            expect(principal.nconst).not.toBeUndefined()
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })

    it("should parse the name dataset", async () => {
        const result = await parseFile({
            type: "name.basics",
            filePath: getFilePath("name.basics.tsv"),
        })

        let i = 0
        for (const principal of result) {
            expect(principal.primaryName).not.toBeUndefined()
            expect(principal.birthYear).not.toBeUndefined()
            expect(principal.nconst).not.toBeUndefined()
            //TODO better tests here

            i++
        }

        expect(lines).toBe(i)
    })
})
 */
