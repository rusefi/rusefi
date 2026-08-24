#!/bin/bash

# This script will try to flash/update RusEFI part of firmware over the net

# see blt_conf.h
NET_ARGS="-a=192.168.10.1 -p=1000"

echo This script assumes that you have BootCommander somewhere in your PATH, otherwise read comments inside
BootCommander -t=xcp_net -a= $NET_ARGS build/rusefi.srec

# OR
# You can build it from sources with:
# (cd ext/openblt/Host/Source/LibOpenBLT/ ; mkdir build ; cd build ; cmake .. ; make -j )
# and
# (cd ext/openblt/Host/Source/BootCommander/ ; mkdir build ; cd build ; cmake .. ; make -j )
# And run:
# ext/openblt/Host/BootCommander -t=xcp_net -a= $NET_ARGS build/rusefi.srec
