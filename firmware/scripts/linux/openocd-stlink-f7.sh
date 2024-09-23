#!/bin/sh

openocd -f interface/stlink.cfg -f target/stm32f7x.cfg -c '$_TARGETNAME configure -rtos auto' $*
