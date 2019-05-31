rem This batch files reads rusefi_config.txt and produses firmware persistent configuration headers
rem the storage section of rusefi.ini is updated as well


rem lazy is broken - TS input is not considered a change
rm build/config.gen

java -jar ../java_tools/ConfigDefinition.jar -definition integration\rusefi_config.txt -romraider integration -ts_destination tunerstudio -c_defines controllers\algo\rusefi_generated.h -c_destination controllers\algo\engine_configuration_generated_structures.h -java_destination ../java_console -skip build/config.gen


rem This would automatically copy latest file to 'dev' TS project
set ts_path="%HOMEDRIVE%%HOMEPATH%\Documents\TunerStudioProjects\dev\projectCfg"
echo %ts_path%
cp tunerstudio/rusefi.ini %ts_path%\mainController.ini

call gen_config_board microrusefi
call gen_config_board frankenso
call gen_config_board prometheus
