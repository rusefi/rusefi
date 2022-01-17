#!/bin/bash

# This script will flash OpenBLT bootloader only.
# User should prefer to flash composite image,
# see flash_dfu.sh/flash_dfu.bat

dfu-util --alt 0 --download deliver/openblt.dfu --reset
