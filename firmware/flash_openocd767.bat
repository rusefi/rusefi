echo I am flash_openocd767.bat

cd ../misc/install

if not exist openocd/openocd.exe echo openocd/openocd.exe NOT FOUND
if not exist openocd/openocd.exe exit -1

openocd\openocd -f openocd/st_nucleo_f7.cfg  -c "program ../../firmware/build/rusefi.bin verify reset exit 0x08000000"

