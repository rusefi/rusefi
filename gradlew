#!/bin/sh
set -eu
repo_root=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
exec "$repo_root/java_tools/gradlew" -p "$repo_root" "$@"
