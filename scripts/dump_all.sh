#!/usr/bin/env bash

set -ex

DEST_FILE="${1:-$(pwd)/backup.sql.gz}"

DEBUG_FLAG="${2:-}"

DEBUG_CONTENT=()

if [ -n "$DEBUG_FLAG" ]; then
    DEBUG_CONTENT=("--verbose")
fi

PGPASSWORD="$PG_PASSWORD" pg_dump \
    --username="$PG_USER" \
    --port="$PG_PORT" \
    --host="$PG_HOST" \
    --dbname="$PG_DBNAME" \
    --format="plain" \
    --file="$DEST_FILE" \
    "${DEBUG_CONTENT[@]}" \
    --no-owner \
    --compress=gzip:5
