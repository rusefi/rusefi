rem This batch files reads rusefi_config.txt and produses firmware persistent configuration headers
rem the storage section of rusefi.ini is updated as well

java -jar ../java_tools/ConfigDefinition.jar -definition integration\rusefi_config.txt -romraider integration -ts_destination tunerstudio -c_destination controllers\algo -java_destination ../java_console -skip build/config.gen


rem This would automatically copy latest file to 'dev' TS project
set ts_path="%HOMEDRIVE%%HOMEPATH%\Documents\TunerStudioProjects\dev\projectCfg"
echo %ts_path%
cp tunerstudio/rusefi.ini %ts_path%\mainController.ini