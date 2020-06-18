echo java version
java -version

echo Building java console
pwd
cd java_console
call ant clean clean_out_folder jar
cd ..

echo Building TS plugin
cd java_tools/ts_plugin_launcher
call ant clean jar
cd ../..

if not exist java_console_binary/rusefi_console.jar echo CONSOLE COMPILATION FAILED
echo java console looks good
