#!/bin/bash

# sudo apt-get install stlink-tools -y
# sudo apt-get remove --purge stlink-tools

st-flash write rusefi.bin 0x08000000