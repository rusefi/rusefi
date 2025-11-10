#!/bin/bash

# This script will try to flash/update RusEFI part of firmware over can0 interface.
#
# You may want to bring can0 interface up first:
# sudo ip link set can0 type can bitrate 500000 ; sudo ifconfig can0 up
# Sometimes (after communication errors) you may like to restart interface with:
# make sure that you have termination resistor
# sudo ifconfig can0 down; sudo ip link set can0 type can bitrate 500000 ; sudo ifconfig can0 up

# sudo apt-get install can-utils
# candump can0

/sbin/ifconfig can0 | grep RUNNING

# see efi_blt_ids.h
CAN_ARGS="-tid=10667h -rid=107e1h -xid=1"

echo This script assumes that you are using can0 interface and it is ready, otherwise read comments inside
echo This script assumes that you have BootCommander somewhere in your PATH, otherwise read comments inside
BootCommander -t=xcp_can -d=can0 $CAN_ARGS build/rusefi.srec

# OR
# You can build it from sources with:
# (cd ext/openblt/Host/Source/LibOpenBLT/ ; mkdir build ; cd build ; cmake .. ; make -j )
# and
# (cd ext/openblt/Host/Source/BootCommander/ ; mkdir build ; cd build ; cmake .. ; make -j )
# And run:
# ext/openblt/Host/BootCommander -t=xcp_can -d=can0 $CAN_ARGS build/rusefi.srec
