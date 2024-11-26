#!/usr/bin/env bash

set -ex

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

SRC_DIR=$(realpath "$SCRIPT_DIR/..")

DOWNLOAD_FOLDER="$SRC_DIR/downloads"

EXEC_DIR="${1:-./build}"

DEBUG_FLAG="${2:-}"

DO_DELETE_FILE="${3:-}"

MEMORY_SIZE="${4:-16M}"

THREAD_MULTIPLIER="${5:-14}"

DEBUG_CONTENT=()

if [ -n "$DEBUG_FLAG" ]; then
    DEBUG_CONTENT=("--verbose")
fi

function download() {

    NAME="$1"

    mkdir -p "$DOWNLOAD_FOLDER"

    DOWNLOAD_NAME="$DOWNLOAD_FOLDER/${NAME}.tsv"
    DOWNLOAD_NAME_GZ="${DOWNLOAD_NAME}.gz"

    if [ ! -e "$DOWNLOAD_NAME" ]; then
        wget "https://datasets.imdbws.com/${NAME}.tsv.gz" -O "$DOWNLOAD_NAME_GZ" -q
        gzip -d "$DOWNLOAD_NAME_GZ"

        if [ "$NAME" = "title.akas" ]; then

            TEMP_NAME="/tmp/temp.tsv"

            cat "$DOWNLOAD_NAME" | tr -s '\2' ',' >"$TEMP_NAME"
            rm "$DOWNLOAD_NAME"
            mv "$TEMP_NAME" "$DOWNLOAD_NAME"
        fi

    fi

}

function import() {
    NAME="$1"

    "${EXEC_DIR}/imdb-sql-importer" \
        -f "downloads/${NAME}.tsv" \
        --user "$PG_USER" \
        --password "$PG_PASSWORD" \
        --port "$PG_PORT" \
        --host "$PG_HOST" \
        --has-head \
        "${DEBUG_CONTENT[@]}" \
        --ignore-errors \
        --memory-size "$MEMORY_SIZE" \
        --transaction-size 0 \
        --threads "$(($(nproc) * "$THREAD_MULTIPLIER"))"
}

function delete() {
    if [ -n "$DO_DELETE_FILE" ]; then

        if [ -e "$DOWNLOAD_FOLDER" ]; then
            rm -rf "$DOWNLOAD_FOLDER"
        fi

    fi
}

function process() {
    NAME="$1"

    download "$NAME"

    import "$NAME"

    delete "$NAME"

}

function process_all() {

    mkdir -p "$DOWNLOAD_FOLDER"

    process "title.basics"
    process "name.basics"
    process "title.akas"
    process "title.crew"
    process "title.episode"
    process "title.principals"
    process "title.ratings"

}

process_all
