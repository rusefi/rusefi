rem 
rem STM32F767 version of the firmware for Nucleo-F746ZG board
rem

cd ../../..
set PROJECT_BOARD=NUCLEO_F746
set PROJECT_CPU=ST_STM32F7
set EXTRA_PARAMS=-DDUMMY -DSTM32F746xx ^
 -DEFI_INJECTOR_PIN3=GPIO_UNASSIGNED

rem call compile_and_program.bat -r
make -j1
