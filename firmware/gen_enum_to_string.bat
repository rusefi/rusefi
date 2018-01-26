rem This batch files reads rusefi_enums.h and produses auto_generated_enums.* files

java -jar ../java_tools/enum2string.jar . controllers/algo
