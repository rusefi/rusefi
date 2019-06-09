rem 
rem STM32F767 version of the firmware for Nucleo-F746ZG board
rem

set script_name=!compile-nucleo_f746.bat
echo Entering %script_name%

cd ../../..
set PROJECT_BOARD=nucleo_F746
set PROJECT_CPU=ARCH_STM32F7
set EXTRA_PARAMS=-DDUMMY -DSTM32F746xx ^
 -DEFI_INJECTOR_PIN3=GPIO_UNASSIGNED ^
 -DEFI_COMMUNICATION_PIN=GPIOB_7 ^
 -DEFI_FATAL_ERROR_PIN=GPIOB_14 ^
 -DRAM_UNUSED_SIZE=10 -DCCM_UNUSED_SIZE=10
set DEBUG_LEVEL_OPT="-O2"
call config/boards/common_make.bat

call config/boards/clean_env_variables.bat