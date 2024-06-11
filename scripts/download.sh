#!/usr/bin/env bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

SRC_DIR=$(realpath "$SCRIPT_DIR/..")

DOWNLOAD_FOLDER="$SRC_DIR/downloads"

function download() {

    NAME="$1"

    mkdir -p "$DOWNLOAD_FOLDER"

    DOWNLOAD_NAME="$DOWNLOAD_FOLDER/${NAME}.tsv"
    DOWNLOAD_NAME_GZ="${DOWNLOAD_NAME}.gz"

    if [ ! -e "$DOWNLOAD_NAME" ]; then
        wget "https://datasets.imdbws.com/${NAME}.tsv.gz" -O "$DOWNLOAD_NAME_GZ"
        gzip -d "$DOWNLOAD_NAME_GZ"
    fi

}

function download_all() {

    mkdir -p "$DOWNLOAD_FOLDER"

    download "title.basics"
    download "name.basics"
    download "title.akas" # TODO: preprocess, replace \2 by ,
    download "title.crew"
    download "title.episode"
    download "title.principals"
    download "title.ratings"

}

download_all
