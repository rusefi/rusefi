echo "TIMESTAMP %date% %time%"
set script_name=hw_test.bat
echo Entering %script_name%
pwd

cd firmware
rem Using magic 'cd' system variable here
set "cur_folder=%cd%"
echo "%script_name%: erasing first"
call flash_erase407.bat
cd %cur_folder%
pwd
rem there is some instability with failed flash sometimes, maybe sleep would help?
sleep 3
echo "%script_name%: trying to flash"
rem This script depends on someone else building firmware
call flash_openocd407.bat
IF NOT ERRORLEVEL 0 echo ERROR invoking flash_openocd407.bat
IF NOT ERRORLEVEL 0 EXIT /B 1

cd %cur_folder%

if not exist build/rusefi.bin echo FIRMWARE NOT FOUND
if not exist build/rusefi.bin exit -1

pwd

cd ..

rem echo Running some commands
rem pwd
rem java -cp java_console_binary\rusefi_console.jar com.rusefi.CmdLine "set_led_blinking_period 10"

cd java_console

echo Running tests
which java
which javac
ant realtest

IF NOT ERRORLEVEL 0 echo ERROR DETECTED
IF NOT ERRORLEVEL 0 EXIT /B 1

echo "TIMESTAMP %date% %time%"
pwd
echo "exiting %script_name%"
