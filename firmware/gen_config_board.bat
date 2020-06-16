@echo off

rem This batch files reads rusefi_config.txt and produses firmware persistent configuration headers
rem the storage section of rusefi.ini is updated as well

set BOARDNAME=%1

if %BOARDNAME%.==. (
    echo Board name parameter expected
    exit -1
) 

echo BOARDNAME=%BOARDNAME%

java -DSystemOut.name=gen_config_board ^
 -cp ../java_tools/ConfigDefinition.jar ^
 com.rusefi.board_generator.BoardReader ^
 -board %BOARDNAME% ^
 -firmware_path . ^
 -out tunerstudio ^
 -enumInputFile controllers/algo/rusefi_enums.h ^
 -enumInputFile controllers/algo/rusefi_hw_enums.h


java -DSystemOut.name=gen_config_board ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration\rusefi_config.txt ^
 -tool gen_config.bat ^
 -ts_destination tunerstudio ^
 -firing_order controllers/algo/firing_order.h ^
 -ts_output_name rusefi_%BOARDNAME%.ini ^
 -prepend tunerstudio/%BOARDNAME%_prefix.txt ^
 -prepend config/boards/%BOARDNAME%/prepend.txt

