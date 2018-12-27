rem 
rem STM32F103 version of the firmware
rem

cd ../..
set EXTRA_PARAMS="-DSTM32F1XX"
set PROJECT_CPU=ST_STM32F1
set LDSCRIPT=config/stm32f1egt/STM32F103xB.ld
set USE_FPU=no
make
rem call compile_and_program.bat -r
