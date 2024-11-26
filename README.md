# A IMDB Dataset Parser in C++

## What is this?

It parses files from https://developer.imdb.com/non-commercial-datasets/ in C++ and saves it to a postgres Database

Inspired by https://github.com/Totto16/imdb-dataset / https://github.com/Totto16/imdb-dataset-parser/ / https://github.com/andreivinaga/imdb-dataset

This [preseeds](https://cadu.dev/creating-a-docker-image-with-database-preloaded/) a postgresql database with the whole data to a docker image, that can be used easily.

## How to use

Use the docker images on GitHub as base database image, e.g [latest](ghcr.io/totto16/preseeded-imdb-database:latest) or also specific dates [20241126](ghcr.io/totto16/preseeded-imdb-database:20241126). Use it like a normal `postgres:16-alpine` docker image, and you have the table `imdb`, which already has the data.
