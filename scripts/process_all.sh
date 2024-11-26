#!/usr/bin/env bash

set -ex

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

SRC_DIR=$(realpath "$SCRIPT_DIR/..")

DOWNLOAD_FOLDER="$SRC_DIR/downloads"

EXEC_DIR="${1:-./build}"

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
        --verbose \
        --ignore-errors \
        --memory-size 16M \
        --transaction-size 0 \
        --threads "$(($(nproc) * 14))"
}

function process() {
    NAME="$1"

    download "$NAME"

    import "$NAME"

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
