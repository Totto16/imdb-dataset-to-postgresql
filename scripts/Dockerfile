FROM ubuntu:24.10 AS cpp-buildenv

ARG DEBUG=

WORKDIR /tmp

RUN apt-get update && \
    apt-get install wget \
    ninja-build \
    python3-pip \
    lsb-release \
    wget \
    software-properties-common \
    gnupg \
    cmake \
    git \
    pkg-config \
    postgresql-common \
    -y --no-install-recommends && \
    wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh 20 && \
    /usr/share/postgresql-common/pgdg/apt.postgresql.org.sh -y && \
    apt-get install postgresql-18 \
    libpq-dev \
    -y --no-install-recommends && \
    pip install meson --break-system-packages

COPY . /tmp/


ENV CC=clang-20
ENV CXX=clang++-20
ENV LD=clang-20


RUN meson setup build \
    -Dbuildtype=release \
    -Ddefault_library=static \
    -Dpostgres-cxx-client:postgres_version_range=">=18" && \
    meson compile -C build


FROM ubuntu:24.10 AS import-stage

WORKDIR /tmp

COPY scripts/ /tmp/scripts/

COPY --from=cpp-buildenv /tmp/build/imdb-sql-importer /tmp/

RUN apt-get update && \
    apt-get install postgresql-common \
    wget \
    unzip \
    ca-certificates \
    -y --no-install-recommends && \
    /usr/share/postgresql-common/pgdg/apt.postgresql.org.sh -y && \
    apt-get install postgresql-18 \
    -y --no-install-recommends

ENV PG_USER="imdb"
ENV PG_PASSWORD="ODFiOWZkMDI0NTY4NWM5MzQzNjcyY2Jm"
ENV PG_PORT="5432"
ENV PG_HOST="/var/run/postgresql"
ENV PG_DBNAME="imdb"

ENV PGDATA="/data/"

RUN ./scripts/dump_for_docker.sh "$DEBUG" "18"

# inspiration: https://cadu.dev/creating-a-docker-image-with-database-preloaded/
FROM postgres:18-alpine AS dump-importer


COPY --from=import-stage /tmp/result.sql.gz /docker-entrypoint-initdb.d/

RUN ["sed", "-i", "s/exec \"$@\"/echo \"skipping...\"/", "/usr/local/bin/docker-entrypoint.sh"]

ENV POSTGRES_USER=postgres
ENV POSTGRES_PASSWORD=postgres
ENV PGDATA=/data

RUN ["/usr/local/bin/docker-entrypoint.sh", "postgres"]

# final build stage
FROM postgres:18-alpine

COPY --from=dump-importer /data $PGDATA
