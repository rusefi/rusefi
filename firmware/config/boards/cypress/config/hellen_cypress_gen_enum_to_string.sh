#!/bin/bash

# This batch files reads rusefi_hw_enums.h and produces auto_generated_enums.* files

cd ../../../..

java -DSystemOut.name=logs/gen_enum_to_string_hellen_cypress \
 -jar ../java_tools/enum_to_string/build/libs/enum_to_string-all.jar \
 -outputPath config/boards/cypress/config/controllers/algo \
 -enumInputFile config/boards/cypress/rusefi_hw_enums.h

