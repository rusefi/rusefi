#!/bin/bash
# This is a Linux/Unix script to do USB DFU (Device Firmware Update).
#
# The STM32 has multiple ways to write an application into the MCU.  One is
# is USB DFU mode.
# This can write a blank or overwrite a corrupted ECU, but does require
# that the processor be set into DFU mode by holding down the 'Program' (AKA
# 'Boot1') button at reset or power up.
# Note: The 'Program' button is only checked immediately after reset.  The
# button may be released after that instant.  A reset or power cycle may be
# needed to exit DFU mode.
#

dfu-util --alt 0 --download deliver/rusefi.dfu -s :leave

# For Windows the batch file flash_dfu.bat runs the equivalent command
#   DfuSeCommand.exe -c -d --fn deliver/rusefi.dfu
