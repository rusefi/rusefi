
rem this is about programming stm32 directly - i.e. without ST Link/SWD or anything. 
rem you need this if you've burned your st-link or if you have naked stm32 chip soldered somewhere


arm-none-eabi-objcopy -O binary rusefi.elf rusefi.dfu



rem dfu-util-static.exe -a 0 -D fm_rusefi.dfu 