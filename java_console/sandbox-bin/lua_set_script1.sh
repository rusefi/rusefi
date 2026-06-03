#!/bin/bash
#
# lua_set_script1.sh
# Builds the rusEFI java console and applies sandbox_script1.lua to the connected ECU.
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
LUA_FILE="$SCRIPT_DIR/sandbox_script1.lua"
JAR="$REPO_ROOT/console/rusefi_console.jar"

echo "[lua_set_script1] SCRIPT_DIR=$SCRIPT_DIR"
echo "[lua_set_script1] REPO_ROOT=$REPO_ROOT"
echo "[lua_set_script1] LUA_FILE=$LUA_FILE"
echo "[lua_set_script1] JAR=$JAR"

if [[ ! -f "$LUA_FILE" ]]; then
  echo "[lua_set_script1] ERROR: lua file not found at $LUA_FILE" >&2
  exit 10
fi

cd "$REPO_ROOT"
echo "[lua_set_script1] running ./gradlew :ui:shadowJar ..."
./gradlew :ui:shadowJar

if [[ ! -f "$JAR" ]]; then
  echo "[lua_set_script1] ERROR: console jar not found at $JAR after build" >&2
  exit 11
fi

echo "[lua_set_script1] invoking: java -jar $JAR set_lua $LUA_FILE"
java -jar "$JAR" set_lua "$LUA_FILE"
echo "[lua_set_script1] done"
