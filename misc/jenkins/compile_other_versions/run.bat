
set FTP_SERVER=home451478433.1and1-data.host

pwd

call misc\jenkins\build_java_console.bat
if not exist java_console_binary/rusefi_console.jar exit -1



cd firmware
call clean.bat
cd ..
cho Compiling F746
cd firmware\config\boards\nucleo_f746
call !compile-nucleo_f746.bat
cd ..
pwd
set bundle_name=stm32f746_nucleo
call misc\jenkins\compile_other_versions\build_version.bat
echo Done with F746-nucleo


cd firmware
call clean.bat
cd ..
echo Compiling F767
cd firmware\config\boards\nucleo_f767
call !compile-nucleo_f767.bat
cd ..
pwd
set bundle_name=stm32f767_nucleo
call misc\jenkins\compile_other_versions\build_version.bat
echo Done with F767-nucleo

cd firmware
call clean.bat
cd ..
echo Compiling F767
cd firmware\config\boards\nucleo_f767
call !compile-f767.bat
cd ..
pwd
set bundle_name=stm32f767_osc
call misc\jenkins\compile_other_versions\build_version.bat
echo Done with F767-osc



cd firmware
call clean.bat
cd ..
echo Compiling prometheus_405
cd firmware\config\boards\prometheus
call !compile-prometheus-405.bat
cd ..
pwd
set bundle_name=prometheus_405
call misc\jenkins\compile_other_versions\build_version.bat
echo Done with prometheus_405


cd firmware
call clean.bat
cd ..
echo Compiling prometheus_469
cd firmware\config\boards\prometheus
call !compile-prometheus-469.bat
cd ..
pwd
set bundle_name=prometheus_469
call misc\jenkins\compile_other_versions\build_version.bat
echo Done with prometheus_469


