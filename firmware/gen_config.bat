@echo off
rem This batch files reads rusefi_config.txt and produses firmware persistent configuration headers
rem the storage section of rusefi.ini is updated as well


rem lazy is broken - TS input is not considered a change
rm build/config.gen

mkdir build

java -Drusefi.generator.lazyfile.enabled=true ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration\rusefi_config.txt ^
 -romraider integration ^
 -ts_destination tunerstudio ^
 -c_defines     controllers\generated\rusefi_generated.h ^
 -c_destination controllers\generated\engine_configuration_generated_structures.h ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/Fields.java ^
 -romraider_destination ../java_console/rusefi.xml ^
 -skip build/config.gen
IF NOT ERRORLEVEL 0 echo ERROR generating
IF NOT ERRORLEVEL 0 EXIT /B 1


rem This would automatically copy latest file to 'dev' TS project
set ts_path="%HOMEDRIVE%%HOMEPATH%\Documents\TunerStudioProjects\dev\projectCfg"
echo %ts_path%
cp tunerstudio/rusefi.ini %ts_path%\mainController.ini

call gen_config_board microrusefi
IF NOT ERRORLEVEL 0 echo ERROR generating
IF NOT ERRORLEVEL 0 EXIT /B 1

call gen_config_board frankenso
IF NOT ERRORLEVEL 0 echo ERROR generating
IF NOT ERRORLEVEL 0 EXIT /B 1

call gen_config_board prometheus
IF NOT ERRORLEVEL 0 echo ERROR generating
IF NOT ERRORLEVEL 0 EXIT /B 1

