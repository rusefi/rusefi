@echo off

rem This batch files reads rusefi_config.txt and produses firmware persistent configuration headers
rem the storage section of rusefi.ini is updated as well

cd ../../../..

java -cp ../java_tools/ConfigDefinition.jar;../java_tools/configuration_definition/lib/snakeyaml.jar ^
 com.rusefi.board_generator.BoardReader ^
 -board kinetis ^
 -firmware_path . ^
 -out config/boards/kinetis/config/tunerstudio ^
 -enumInputFile controllers/algo/rusefi_enums.h ^
 -enumInputFile config/boards/kinetis/rusefi_hw_enums.h

java -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/rusefi_config.txt ^
 -ts_destination tunerstudio ^
 -ts_output_name rusefi_kinetis.ini ^
 -c_defines config/boards/kinetis/config/controllers/algo/rusefi_generated.h ^
 -c_destination config/boards/kinetis/config/controllers/algo/engine_configuration_generated_structures.h ^
 -prepend config/boards/kinetis/config/rusefi_config_kinetis.txt ^
 -prepend config/boards/kinetis/config/tunerstudio/kinetis_prefix.txt ^
 -skip build_kinetis/config.gen

