import { existsSync } from "fs";
const { NativeParser } = require("bindings")("imdb-dataset-parser"); //require("./build/Release/imdb-dataset-parser.node")
export var IteratorState;
(function (IteratorState) {
    IteratorState[IteratorState["NA"] = 0] = "NA";
    IteratorState[IteratorState["FINISHED"] = 1] = "FINISHED";
    IteratorState[IteratorState["WORKING"] = 2] = "WORKING";
})(IteratorState || (IteratorState = {}));
export function sleep(ms) {
    return new Promise((resolve) => setTimeout(resolve, ms));
}
const possibleValues = [
    "name.basics",
    "title.akas",
    "title.basics",
    "title.crew",
    "title.episode",
    "title.principals",
    "title.ratings",
];
export class TSVParser {
    constructor(options) {
        this.lines = [];
        this.maxLines = 100;
        this.state = IteratorState.NA;
        this.lineCount = 0;
        this.parsedLine = (parsedLine) => {
            ++this.lineCount;
            this.lines.push(parsedLine);
            if (this.lines.length === this.maxLines) {
                this.nativeParser.pause();
            }
        };
        const { filePath, type, hasHead, maxLines } = options;
        if (maxLines !== undefined) {
            this.maxLines = maxLines;
        }
        if (!existsSync(filePath)) {
            throw new Error(`Filepath was invalid: '${filePath}'`);
        }
        if (!possibleValues.includes(type)) {
            throw new Error(`Not a valid type: '${type}'`);
        }
        this.state = IteratorState.WORKING;
        this.nativeParser = new NativeParser();
        this.nativeParser.on("parsedLine", this.parsedLine);
        this.nativeParser.on("error", (err) => {
            if (err instanceof Error) {
                throw err;
            }
            else {
                throw new Error(`Received error while reading: ${err}`);
            }
        });
        this.nativeParser.on("end", () => {
            this.state = IteratorState.FINISHED;
        });
        this.nativeParser.run(filePath, type, hasHead ?? "auto");
    }
    getLine() {
        const line = this.lines.shift();
        if (!line) {
            throw new Error("Cannot get line");
        }
        return line;
    }
    isEmpty() {
        return this.lines.length === 0;
    }
    async next() {
        if (this.isEmpty()) {
            this.nativeParser.resume();
            await this.waitForNewLines();
        }
        return new Promise((resolve, reject) => {
            if (this.finished) {
                return resolve({
                    value: null,
                    done: true,
                });
            }
            resolve({
                value: this.getLine(),
                done: false,
            });
        });
    }
    async waitForNewLines() {
        while (this.lines.length === 0) {
            if (this.finished) {
                return;
            }
            await sleep(5);
            //await new Promise((resolve) => this.stream.once("data", resolve))
        }
    }
    get finished() {
        /* console.log(
            "finished: ",
            this.state === IteratorState.FINISHED && this.lines.length === 0
        ) */
        return this.state === IteratorState.FINISHED && this.lines.length === 0;
    }
    [Symbol.asyncIterator]() {
        return this;
    }
    getLineCount() {
        return this.lineCount;
    }
}
