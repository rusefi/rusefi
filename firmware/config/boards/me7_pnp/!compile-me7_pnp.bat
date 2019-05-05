rem 
rem VAG-ME7 version of the firmware for F767ZI chip NOT on Red board, with a real oscilattor
rem

cd ../../..
set PROJECT_BOARD=me7_pnp
set PROJECT_CPU=ARCH_STM32F7
set EXTRA_PARAMS=-DDUMMY -DSTM32F767xx ^
 -DEFI_ENABLE_ASSERTS=FALSE ^
 -DEFI_USE_OSC=TRUE ^
 -DCH_DBG_ENABLE_CHECKS=FALSE -DCH_DBG_ENABLE_TRACE=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE
set DEBUG_LEVEL_OPT="-O2"
make -j4
if not exist build/rusefi.hex echo FAILED to compile ME7_PNP
if not exist build/rusefi.hex exit -1

..\misc\hex2dfu\HEX2DFU.exe build/rusefi.hex -out build/rusefi.dfu

rem Clean up so that no one inherits this value
set PROJECT_BOARD=
set PROJECT_CPU=
set EXTRA_PARAMS=
set DEBUG_LEVEL_OPT=
set USE_BOOTLOADER=
