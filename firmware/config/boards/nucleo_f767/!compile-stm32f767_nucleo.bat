rem 
rem STM32F767 version of the firmware for Nucleo-F767ZI board
rem

set script_name=!compile-nucleo_f767.bat
echo Entering %script_name%

cd ../../..
set PROJECT_BOARD=nucleo_f767
set PROJECT_CPU=ARCH_STM32F7
set EXTRA_PARAMS=-DDUMMY -DSTM32F767xx ^
 -DEFI_INJECTOR_PIN3=GPIO_UNASSIGNED ^
 -DEFI_COMMUNICATION_PIN=GPIOB_7 ^
 -DEFI_FATAL_ERROR_PIN=GPIOB_14 ^
 -DEFI_ENABLE_ASSERTS=FALSE ^
 -DCH_DBG_ENABLE_CHECKS=FALSE -DCH_DBG_ENABLE_TRACE=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE
set DEBUG_LEVEL_OPT="-O2"
make -j4
if not exist build/rusefi.hex echo FAILED to compile nucleo_f767
if not exist build/rusefi.hex exit -1

..\misc\hex2dfu\HEX2DFU.exe build/rusefi.hex -out build/rusefi.dfu

call config/boards/clean_env_variables.bat
