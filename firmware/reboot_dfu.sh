#!/bin/bash

# fail on error!
set -e

cd ../
./gradlew :ui:shadowJar
cd firmware

java -jar ../console/rusefi_console.jar reboot_dfu
