#!/bin/bash

PROJECT_BOARD=$1
PROJECT_CPU=$2

# fail on error
set -e

SCRIPT_NAME="common_make.sh"
echo "Entering $SCRIPT_NAME with board $1 and CPU $2"
BOARD_DIR=$(pwd)
echo "Board dir is $BOARD_DIR"

# Back out to the firmware root, relative to this script's location, as it may be called
# from outside the firmware tree.
FW_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )/../..
FW_DIR=$(readlink -f $FW_DIR)
echo "FW dir is $FW_DIR"
cd $FW_DIR

mkdir -p .dep
echo "Calling make for the main firmware..."
make -j6 -r PROJECT_BOARD=$PROJECT_BOARD PROJECT_CPU=$PROJECT_CPU BOARD_DIR=$BOARD_DIR
[ -e build/fome.hex ] || { echo "FAILED to compile by $SCRIPT_NAME with $PROJECT_BOARD $DEBUG_LEVEL_OPT and $EXTRA_PARAMS"; exit 1; }
if [ "$USE_OPENBLT" = "yes" ]; then
  # TODO: why is this rm necessary?
  rm -f pch/pch.h.gch/*
  echo "Calling make for the bootloader..."
  cd bootloader; make -j6 PROJECT_BOARD=$PROJECT_BOARD PROJECT_CPU=$PROJECT_CPU BOARD_DIR=$BOARD_DIR; cd ..
  [ -e bootloader/blbuild/fome_bl.hex ] || { echo "FAILED to compile OpenBLT by $SCRIPT_NAME with $PROJECT_BOARD"; exit 1; }
fi

if uname | grep "NT"; then
  HEX2DFU=../misc/encedo_hex2dfu/hex2dfu.exe
else
  HEX2DFU=../misc/encedo_hex2dfu/hex2dfu.bin
fi
chmod u+x $HEX2DFU

mkdir -p deliver
rm -f deliver/*

# delete everything we're going to regenerate
rm build/fome.bin build/fome.srec

# Extract the firmware's base address from the elf - it may be different depending on exact CPU
firmwareBaseAddress="$(objdump -h -j .vectors build/fome.elf | awk '/.vectors/ {print $5 }')"
checksumAddress="$(printf "%X\n" $((0x$firmwareBaseAddress+0x1c)))"

echo "Base address is 0x$firmwareBaseAddress"
echo "Checksum address is 0x$checksumAddress"

echo "$SCRIPT_NAME: invoking hex2dfu to place image checksum"
$HEX2DFU -i build/fome.hex -c $checksumAddress -b build/fome.bin
rm build/fome.hex
# re-make hex, srec with the checksum in place
objcopy -I binary -O ihex --change-addresses=0x$firmwareBaseAddress build/fome.bin build/fome.hex
objcopy -I binary -O srec --change-addresses=0x$firmwareBaseAddress build/fome.bin build/fome.srec

if [ "$USE_OPENBLT" = "yes" ]; then
  # this image is suitable for update through bootloader only
  # srec is the only format used by OpenBLT host tools
  cp build/fome.srec deliver/fome_update.srec
else
  # standalone image (for use with no bootloader)
  cp build/fome.bin  deliver/
fi

# bootloader and combined image
if [ "$USE_OPENBLT" = "yes" ]; then
  echo "$SCRIPT_NAME: invoking hex2dfu for OpenBLT"

  # do we need all these formats?
  cp bootloader/blbuild/fome_bl.bin  deliver/fome_bl.bin

  echo "$SCRIPT_NAME: invoking hex2dfu for combined OpenBLT+FOME image"
  $HEX2DFU -i bootloader/blbuild/fome_bl.hex -i build/fome.hex -b deliver/fome.bin
fi

echo "$SCRIPT_NAME: build folder content:"
ls -l build

echo "$SCRIPT_NAME: deliver folder content:"
ls -l deliver
