#!/bin/sh

openocd -f interface/jlink.cfg -c 'transport select swd' -f target/stm32f7x.cfg -c '$_TARGETNAME configure -rtos auto' $*
