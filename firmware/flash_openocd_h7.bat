echo I am flash_openocd_h7.bat

cd ../misc/install

if not exist openocd/openocd.exe echo openocd/openocd.exe NOT FOUND
if not exist openocd/openocd.exe exit -1

openocd\openocd -f openocd/st_nucleo_h743zi.cfg  -c "program ../../firmware/build/rusefi.bin verify reset exit 0x08000000"

