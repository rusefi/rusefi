rem st-link_cli -c SWD ur -ME
rem 0x080000 would erase both code and configuration
rem use 0x040000 if you want to erase only configuratio
echo I am flash_erase.bat

rem weird, it used to be much nicer with openocd 0.8.0, file location was not broken?
rem maybe https://sourceforge.net/p/openocd/tickets/105/ ?

cp -r ../misc/install/openocd .

openocd/openocd           -f openocd/stm32f429disc1.cfg                                    -c init -c targets -c "halt" -c "flash erase_address 0x08000000 0x080000" -c shutdown

rem openocd-0.8.0.exe -f interface/stlink-v2.cfg -f board/stm32f4discovery.cfg -c init -c targets -c "halt" -c "flash erase_address 0x08000000 0x080000" -c shutdown