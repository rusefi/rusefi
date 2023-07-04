#!/bin/bash

# fail on error!
set -e

cd ../java_tools
./gradlew :ui:shadowJar
cd ../firmware

java -jar ../java_console_binary/rusefi_console.jar reboot_dfu