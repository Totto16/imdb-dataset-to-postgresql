#!/usr/bin/env bash

set -ex

DEST_FILE="${1:-$(pwd)/backup.sql.gz}"

PGPASSWORD="$PG_PASSWORD" pg_dump \
    --username="$PG_USER" \
    --port="$PG_PORT" \
    --host="$PG_HOST" \
    --dbname="$PG_DBNAME" \
    --format="plain" \
    --file="$DEST_FILE" \
    --verbose \
    --no-owner \
    --compress=gzip:5
