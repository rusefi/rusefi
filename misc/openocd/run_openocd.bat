rem if you have an older Rev C discovery please update to latest Rev D firmware

openocd-0.8.0 -f interface/stlink-v2.cfg -f target/stm32f4x_stlink.cfg -c init -c reset
