
cd ../../..

set LDSCRIPT = config/boards/NUCLEO_F767/STM32F76xxI.ld

set PROJECT_BOARD=proteus
set PROJECT_CPU=ARCH_STM32F7

call config/boards/common_make.bat

