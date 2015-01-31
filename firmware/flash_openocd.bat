openocd-0.8.0.exe -f interface/stlink-v2.cfg -f board/stm32f4discovery.cfg -c init -c targets -c "halt" -c "flash write_image erase rusefi.elf" -c "verify_image rusefi.elf" -c "reset run" -c shutdown
