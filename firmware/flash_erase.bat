rem st-link_cli -c SWD ur -ME
rem 0x080000 would erase both code and configuration
rem use 0x040000 if you want to erase only configuration
openocd-0.8.0.exe -f interface/stlink-v2.cfg -f board/stm32f4discovery.cfg -c init -c targets -c "halt" -c "flash erase_address 0x08000000 0x080000" -c shutdown