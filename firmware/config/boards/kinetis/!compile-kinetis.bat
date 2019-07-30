echo "Compiling for Kinetis MKE16F512"
rem TODO: somehow this -DDUMMY is helping us to not mess up the parameters, why?!
rem https://github.com/rusefi/rusefi/issues/684
set EXTRA_PARAMS=-DDUMMY -DCPU_MKE16F512VLH16 -DCPU_MKE16F512VLH16_cm4 -D__USE_CMSIS^
 -DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_CHECKS=FALSE -DCH_DBG_ENABLE_TRACE=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE^
 -DFIRMWARE_ID=\"kinetis\" ^
 -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
set BUILDDIR=build_kinetis
set PROJECT_BOARD=kinetis
set PROJECT_CPU=kinetis
set DEBUG_LEVEL_OPT="-O2"
set USE_FATFS=no
set USE_BOOTLOADER=no
rem -DCH_DBG_ENABLE_CHECKS=FALSE

cd ../../..

rm -f build_kinetis/rusefi_kinetis.*

make -r -j4 2> err.log
rem rm -f build/rusefi.dfu
rem dir build
rem ..\misc\hex2dfu\HEX2DFU.exe build/rusefi.hex   -out build/rusefi.dfu
