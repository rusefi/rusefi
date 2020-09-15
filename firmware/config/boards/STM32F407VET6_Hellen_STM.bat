
echo "Compiling for Hellen-STM"
cd ../..
rem TODO: somehow this -DDUMMY is helping us to not mess up the parameters, why?!
rem https://github.com/rusefi/rusefi/issues/684
set EXTRA_PARAMS="-DDUMMY -DEFI_COMMUNICATION_PIN=GPIOD_13 -DEFI_WARNING_PIN=GPIOA_9 -DEFI_RUNNING_PIN=GPIOD_14 -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS"
set DEBUG_LEVEL_OPT="-O2"

rem make -r -j4 clean

call config/boards/common_make.bat
