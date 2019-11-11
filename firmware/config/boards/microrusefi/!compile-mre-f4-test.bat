
cd ../../..

set PROJECT_BOARD=microrusefi
set PROJECT_CPU=ARCH_STM32F4
set DEFAULT_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=MRE_BOARD_TEST

call config/boards/common_make.bat

call config/boards/clean_env_variables.bat
