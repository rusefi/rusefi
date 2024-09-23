#!/bin/sh

# Please use https://github.com/dron0gus/openocd/tree/artery-dev OpenOCD for Artery chips support
# Plase run this script from firmware directory

openocd -f interface/jlink.cfg -c 'transport select swd' -f ChibiOS/demos/STM32/RT-AT32F435-ARTERY144/at32f4x.cfg -c '$_TARGETNAME configure -rtos auto' $*
