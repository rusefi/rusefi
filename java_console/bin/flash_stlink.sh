#!/bin/bash

# sudo apt-get install stlink-tools -y
# sudo apt-get remove --purge stlink-tools
# armbian packages repo has 1.5 which does not seem to support ST-LINK 2.1 from newest discovery boards
# version 1.6.1 of https://github.com/stlink-org/stlink works!
# See https://freeelectron.ro/installing-st-link-v2-to-flash-stm32-targets-on-linux/
# master of https://github.com/stlink-org/stlink does not work https://github.com/stlink-org/stlink/issues/1165

st-flash write rusefi.bin 0x08000000