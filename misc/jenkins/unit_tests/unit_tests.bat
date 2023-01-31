echo "TIMESTAMP %date% %time% I am unit_tests.bat"

rem 'RUSEFI_BUILD_FTP_PASS' environment variable

echo We should be in root folder
pwd

rem Prior to running unit tests we build real hardware firmware

git submodule update --init

cd firmware
call update_version.bat
IF NOT ERRORLEVEL 0 echo ERROR: INVOKING VERSION HEADER GENERATOR
IF NOT ERRORLEVEL 0 EXIT /B 1
cd ..

echo We are in root folder
pwd

cd unit_tests
rem Clean build is needed since some file renames would produce a broken incremental build

rem Only here we are building unit_tests
call clean_compile.bat
pwd
ls -l build
if not exist build/rusefi_test.exe echo ERROR: FAILED TO COMPILE UNIT TESTS
if not exist build/rusefi_test.exe exit -1

echo "TIMESTAMP %date% %time% Unit tests build looks good, now executing unit tests"

call build\rusefi_test.exe
IF NOT ERRORLEVEL 0 echo ERROR: UNIT TEST FAILED
IF NOT ERRORLEVEL 0 EXIT /B 1

cd ..

echo Back to root folder
pwd

