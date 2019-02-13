
set FTP_SERVER=home451478433.1and1-data.host

pwd

call misc\jenkins\build_java_console.bat
if not exist java_console_binary/rusefi_console.jar exit -1



cd firmware
make clean
cd ..

echo Compiling F767
cd firmware\config\boards\NUCLEO_F746
call !compile-nucleo_f746.bat
pwd


echo OK for now

