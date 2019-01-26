
echo "Compiling for https://rusefi.com/forum/viewtopic.php?f=4&t=1498"
cd ../..
set EXTRA_PARAMS="-DEFI_COMMUNICATION_PIN=GPIOB_9 -DSTM32_PLLM_VALUE=25"
make clean
make
..\misc\hex2dfu\HEX2DFU.exe build/rusefi.hex   -out build/rusefi.dfu
