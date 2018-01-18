rem
rem This script would compile firmware, dev console and win32 simulator into a single bundle file
rem This script depends on Cygwin tools: zip
rem

set FTP_SERVER=home451478433.1and1-data.host

echo build_current_bundle.bat: Hello rusEfi build full bundle

java -version

echo %date% %time%

echo build_current_bundle.bat: Building win32 functional tests
cd simulator
gcc -v
make -v
cd ..



cd firmware
echo %date% %time%


echo Erasing chip
if not exist flash_erase_new_board.bat echo NOT FOUND flash_erase_new_board.bat
if not exist flash_erase_new_board.bat exit -1
call flash_erase_new_board.bat


echo build_current_bundle.bat: Building firmware
rm -fR .dep
rm -fR build
git submodule update --init

call update_version.bat

call clean_compile_two_versions.bat
if not exist deliver/rusefi_release.hex echo FAILED RELEASE
if not exist deliver/rusefi_release.hex exit -1

if not exist deliver/rusefi_debug.hex echo FAILED DEBUG
if not exist deliver/rusefi_debug.hex exit -1

cd ..


echo Building java console
cd java_console
call ant clean
call ant
cd ..

if not exist java_console_binary/rusefi_console.jar echo CONSOLE COMPILATION FAILED
if not exist java_console_binary/rusefi_console.jar exit -1

echo Building rusefi simulator
cd simulator

mkdir out
rm -rf build
rm -rf .dep
call compile.bat

if not exist build/rusefi_simulator.exe echo SIMULATOR COMPILATION FAILED
if not exist build/rusefi_simulator.exe exit -1

cd ..

rm -rf temp
mkdir temp

set folder=snapshot_%date:~10%%date:~4,2%%date:~7,2%_%time:~0,2%%time:~3,2%_rusefi
set folder=temp\%folder%

# this replaces spaces with 0s - that's needed before 10am
set folder=%folder: =0%

echo Working folder:
echo %folder%
mkdir %folder%

cp java_console_binary/rusefi_console.jar %folder%
cp simulator/build/rusefi_simulator.exe %folder%
cp firmware/tunerstudio/rusefi.ini %folder%

cp firmware/svnversion.h %folder%
cp -r misc/install/openocd %folder% 
cp java_console/rusefi.xml %folder%
cp firmware/deliver/rusefi_release.* %folder%
cp firmware/deliver/rusefi_debug.* %folder%
cp misc/console_launcher/rusefi_console.exe %folder%


cd temp
echo "Please copy find.exe to findcyg.exe in cygwin folder"
findcyg . -name '.svn' > folders_to_delete.txt
echo "Deleting .svn"
xargs rm  -rf < folders_to_delete.txt
echo "Deleted .svn"
rm -rf folders_to_delete.txt

echo "Building bundle"
pwd
zip -r rusefi_bundle.zip *
echo "Bundle ready"
cd ..
echo "We are back in root directory"
pwd

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


