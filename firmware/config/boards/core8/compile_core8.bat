@echo off

cd firmware
make clean -j8 PROJECT_BOARD=core8
make -j8 PROJECT_BOARD=core8 PROJECT_CPU=ARCH_STM32F4
bash.exe gen_config_board.sh core8 core8 rusefi_core8.ini




