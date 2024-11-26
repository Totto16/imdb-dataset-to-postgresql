#!/usr/bin/env bash

set -ex

set -o pipefail

DEBUG_FLAG="${1:-}"

mkdir -p "$PGDATA"
chmod 00700 "$PGDATA"

service postgresql start

PG_MAJOR="16"
PG_BIN_DIR="/usr/lib/postgresql/$PG_MAJOR/bin"
export PATH="$PATH:$PG_BIN_DIR"

function sql_command() {
    su postgres -c "\"$PG_BIN_DIR/psql\" -h \"$PG_HOST\" -p \"$PG_PORT\" postgres -c \"$1\""
}

function sql_command_imdb() {
    su imdb -c "$1"
}
# create user and database
sql_command "CREATE DATABASE $PG_DBNAME;"
sql_command "CREATE USER $PG_USER WITH ENCRYPTED PASSWORD '$PG_PASSWORD';"
sql_command "GRANT ALL PRIVILEGES ON DATABASE $PG_DBNAME TO $PG_USER;"
sql_command "GRANT ALL ON SCHEMA public TO $PG_USER;"
sql_command "ALTER DATABASE $PG_DBNAME OWNER TO $PG_USER;"
sql_command "GRANT USAGE ON SCHEMA public TO $PG_USER;"

service postgresql restart

#add imdb user
useradd -ms /bin/bash imdb

# import schema
sql_command_imdb "PGPASSWORD=\"$PG_PASSWORD\" \"$PG_BIN_DIR/psql\" -h \"$PG_HOST\" -p \"$PG_PORT\" -U \"$PG_USER\" -f \"$(pwd)/scripts/imdb_schema.sql\" \"$PG_DBNAME\""

ulimit -l unlimited unlimited

sql_command_imdb "\"$(pwd)/scripts/process_all.sh\" \"/tmp\" \"$DEBUG_FLAG\""

sql_command_imdb "\"$(pwd)/scripts/dump_all.sh\" \"/tmp/result.sql.gz\" \"$DEBUG_FLAG\""

service postgresql stop
