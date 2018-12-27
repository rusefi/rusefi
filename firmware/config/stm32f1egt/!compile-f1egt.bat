rem 
rem STM32F469 version of the firmware for https://rusefi.com/forum/viewtopic.php?f=4&t=1215
rem

cd ../../..
set EXTRA_PARAMS="-DSTM32F469xx"
set PROJECT_CPU=ST_STM32F1
set LDSCRIPT=config/stm32f1egt/STM32F103xB.ld
call compile_and_program.bat -r
