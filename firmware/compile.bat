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


call generate_memory_usage_report.bat
