#!/bin/sh

openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c '$_TARGETNAME configure -rtos auto' $*
