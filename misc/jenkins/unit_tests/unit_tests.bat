echo "TIMESTAMP %date% %time% I am unit_tests.bat"

set FTP_SERVER=home451478433.1and1-data.host
rem 'RUSEFI_BUILD_FTP_PASS' environment variable 

echo We should be in root folder
pwd

rem Prior to running unit tests we build real hardware firmware

cd firmware

git submodule update --init

rm -fR .dep
rm -fR build
call update_version.bat

make -j4
if not exist build/rusefi.hex echo FAILED TO COMPILE FIRMWARE
if not exist build/rusefi.hex exit -1
cd ..

echo We are in root folder
pwd

cd unit_tests

rem Only here we are building unit_tests
call compile.bat
pwd
ls -l build
if not exist build/rusefi_test.exe echo FAILED TO COMPILE UNIT TESTS
if not exist build/rusefi_test.exe exit -1

echo "TIMESTAMP %date% %time% Unit tests build looks good, now executing unit tests"

cd build
call rusefi_test.exe
IF NOT ERRORLEVEL echo UNIT TEST FAILED
IF NOT ERRORLEVEL 0 EXIT /B 1

cd ..
cd ..

echo Back to root folder
pwd



echo Preparing firmware-only fast upload
zip -j firmware/build/rusefi_firmware.zip firmware/svnversion.h firmware/build/rusefi.hex firmware/build/rusefi.bin  firmware/build/rusefi.elf firmware/tunerstudio/rusefi.ini

echo "TIMESTAMP %date% %time% Now uploading only firmware

ncftpput -u u71977750-build -p %RUSEFI_BUILD_FTP_PASS% %FTP_SERVER% separate_files firmware/build/rusefi_firmware.zip
