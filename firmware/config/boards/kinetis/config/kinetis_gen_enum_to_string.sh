#!/bin/bash

cd ../../../..

# todo: CUSTOM_ENUM_OUTPUT=config/boards/kinetis/config/controllers/algo?

java -DSystemOut.name=logs/gen_enum_to_string_kinetis \
 -jar ../java_tools/enum_to_string/build/libs/enum_to_string-all.jar \
 -outputPath config/boards/kinetis/config/controllers/algo \
 -enumInputFile config/boards/kinetis/rusefi_hw_enums.h



