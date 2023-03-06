import { DataTypeToInterface, ImdbDataType } from "./columns";
export declare enum IteratorState {
    NA = 0,
    FINISHED = 1,
    WORKING = 2
}
export interface ITSVParserOptions<T extends keyof DataTypeToInterface> {
    type: T;
    filePath: string;
    hasHead?: OmitHeadType;
    maxLines?: number;
}
export declare function sleep(ms: number): Promise<void>;
export type OmitHeadType = "auto" | boolean;
export declare class TSVParser<T extends ImdbDataType> implements AsyncIterable<DataTypeToInterface[T]> {
    private nativeParser;
    private lines;
    private maxLines;
    private state;
    private lineCount;
    constructor(options: ITSVParserOptions<T>);
    private parsedLine;
    private getLine;
    private isEmpty;
    next(): Promise<IteratorResult<DataTypeToInterface[T]>>;
    private waitForNewLines;
    private get finished();
    [Symbol.asyncIterator](): AsyncIterableIterator<DataTypeToInterface[T]>;
    getLineCount(): number;
}
