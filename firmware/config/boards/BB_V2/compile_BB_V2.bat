@echo off

cd ../../../
make clean -j8 PROJECT_BOARD=BB_V2
make -j8 PROJECT_BOARD=BB_V2 PROJECT_CPU=ARCH_STM32F4
gen_config_board.sh config/boards/BB_V2 BB_V2 rusefi_BB_V2.ini

pause




