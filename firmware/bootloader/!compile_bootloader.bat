@echo off

rem Needed for board overrides
IF "%BOOTLOADER_CODE_DESTINATION_PATH%"=="" (SET BOOTLOADER_CODE_DESTINATION_PATH="..")

set BOOTLOADER_CODE_DESTINATION_FILE=%BOOTLOADER_CODE_DESTINATION_PATH%/bootloader_generated.hxx
rm -f %BOOTLOADER_CODE_DESTINATION_FILE% 

echo Starting bootloader compilation...
make -j4 -f src/Makefile %1 %2 %3

if errorlevel 1 echo make compilation failed
if errorlevel 1 exit -1
 
echo Bootloader build success.

cd blbuild
rem Generate a header file with binary bootloader code
java -jar ../../../java_tools/bin2header.jar bootloader.bin %BOOTLOADER_CODE_DESTINATION_FILE% "static const volatile uint8_t bootloader_code[] BOOTLOADER_SECTION"
cd ..

rem Touch 'bootloader_storage.c' to update its modification date (needed for make)
copy /b bootloader_storage.c +,,
