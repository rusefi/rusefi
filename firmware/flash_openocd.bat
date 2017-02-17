echo I am flash_openocd.bat

cp -r ../misc/install/openocd .


openocd/openocd           -f openocd/stm32f429disc1.cfg                                    -c "program build/rusefi.bin verify reset exit 0x08000000"

rem openocd-0.8.0.exe -f interface/stlink-v2.cfg -f board/stm32f4discovery.cfg -c "program rusefi.bin verify reset exit 0x08000000"
