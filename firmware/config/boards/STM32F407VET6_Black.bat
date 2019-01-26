
echo "Compiling for https://rusefi.com/forum/viewtopic.php?f=4&t=1489"
cd ../..
rem TODO: somehow this -DDUMMY is helping us to not mess up the parameters, why?!
rem https://github.com/rusefi/rusefi/issues/684
set EXTRA_PARAMS="-DDUMMY -DEFI_COMMUNICATION_PIN=GPIOA_7 -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS"
make -j8 clean
make -j8
rm -f build/rusefi.dfu
dir build
..\misc\hex2dfu\HEX2DFU.exe build/rusefi.hex   -out build/rusefi.dfu
