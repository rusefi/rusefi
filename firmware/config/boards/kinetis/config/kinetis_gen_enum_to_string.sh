#!/bin/bash

cd ../../../..

java -DSystemOut.name=logs/gen_enum_to_string_kinetis \
 -jar ../java_tools/enum2string.jar \
 -outputPath config/boards/kinetis/config/controllers/algo \
 -enumInputFile config/boards/kinetis/rusefi_hw_enums.h



