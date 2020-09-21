@echo off

rem This batch files reads rusefi_enums.h and produses auto_generated_enums.* files

cd ../../../../..

java -jar ../java_tools/enum2string.jar ^
 -inputPath . ^
 -outputPath config/boards/hellen/cypress/config/controllers/algo ^
 -enumInputFile controllers/algo/rusefi_enums.h ^
 -enumInputFile config/boards/hellen/cypress/rusefi_hw_enums.h ^

