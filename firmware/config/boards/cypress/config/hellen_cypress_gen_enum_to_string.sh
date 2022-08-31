#!/bin/bash

# This batch files reads rusefi_hw_enums.h and produces auto_generated_enums.* files

cd ../../../../..

java -DSystemOut.name=logs/gen_enum_to_string_hellen_cypress \
 -jar ../java_tools/enum2string.jar \
 -outputPath config/boards/сypress/config/controllers/algo \
 -enumInputFile config/boards/cypress/rusefi_hw_enums.h

