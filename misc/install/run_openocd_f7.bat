# it's fragile :( sometimes you have to turn things on/off but cheap stlink v2 does connect to F7 using this script
# hint: use some other tool to transfer binary, use GDB only for actual debug

openocd\openocd.exe -f openocd/st_nucleo_f7.cfg -c init -c reset
