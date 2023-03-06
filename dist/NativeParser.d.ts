/// <reference types="node" />
import { DataTypeToInterface, ImdbDataType } from "./columns";
import { OmitHeadType } from "./TSVParser";
import { EventEmitter } from "events";
export declare class NativeParser<T extends ImdbDataType> extends EventEmitter {
    constructor();
    on(event: "error", listener: (error: Error | string) => void): this;
    on(event: "parsedLine", listener: (parsedLine: DataTypeToInterface[T]) => void): this;
    on(event: "end", listener: () => void): this;
    pause(): void;
    run(filePath: string, type: T, hasHead?: OmitHeadType): void;
    resume(): void;
    removeAllListeners(event?: string): this;
    eventNames(): string[];
}
