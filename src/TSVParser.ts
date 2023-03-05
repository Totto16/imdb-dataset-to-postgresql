import { createReadStream, existsSync, ReadStream } from "fs"
import readline from "readline"

import { dataTypeMap, DataTypeToInterface, ImdbDataType } from "./columns"
import { Model } from "./Model"
import { IMappedTypes } from "./types"
import { sleep } from "./util"

export enum IteratorState {
    NA,
    FINISHED,
    WORKING,
}

export interface ITSVParserOptions<T extends keyof DataTypeToInterface> {
    type: T
    filePath: string
    hasHead?: boolean
    maxLines?: number
}

type OmitHeadTYpe = "auto" | boolean

export class TSVParser<T extends ImdbDataType>
    implements AsyncIterable<DataTypeToInterface[T]>
{
    private stream: ReadStream
    private lines: DataTypeToInterface[T][] = []
    private maxLines = 100
    private state: IteratorState = IteratorState.NA
    private lineCount = 0
    private omitHead: OmitHeadTYpe = "auto"

    private model: Model<DataTypeToInterface[T]>

    constructor(options: ITSVParserOptions<T>) {
        const { filePath, type, hasHead, maxLines } = options
        if (hasHead !== undefined) {
            this.omitHead = hasHead
        }

        if (maxLines !== undefined) {
            this.maxLines = maxLines
        }

        if (!existsSync(filePath)) {
            throw new Error(`Cannot find file at path: ${filePath}`)
        }

        this.model = new Model<DataTypeToInterface[T]>(
            dataTypeMap[type] as IMappedTypes<DataTypeToInterface[T]>
        )

        this.state = IteratorState.WORKING

        this.stream = createReadStream(filePath)

        let rl = readline.createInterface({ input: this.stream })
        rl.on("line", this.onLine)
        rl.on("error", (error) => console.error(error))
        rl.on("close", () => {
            this.state = IteratorState.FINISHED
        })
    }

    private onLine = (line: string) => {
        if (!line) {
            return
        }

        ++this.lineCount

        if (this.lineCount === 1 && this.omitHead !== false) {
            if (this.omitHead === "auto") {
                if (this.model.isHead(line)) {
                    return
                }
            }
            return
        }

        const parsedLine = this.model.parseLine(line)
        this.lines.push(parsedLine)

        if (this.lines.length === this.maxLines) {
            this.stream.pause()
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
            this.stream.resume()
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
