echo I am flash_openocd.bat

cp -r ../misc/install/openocd .

if not exist openocd/openocd.exe echo openocd/openocd.exe NOT FOUND
if not exist openocd/openocd.exe exit -1

openocd\openocd -f openocd/stm32f4discovery.cfg  -c "program build/rusefi.bin verify reset exit 0x08000000"

