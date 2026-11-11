#!/usr/bin/env bash
set -euo pipefail

if [[ $# != 1 || ! -f "$1/launch4j.jar" ]]; then
  echo "Usage: $0 /path/to/Launch4j-3.50" >&2
  exit 2
fi

launch4j_home=$(cd "$1" && pwd)
version=$(java -jar "$launch4j_home/launch4j.jar" --help 2>&1 || true)
if [[ $version != *'Launch4j 3.50 '* ]]; then
  echo "Expected Launch4j 3.50 in $launch4j_home" >&2
  exit 1
fi

cd "$(dirname "$0")"
java -jar "$launch4j_home/launch4j.jar" rusefi_updater.xml
