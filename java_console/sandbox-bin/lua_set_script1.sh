#!/bin/bash
#
# lua_set_script1.sh
# Builds the rusEFI java console and applies sandbox_script1.lua to the connected ECU.
#
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

cd "$REPO_ROOT"
./gradlew :ui:shadowJar

java -jar console/rusefi_console.jar set_lua "$SCRIPT_DIR/sandbox_script1.lua"
