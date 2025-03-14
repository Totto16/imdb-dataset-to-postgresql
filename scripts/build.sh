#!/usr/bin/env bash

set -e

IMAGE_NAME="imdb/preseeded-database"

REGISTRY_SELFHOSTED="registry.totto.lt"
REMOTE_NAME_SELFHOSTED="$REGISTRY_SELFHOSTED/$IMAGE_NAME"

REMOTE_NAME_GITHUB="ghcr.io/totto16/preseeded-imdb-database"

DOCKERFILE="./scripts/Dockerfile"

LIMIT_ARGS=("--ulimit" "memlock=-1:-1")

function push_tag() {

    VERSION="$1"
    PUSH_VERSION="$2"

    docker image tag "$IMAGE_NAME:$VERSION" "$REMOTE_NAME_SELFHOSTED:$PUSH_VERSION"
    docker push "$REMOTE_NAME_SELFHOSTED:$PUSH_VERSION"

    docker image tag "$IMAGE_NAME:$VERSION" "$REMOTE_NAME_GITHUB:$PUSH_VERSION"
    docker push "$REMOTE_NAME_GITHUB:$PUSH_VERSION"

}

CURRENT_DATE="$(date -u +"%Y%m%d")"

function build_image_for_current_platform() {

    docker build "${LIMIT_ARGS[@]}" -t "$IMAGE_NAME:$CURRENT_DATE" . -f "$DOCKERFILE"

    push_tag "$CURRENT_DATE" latest
    push_tag "$CURRENT_DATE" "$CURRENT_DATE"

}

function build_image_for_multiple_platforms() {

    docker buildx build --platform 'linux/amd64,linux/arm64/v8' \
        "${LIMIT_ARGS[@]}" \
        -t "$REMOTE_NAME_SELFHOSTED:latest" \
        -t "$REMOTE_NAME_SELFHOSTED:$CURRENT_DATE" \
        -t "$REMOTE_NAME_GITHUB:latest" \
        -t "$REMOTE_NAME_GITHUB:$CURRENT_DATE" \
        . -f "$DOCKERFILE" --push

}

function get_type() {

    if ! command -v docker >/dev/null 2>&1; then
        echo "Docker is not installed" >&2
        exit 1
    elif docker buildx version >/dev/null 2>&1; then
        echo "buildx"
    else
        echo "normal"
    fi

}

function print_usage() {
    echo "usage: $0 [type]" >&2
    echo -e "\ttype: 'auto', 'normal', 'buildx' default: 'auto'" >&2
    exit 1
}

function build_image() {

    TYPE="auto"

    if [ $# -eq 1 ]; then
        TYPE="$1"
    fi

    if [ "$TYPE" == "auto" ]; then
        TYPE="$(get_type)"
        echo "auto resolved to type: '$TYPE'"
    fi

    case "$TYPE" in

    "normal")
        build_image_for_current_platform
        ;;
    "buildx")
        # note: if this fails, read https://www.docker.com/blog/how-to-rapidly-build-multi-architecture-images-with-buildx/
        build_image_for_multiple_platforms
        ;;
    *)
        echo "Error: unsupported type: '$TYPE'" >&2
        print_usage
        ;;

    esac

}

if [ $# -gt 1 ]; then
    print_usage
fi

build_image "$@"
