#!/bin/bash
# file gen_config_default.sh

# TODO: there is a chance that we can simply remove this file completely? let's try once we fix https://github.com/rusefi/rusefi/issues/7604

cd ../java_tools
./gradlew :config_definition:shadowJar
cd ../firmware

bash gen_signature.sh f407-discovery
# todo: eliminate that legacy `rusefi.ini'!
bash gen_config_board.sh config/boards/f407-discovery f407-discovery rusefi.ini
bash bin/gen_image_board.sh config/boards/f407-discovery f407-discovery rusefi.ini
exit $?
