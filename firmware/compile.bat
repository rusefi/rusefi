@echo off

rm -rf build\fome.hex
rm -rf build\fome.bin


git submodule update --init
rem PS:
rem git submodule update --recursive --remote
rem magic once needed

call update_version.bat

rem WHY??? do we have to sometimes create this folder manually?! is this about '-r' flag with make?
mkdir .dep


echo Starting compilation
rem the important piece (pass external args if needed)

make %1 %2 %3

rem cd build
rem if not exist fome.hex echo "compilation failed"
rem if not exist fome.hex exit -1
rem cd ..
if errorlevel 1 echo make compilation failed
if errorlevel 1 exit -1
 
echo Build complete success.


call generate_memory_usage_report.bat
