@echo off
rm -rf .dep/

rm -rf build\rusefi.hex

call update_version.bat

echo Starting compilation
rem the important piece
make

rem cd build
rem if not exist rusefi.hex echo "compilation failed"
rem if not exist rusefi.hex exit -1
rem cd ..
if errorlevel 1 goto error
 
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


cd build
rem Generate human-readable version of the .map memory usage report
java -jar ../../java_tools/gcc_map_reader.jar > ../rusefi_ram_report.txt
cd ..

arm-none-eabi-size  --format=berkeley "build\rusefi.elf"

rem file, let's program the board right away
flash.bat
exit

:error
echo Compilation failed
