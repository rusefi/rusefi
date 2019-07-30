@echo off

rem This batch files reads rusefi_enums.h and produses auto_generated_enums.* files

cd ../../../..

java -jar ../java_tools/enum2string.jar ^
 -inputPath . ^
 -outputPath config/boards/kinetis/config/controllers/algo ^
 -enumInputFile controllers/algo/rusefi_enums.h ^
 -enumInputFile config/boards/kinetis/rusefi_hw_enums.h ^

