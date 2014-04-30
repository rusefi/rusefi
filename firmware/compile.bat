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


svn info > ../firmware_binary/version.txt
cp config/features.h ../firmware_binary
cp build/rusefi.hex ../firmware_binary
cp build/rusefi.elf ../firmware_binary

cp tunerstudio/rusefi.ini ../firmware_binary

cd ../firmware_binary
del firmaware_binary.zip
7z a firmaware_binary.zip rusefi.hex rusefi.ini features.h flash.bat
cd ../firmware


cd build
rem Generate human-readable version of the .map memory usage report
java -jar ../../java_tools/gcc_map_reader.jar > ../../firmware_binary/rusefi_ram_report.txt
cd ..

rem file, let's program the board right away
flash.bat
exit

:error
echo Compilation failed
