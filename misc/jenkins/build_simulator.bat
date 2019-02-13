echo Building rusefi simulator
pwd
cd simulator
gcc -v
make -v
mkdir out
rm -rf build
rm -rf .dep
call compile.bat
cd ..
if not exist simulator/build/rusefi_simulator.exe echo SIMULATOR COMPILATION FAILED
