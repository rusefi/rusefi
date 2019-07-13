set script_name=common_make.bat
echo Entering %script_name%

mkdir .dep
rem todo: start using env variable for number of threads or for '-r'
make -j4 -r
if not exist build/rusefi.hex echo FAILED to compile by %script_name% with %PROJECT_BOARD% %DEBUG_LEVEL_OPT% and %EXTRA_PARAMS%
if not exist build/rusefi.hex exit -1

mkdir deliver

rm -f deliver/rusefi.dfu
echo %script_name%: invoking hex2dfu.exe
..\misc\encedo_hex2dfu\hex2dfu.exe -i build/rusefi.hex -o deliver/rusefi.dfu
cp rusefi.bin deliver/

ls -l deliver