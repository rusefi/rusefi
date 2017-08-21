rem if you have an older Rev C discovery please update to latest Rev D firmware

rem As of March 2017 this enabled GDB debugging using ARM Eclipse plug-in

openocd\openocd.exe -f openocd/stm32f429disc1.cfg -c init -c reset
