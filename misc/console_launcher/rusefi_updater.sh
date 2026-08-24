#!/bin/sh
# Linux/macOS counterpart of rusefi_updater.exe: chdir into the bundle's console folder
# (rusefi_updater.xml does the same via launch4j <chdir>) and start the console.
# Arguments are forwarded so that Autoupdate#startConsoleAsANewProcess can pass them on.

cd "$(dirname "$0")/console" || exit 1
exec java -jar ./rusefi_console.jar "$@"
