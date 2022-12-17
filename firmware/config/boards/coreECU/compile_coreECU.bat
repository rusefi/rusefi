@echo off

cd ../../../
make clean -j8 PROJECT_BOARD=coreECU
make -j8 PROJECT_BOARD=coreECU PROJECT_CPU=ARCH_STM32F4
gen_config_board.sh coreECU coreECU
pause




