#!/bin/bash
# file gen_config_default.sh

cd ../java_tools
./gradlew :config_definition:shadowJar
cd ../firmware

bash gen_signature.sh f407-discovery
# todo: eliminate that legacy `rusefi.ini'!
bash gen_config_board.sh config/boards/f407-discovery f407-discovery rusefi.ini
bash bin/gen_image_board.sh config/boards/f407-discovery f407-discovery rusefi.ini
exit $?
