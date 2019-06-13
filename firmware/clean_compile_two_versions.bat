
rd /s /q deliver
mkdir deliver

call clean.bat
echo "TIMESTAMP %date% %time%"
make -j4 DEBUG_LEVEL_OPT='-O2'  EXTRA_PARAMS='-DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_TRACE=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE'

rem mv build\rusefi.elf deliver\rusefi_no_asserts.elf
mv build\rusefi.bin deliver\rusefi_no_asserts.bin
rem this file is needed for DFU generation
mv build\rusefi.hex deliver\rusefi_no_asserts.hex
echo Release compilation results 1/2
echo "TIMESTAMP %date% %time%"
ls -l build
if not exist deliver/rusefi_no_asserts.hex echo FAILED to compile NO ASSERTS version
if not exist deliver/rusefi_no_asserts.hex exit -1

call clean.bat
echo "TIMESTAMP %date% %time%"
make -j4 

mv build\rusefi.elf deliver\rusefi.elf
rem this file is needed for DFU generation
mv build\rusefi.hex deliver\rusefi.hex
rem Keeping file in place since hw_test.bat would take it from current location
cp build\rusefi.bin deliver\rusefi.bin
echo Debug compilation results 2/2
echo "TIMESTAMP %date% %time%"
ls -l build
if not exist deliver/rusefi.hex echo FAILED to compile DEFAULT with DEBUG
if not exist deliver/rusefi.hex exit -1
