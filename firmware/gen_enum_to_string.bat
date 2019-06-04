rem This batch files reads rusefi_enums.h and produses auto_generated_enums.* files

java -jar ../java_tools/enum2string.jar -inputPath . -outputPath controllers/algo -enumInputFile controllers/algo/rusefi_enums.h -enumInputFile controllers/algo/rusefi_hw_enums.h
