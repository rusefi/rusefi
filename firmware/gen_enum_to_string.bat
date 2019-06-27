rem This batch files reads rusefi_enums.h and produses auto_generated_enums.* files

rm gen_enum_to_string.log

java -DSystemOut.name=gen_enum_to_string ^
 -jar ../java_tools/enum2string.jar ^
 -inputPath . ^
 -outputPath controllers/algo ^
 -enumInputFile controllers/algo/rusefi_enums.h ^
 -enumInputFile controllers/algo/rusefi_hw_enums.h ^

