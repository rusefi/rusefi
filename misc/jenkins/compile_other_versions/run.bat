
set FTP_SERVER=home451478433.1and1-data.host

pwd

call misc\jenkins\build_java_console.bat
if not exist java_console_binary/rusefi_console.jar exit -1



cd firmware
call clean.bat
cd ..
cho Compiling F746
cd firmware\config\boards\NUCLEO_F746
call !compile-nucleo_f746.bat
cd ..
pwd
set stm_arch=stm32f746
call misc\jenkins\compile_other_versions\build_version.bat
echo Done with F746


cd firmware
call clean.bat
cd ..
echo Compiling F767
cd firmware\config\boards\NUCLEO_F767
call !compile-nucleo_f767.bat
cd ..
pwd
set stm_arch=stm32f767
call misc\jenkins\compile_other_versions\build_version.bat

echo Done with F767
