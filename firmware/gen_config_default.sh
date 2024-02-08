#!/bin/bash
# file gen_config_default.sh

cd ../java_tools
./gradlew :config_definition:shadowJar
cd ../firmware

export BOARD_DIR="config/boards/f407-discovery"
export SHORT_BOARD_NAME="f407-discovery"
bash gen_config_board.sh
exit $?
