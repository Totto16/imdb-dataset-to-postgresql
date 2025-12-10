#!/usr/bin/env bash

set -e

USER="imdb"
DBNAME="imdb"
PASSWORD="YTJlMGYyOWJlMjRiMTM0YmY4NjI5N2Y4"
PORT="5555"
HOST="localhost"

function import() {
    NAME="$1"

    ./build/imdb-sql-importer \
        -f "downloads/${NAME}.tsv" \
        --user "$USER" \
        --dbname "$DBNAME" \
        --password "$PASSWORD" \
        --port "$PORT" \
        --host "$HOST" \
        --has-head \
        --verbose \
        --ignore-errors \
        --memory-size 16M \
        --transaction-size 0 \
        --threads "$(($(nproc) * 14))"
}

function import_all() {

    import "title.basics"
    import "name.basics"
    import "title.akas" # TODO: preprocess, replace \2 by ,
    # cat downloads/title.akas.tsv | tr -s '\2' ',' > downloads/title.akas.tsv2     # and rwlace old one
    import "title.crew"
    import "title.episode"
    import "title.principals"
    import "title.ratings"

}

import_all


# tt10268556
# tt10671806
# tt10730420
# tt11421010
# tt11685972
# tt11824994
# tt12089310
# tt12463990
# tt12630054
# tt13680870
# tt14875768
# tt21043750
# tt22051626
# tt22051772
# tt22814400
# tt23619910
# tt25009634
# tt25009636
# tt25009642
# tt25009644
# tt25009646
# tt25009652
# tt25009654
# tt25009658
# tt25009660
# tt25009662
# tt25009664
# tt25009666
# tt25009668
# tt25009670
# ...

# Successfully inserted 50586004 rows
# It took 59 minutes 10 seconds 678 milliseconds
# Ignored 455 Errors

# real	59m16,409s
# user	11m9,679s
# sys	13m24,402s
