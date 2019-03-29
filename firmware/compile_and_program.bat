@echo off

rm -rf build\rusefi.hex
rm -rf build\rusefi.bin


git submodule update --init
rem PS:
rem git submodule update --recursive --remote
rem magic once needed

call update_version.bat

echo Starting compilation
rem the important piece (pass external args if needed)
make %1 %2 %3

rem cd build
rem if not exist rusefi.hex echo "compilation failed"
rem if not exist rusefi.hex exit -1
rem cd ..
if errorlevel 1 echo make compilation failed
if errorlevel 1 exit -1
 
echo Build complete success.


rem svn info > ../firmware_binary/version.txt
rem cp config/features.h ../firmware_binary
rem cp build/rusefi.hex ../firmware_binary
rem cp build/rusefi.elf ../firmware_binary

rem cp tunerstudio/rusefi.ini ../firmware_binary

rem cd ../firmware_binary
rem del firmaware_binary.zip
rem 7z a firmaware_binary.zip rusefi.hex rusefi.ini features.h flash.bat
rem cd ../firmware


call generate_memory_usage_report.bat

echo Looking at size
arm-none-eabi-size  --format=berkeley "build\rusefi.elf"
echo Compiler version
arm-none-eabi-gcc -v

echo Have the file, let's program the board right away
call flash_openocd

echo Done flashing                       

