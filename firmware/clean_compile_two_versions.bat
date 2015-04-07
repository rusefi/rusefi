
rd /s /q deliver
mkdir deliver

call clean.bat
make
cp build\rusefi.elf deliver\rusefi_debug.elf

call clean.bat
make EXTRA_PARAMS='-DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_TRACE=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE'

cp build\rusefi.elf deliver\rusefi_release.elf

