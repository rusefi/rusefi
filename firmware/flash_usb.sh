#!/bin/bash

# This script will try to flash/update RusEFI part of firmware over USB interface.

echo "This script may need root privileges for accessing USB device or special udev rules (TODO)"

BootCommander -t=xcp_usb build/rusefi.srec

# OR
# You can build it from sources with:
# (cd ext/openblt/Host/Source/LibOpenBLT/ ; mkdir build ; cd build ; cmake .. ; make -j )
# and
# (cd ext/openblt/Host/Source/BootCommander/ ; mkdir build ; cd build ; cmake .. ; make -j )
# And run:
# ext/openblt/Host/BootCommander -t=xcp_usb build/rusefi.srec
