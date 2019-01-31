set PATH=C:\Program Files\Java\jre\bin;G:\VStudio\Subversion\bin;G:\VStudio\arm-elf-gcc\bin;G:\VStudio\MinGW\msys\1.0\bin;G:\VStudio\Git\bin
echo "Compiling for https://rusefi.com/forum/viewtopic.php?f=4&t=1498"
cd ../..
rem TODO: somehow this -DDUMMY is helping us to not mess up the parameters, why?!
rem https://github.com/rusefi/rusefi/issues/684
rem this board has only 512K flash so using custom FLASH_ADDR
rem You probably want "flash0  : org = 0x08000000, len = 450K" in the .ld file
set EXTRA_PARAMS="-DDUMMY -DEFI_COMMUNICATION_PIN=GPIOB_9 -DSTM32_PLLM_VALUE=25 -DSTM32_RTCPRE_VALUE=25 -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS -DFLASH_ADDR=0x08075000 -DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_CHECKS=FALSE -DCH_DBG_ENABLE_TRACE=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE"
set DEBUG_LEVEL_OPT="-O2"
set USE_FPU="softfp"
set LDSCRIPT=LED/stm32f4_flash.ld
rem make -r clean
rm -rf .dep/

rm -rf build\rusefi.hex
rm -rf build\rusefi.bin

make -r
rem rm -f build/rusefi.dfu
rem dir build
rem ..\misc\hex2dfu\HEX2DFU.exe build/rusefi.hex   -out build/rusefi.dfu
