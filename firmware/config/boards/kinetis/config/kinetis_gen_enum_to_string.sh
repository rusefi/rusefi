#!/bin/bash

cd ../../../..

java -DSystemOut.name=gen_enum_to_string_kinetis \
 -jar ../java_tools/enum2string.jar \
 -outputPath config/boards/kinetis/config/controllers/algo \
 -enumInputFile controllers/algo/rusefi_enums.h \
 -enumInputFile config/boards/kinetis/rusefi_hw_enums.h



