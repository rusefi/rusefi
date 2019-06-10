
cd ../../..

set EXTRA_PARAMS=-DDUMMY ^
 -DEFI_COMMUNICATION_PIN=GPIOE_1 ^
 -DEFI_FATAL_ERROR_PIN=GPIOE_2 ^
 -DEFI_RUNNING_PIN=GPIOE_3 ^
 -DEFI_WARNING_PIN=GPIOE_4 ^



call config/boards/common_make.bat

call config/boards/clean_env_variables.bat
