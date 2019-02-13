java -version

echo Building java console
pwd
cd java_console
call ant clean clean_out_folder build
cd ..

if not exist java_console_binary/rusefi_console.jar echo CONSOLE COMPILATION FAILED
echo java console looks good
