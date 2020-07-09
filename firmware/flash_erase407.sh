#!/bin/bash

# st-link_cli -c SWD ur -ME
# 0x100000 would erase both code and configuration
# use 0x080000 if you want to erase only configuratio
SCRIPT_NAME="flash_erase407.sh"
echo "Entering $SCRIPT_NAME"


# weird, it used to be much nicer with openocd 0.8.0, file location was not broken?
# maybe https://sourceforge.net/p/openocd/tickets/105/ ?

pwd
cd ../misc/install

[ -e openocd/openocd.exe ] || { "echo openocd/openocd.exe NOT FOUND"; exit 1; }

# newer discovery boards
echo "Invoking openocd..."
./openocd/openocd.exe -f openocd/stm32f4discovery.cfg -c init -c targets -c "halt" -c "flash erase_address 0x08000000 0x0100000" -c shutdown
[ $? -eq 0 ] || { echo "ERROR in $SCRIPT_NAME"; exit 1; }

echo "Just invoked openocd to erase chip!"

# older discovery boards or cheap eBay ST-Link
# openocd\openocd.exe -f openocd/stm32f4discovery.cfg -c init -c targets -c "halt" -c "flash erase_address 0x08000000 0x0100000" -c shutdown

echo "exiting $SCRIPT_NAME"
