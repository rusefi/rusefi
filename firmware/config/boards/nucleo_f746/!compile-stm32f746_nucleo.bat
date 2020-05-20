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
 -DFIRMWARE_ID=\"nucleo746\" ^
 -DLED_CRITICAL_ERROR_BRAIN_PIN=GPIOB_14 ^
 -DEFI_TOOTH_LOGGER=FALSE ^
 -DRAM_UNUSED_SIZE=10 ^
 -DSTATUS_LOGGING_BUFFER_SIZE=1400 ^
 -DCCM_UNUSED_SIZE=10
set DEBUG_LEVEL_OPT="-O2"
call config/boards/common_make.bat

