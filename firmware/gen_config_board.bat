@echo off

rem This batch files reads rusefi_config.txt and produses firmware persistent configuration headers
rem the storage section of rusefi.ini is updated as well

set BOARDNAME=%1

if %BOARDNAME%.==. (
    echo Board name parameter expected
    exit -1
) 

echo BOARDNAME=%BOARDNAME%


rem lazy is broken - TS input is not considered a change
rm build/config.gen

java -cp ../java_tools/ConfigDefinition.jar;../java_tools/configuration_definition/lib/snakeyaml.jar ^
 com.rusefi.board_generator.BoardReader ^
 -board %BOARDNAME% ^
 -firmware_path . ^
 -out tunerstudio ^
 -enumInputFile controllers/algo/rusefi_enums.h ^
 -enumInputFile controllers/algo/rusefi_hw_enums.h


java -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration\rusefi_config.txt ^
 -ts_destination tunerstudio ^
 -ts_output_name rusefi_%BOARDNAME%.ini ^
 -prepend tunerstudio/%BOARDNAME%_prefix.txt ^
 -prepend config/boards/%BOARDNAME%/prepend.txt ^
 -skip build/config.gen


rem This would automatically copy latest file to 'dev' TS project
set ts_path="%HOMEDRIVE%%HOMEPATH%\Documents\TunerStudioProjects\dev\projectCfg"
rem cp tunerstudio/rusefi_microrusefi.ini %ts_path%\mainController.ini