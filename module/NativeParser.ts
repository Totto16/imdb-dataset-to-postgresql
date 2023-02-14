import { DataTypeToInterface, ImdbDataType } from "./columns"
import { OmitHeadType } from "./TSVParser"

export interface ParseOptions<T extends ImdbDataType> {
    filePath: string
    type: T
    hasHead?: OmitHeadType
}

export interface NativeParser<T extends ImdbDataType>
    extends NodeJS.EventEmitter {
    on(event: "error", listener: (error: Error) => void): this
    on(
        event: "parsedLine",
        listener: (parsedLine: DataTypeToInterface[T]) => void
    ): this
    on(event: "end", listener: () => void): this
    constructor(options: ParseOptions<T>): NativeParser<T>
    pause(): void
    resume(): void
}

/* export class NativeParser<T extends ImdbDataType> extends NodeJS.EventEmitter{
    constructor(options: ParseOptions<T>)
    {
        //TODO
        imdbDatasetParser.parse(
            filePath,
            type,
            (err: string | null, res: DataTypeToInterface[T] | null) => {
                if (err !== null) {
                    this.emit("error", new Error(err))
                }
                if (res === null) {
                    return reject(
                        new Error(
                            `FATAL: Internal Error: No result was returned`
                        )
                    )
                }
                resolve(res)
            }
    } 

    pause: () => void
    resume: () => void
}
 */
