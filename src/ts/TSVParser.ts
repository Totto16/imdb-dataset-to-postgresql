import { existsSync } from "fs"

import { DataTypeToInterface, ImdbDataType } from "./columns"
import * as n from "./NativeParser"

const { NativeParser } = require("bindings")("imdb-dataset-parser") //require("./build/Release/imdb-dataset-parser.node")

export enum IteratorState {
    NA,
    FINISHED,
    WORKING,
}

export interface ITSVParserOptions<T extends keyof DataTypeToInterface> {
    type: T
    filePath: string
    hasHead?: OmitHeadType
    maxLines?: number
}

export function sleep(ms: number): Promise<void> {
    return new Promise((resolve) => setTimeout(resolve, ms))
}

export type OmitHeadType = "auto" | boolean

const possibleValues: ImdbDataType[] = [
    "name.basics",
    "title.akas",
    "title.basics",
    "title.crew",
    "title.episode",
    "title.principals",
    "title.ratings",
]

export class TSVParser<T extends ImdbDataType>
    implements AsyncIterable<DataTypeToInterface[T]>
{
    private nativeParser: n.NativeParser<T>
    private lines: DataTypeToInterface[T][] = []
    private maxLines = 100
    private state: IteratorState = IteratorState.NA
    private lineCount = 0

    constructor(options: ITSVParserOptions<T>) {
        const { filePath, type, hasHead, maxLines } = options

        if (maxLines !== undefined) {
            this.maxLines = maxLines
        }

        if (!existsSync(filePath)) {
            throw new Error(`Filepath was invalid: '${filePath}'`)
        }

        if (!possibleValues.includes(type)) {
            throw new Error(`Not a valid type: '${type}'`)
        }

        this.state = IteratorState.WORKING

        this.nativeParser = new NativeParser()

        this.nativeParser.on("parsedLine", this.parsedLine)
        this.nativeParser.on("error", (err: Error | string) => {
            if (err instanceof Error) {
                throw err
            } else {
                throw new Error(`Received error while reading: ${err}`)
            }
        })
        this.nativeParser.on("end", () => {
            this.state = IteratorState.FINISHED
        })

        this.nativeParser.run(filePath, type, hasHead ?? "auto")
    }

    private parsedLine = (parsedLine: DataTypeToInterface[T]) => {
        ++this.lineCount

        this.lines.push(parsedLine)

        if (this.lines.length === this.maxLines) {
            this.nativeParser.pause()
        }
    }

    private getLine(): DataTypeToInterface[T] {
        const line = this.lines.shift()
        if (!line) {
            throw new Error("Cannot get line")
        }

        return line
    }

    private isEmpty() {
        return this.lines.length === 0
    }

    public async next(): Promise<IteratorResult<DataTypeToInterface[T]>> {
        if (this.isEmpty()) {
            this.nativeParser.resume()
            await this.waitForNewLines()
        }

        return new Promise((resolve, reject) => {
            if (this.finished) {
                return resolve({
                    value: null,
                    done: true,
                })
            }

            resolve({
                value: this.getLine(),
                done: false,
            })
        })
    }

    private async waitForNewLines(): Promise<void> {
        while (this.lines.length === 0) {
            if (this.finished) {
                return
            }
            await sleep(5)
            //await new Promise((resolve) => this.stream.once("data", resolve))
        }
    }

    private get finished() {
        /* console.log(
            "finished: ",
            this.state === IteratorState.FINISHED && this.lines.length === 0
        ) */
        return this.state === IteratorState.FINISHED && this.lines.length === 0
    }

    public [Symbol.asyncIterator](): AsyncIterableIterator<
        DataTypeToInterface[T]
    > {
        return this
    }

    public getLineCount(): number {
        return this.lineCount
    }
}
