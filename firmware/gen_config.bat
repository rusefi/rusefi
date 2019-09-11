@echo off
rem This batch files reads rusefi_config.txt and produses firmware persistent configuration headers
rem the storage section of rusefi.ini is updated as well

rm gen_config.log
rm gen_config_board.log

rem lazy is broken - TS input is not considered a change
rm build/config.gen

mkdir build

java -DSystemOut.name=gen_config ^
 -Drusefi.generator.lazyfile.enabled=true ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration\rusefi_config.txt ^
 -romraider integration ^
 -ts_destination tunerstudio ^
 -with_c_defines false ^
 -initialize_to_zero false ^
 -c_defines        controllers\generated\rusefi_generated.h ^
 -c_destination    controllers\generated\engine_configuration_generated_structures.h ^
 -c_fsio_constants controllers\generated\fsio_enums_generated.def ^
 -c_fsio_getters   controllers\generated\fsio_getters.def ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/Fields.java ^
 -romraider_destination ../java_console/rusefi.xml ^
 -skip build/config.gen
IF NOT ERRORLEVEL 0 echo ERROR generating
IF NOT ERRORLEVEL 0 EXIT /B 1


rem This would automatically copy latest file to 'dev' TS project
set ts_path="%HOMEDRIVE%%HOMEPATH%\Documents\TunerStudioProjects"
echo %ts_path%
cp tunerstudio/rusefi.ini %ts_path%\dev\projectCfg\mainController.ini
cp tunerstudio/rusefi_microrusefi.ini %ts_path%\dev_mre\projectCfg\mainController.ini

call gen_config_board microrusefi
IF NOT ERRORLEVEL 0 echo ERROR generating
IF NOT ERRORLEVEL 0 EXIT /B 1

call gen_config_board frankenso
IF NOT ERRORLEVEL 0 echo ERROR generating
IF NOT ERRORLEVEL 0 EXIT /B 1

call gen_config_board prometheus
IF NOT ERRORLEVEL 0 echo ERROR generating
IF NOT ERRORLEVEL 0 EXIT /B 1

