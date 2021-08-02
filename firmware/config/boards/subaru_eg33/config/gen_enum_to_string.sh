#!/bin/sh

echo This batch files reads rusefi_enums.h and produces auto_generated_enums.* files

#cd ../../../..
#cd ..

BOARD=subaru_eg33

java -DSystemOut.name=gen_enum_to_string \
    -jar ../java_tools/enum2string.jar \
    -inputPath . \
    -outputPath config/boards/${BOARD}/config/controllers/algo \
    -enumInputFile config/boards/${BOARD}/rusefi_hw_enums.h
