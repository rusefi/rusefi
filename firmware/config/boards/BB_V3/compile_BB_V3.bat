@echo off

cd ../../../
make clean -j8 PROJECT_BOARD=BB_V3
make -j8 PROJECT_BOARD=BB_V3 PROJECT_CPU=ARCH_STM32F4
gen_config_board.sh BB_V3 BB_V3

pause




