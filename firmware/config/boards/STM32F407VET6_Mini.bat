
echo "Compiling for https://rusefi.com/forum/viewtopic.php?f=4&t=1498"
cd ../..
rem TODO: somehow this -DDUMMY is helping us to not mess up the parameters, why?!
rem https://github.com/rusefi/rusefi/issues/684
rem this board has only 512K flash so using custom FLASH_ADDR
rem You probably want "flash0  : org = 0x08000000, len = 450K" in the .ld file
set EXTRA_PARAMS=-DDUMMY -DEFI_COMMUNICATION_PIN=GPIOB_9 -DSTM32_RTCPRE_VALUE=25 -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS^
 -DEFI_INTERNAL_FLASH=FALSE ^
 -DHAL_USE_RTC=FALSE ^
 -DBOARD_OTG_NOVBUSSENS ^
 -DFLASH_ADDR=0x08075000

make -j8 clean

call config/boards/common_make.bat
