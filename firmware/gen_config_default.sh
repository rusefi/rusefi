#!/bin/bash
# file gen_config_default.sh

cd ../java_tools
./gradlew :config_definition:shadowJar
cd ../firmware

bash gen_config_board.sh config/boards/f407-discovery f407-discovery rusefi.ini
exit $?
