#!/bin/bash
# file gen_config_default.sh

cd ../java_tools
./gradlew :config_definition:shadowJar
cd ../firmware

bash gen_signature.sh f407-discovery
bash gen_config_board.sh config/boards/f407-discovery f407-discovery evotech.ini
bash bin/gen_image_board.sh config/boards/f407-discovery f407-discovery evotech.ini
exit $?
