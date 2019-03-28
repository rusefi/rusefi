rem
rem This script would compile firmware, dev console and win32 simulator into a single bundle file
rem This script depends on Cygwin tools: zip
rem

set FTP_SERVER=home451478433.1and1-data.host

echo build_current_bundle.bat: Hello rusEfi build full bundle

echo %date% %time%


cd firmware/bootloader
call !clean_bootloader.bat
call !compile_bootloader_discovery407.bat
if not exist bootloader_generated.hxx echo FAILED TO COMPILE BOOTLOADER
if not exist bootloader_generated.hxx exit -1
pwd
cd ../..

cd firmware
echo %date% %time%


echo build_current_bundle.bat: will be Erasing chip
if not exist flash_erase407.bat echo NOT FOUND flash_erase.bat
if not exist flash_erase407.bat exit -1
echo build_current_bundle.bat: Erasing chip
pwd
rem Using magic 'cd' system variable here
set "cur_folder=%cd%"
call flash_erase407.bat
cd %cur_folder%
pwd


echo build_current_bundle.bat: Building firmware
call clean.bat
git submodule update --init

call update_version.bat

call clean_compile_two_versions.bat
if not exist deliver/rusefi_release.hex echo FAILED RELEASE
if not exist deliver/rusefi_release.hex exit -1

if not exist deliver/rusefi_debug.hex echo FAILED DEBUG
if not exist deliver/rusefi_debug.hex exit -1

..\misc\hex2dfu\HEX2DFU.exe deliver/rusefi_release.hex -out deliver/rusefi_release.dfu
..\misc\hex2dfu\HEX2DFU.exe deliver/rusefi_debug.hex   -out deliver/rusefi_debug.dfu
cd ..

call misc\jenkins\build_java_console.bat
if not exist java_console_binary/rusefi_console.jar exit -1

call misc\jenkins\build_simulator.bat
if not exist simulator/build/rusefi_simulator.exe exit -1

rm -rf temp
mkdir temp

set stm_arch=stm32f407
set folder=snapshot_%date:~10%%date:~4,2%%date:~7,2%_%time:~0,2%%time:~3,2%_%stm_arch%_rusefi
set folder=temp\%folder%

rem this replaces spaces with 0s - that's needed before 10am
set folder=%folder: =0%
echo "folder variable=%folder%"

pwd
call misc\jenkins\build_working_folder.bat

echo "Building only console"
cd %folder%
pwd
dir
zip ../rusefi_console.zip rusefi_console.jar rusefi.xml

if not exist ../rusefi_console.zip echo CONSOLE ZIP FAILED
if not exist ../rusefi_console.zip exit -1

echo "only console ready"

echo "Going back to root folder"
cd ..
cd ..                                                             
pwd

echo "Making rusefi_simulator.zip"
pwd
zip -j temp/rusefi_simulator.zip simulator/build/rusefi_simulator.exe firmware/tunerstudio/rusefi.ini java_console_binary/rusefi_console.jar


echo open ftp://u71977750-build:%RUSEFI_BUILD_FTP_PASS%@%FTP_SERVER%/ > ftp_commands.txt
echo binary >> ftp_commands.txt
echo put rusefi_bundle.zip >> ftp_commands.txt

echo cd separate_files >> ftp_commands.txt
echo put rusefi_simulator.zip >> ftp_commands.txt
echo put rusefi_console.zip >> ftp_commands.txt
echo exit >> ftp_commands.txt

cd temp
call winscp.com /script=../ftp_commands.txt
IF NOT ERRORLEVEL 0 echo winscp error DETECTED
IF NOT ERRORLEVEL 0 EXIT /B 1


cd ..
echo %date% %time%
echo "build_current_bundle: DONE here"
pwd

