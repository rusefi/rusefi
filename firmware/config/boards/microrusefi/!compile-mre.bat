
cd ../../..

set LDSCRIPT = config/boards/NUCLEO_F767/STM32F76xxI.ld

set PROJECT_BOARD=nucleo_f767
set PROJECT_CPU=ARCH_STM32F7
set EXTRA_PARAMS=-DDUMMY ^
 -DSTM32F767xx ^
 -DEFI_USE_OSC=TRUE ^
 -DEFI_COMMUNICATION_PIN=GPIOE_1 ^
 -DEFI_FATAL_ERROR_PIN=GPIOE_2 ^
 -DEFI_RUNNING_PIN=GPIOE_3 ^
 -DEFI_WARNING_PIN=GPIOE_4

call config/boards/common_make.bat

call config/boards/clean_env_variables.bat
