
rd /s /q deliver
mkdir deliver

call clean.bat
make
cp build\rusefi.elf deliver\rusefi_debug.elf
cp build\rusefi.bin deliver\rusefi_debug.bin
cp build\rusefi.hex deliver\rusefi_debug.hex
echo Debug compilation results
ls -l build

call clean.bat
make DEBUG_LEVEL_OPT='-O2'  EXTRA_PARAMS='-DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_TRACE=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE'

cp build\rusefi.elf deliver\rusefi_release.elf
cp build\rusefi.bin deliver\rusefi_release.bin
cp build\rusefi.hex deliver\rusefi_release.hex
echo Release compilation results
ls -l build
