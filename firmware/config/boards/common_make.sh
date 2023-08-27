#!/bin/bash

PROJECT_BOARD=$1
PROJECT_CPU=$2

# fail on error
set -e

SCRIPT_NAME="common_make.sh"
echo "Entering $SCRIPT_NAME with board $1 and CPU $2"

cd ../../..

mkdir -p .dep
make -j6 -r PROJECT_BOARD=$PROJECT_BOARD PROJECT_CPU=$PROJECT_CPU
[ -e build/fome.hex ] || { echo "FAILED to compile by $SCRIPT_NAME with $PROJECT_BOARD $DEBUG_LEVEL_OPT and $EXTRA_PARAMS"; exit 1; }
if [ "$USE_OPENBLT" = "yes" ]; then
  # TODO: why is this rm necessary?
  rm -f pch/pch.h.gch/*
  cd bootloader; make -f src/Makefile PROJECT_BOARD=$PROJECT_BOARD PROJECT_CPU=$PROJECT_CPU -j12; cd ..
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

echo "$SCRIPT_NAME: invoking hex2dfu for incremental rusEFI image"
$HEX2DFU -i build/fome.hex -C 0x1C -o build/fome.dfu

if [ "$USE_OPENBLT" = "yes" ]; then
  # this image is suitable for update through bootloader only
  # srec is the only format used by OpenBLT host tools
  cp build/fome.srec deliver/fome_update.srec
else
  # standalone images (for use with no bootloader)
  cp build/fome.bin  deliver/
  cp build/fome.dfu  deliver/
  # cp build/fome.hex  deliver/
fi

# bootloader and composite image
if [ "$USE_OPENBLT" = "yes" ]; then
  rm -f deliver/fome_bl.dfu
  echo "$SCRIPT_NAME: invoking hex2dfu for OpenBLT"
  $HEX2DFU -i bootloader/blbuild/fome_bl.hex -o bootloader/blbuild/fome_bl.dfu

  # do we need all these formats?
  cp bootloader/blbuild/fome_bl.bin  deliver/fome_bl.bin
  cp bootloader/blbuild/fome_bl.dfu  deliver/fome_bl.dfu
  #cp bootloader/blbuild/fome_bl.hex  deliver/fome_bl.hex

  rm -f deliver/fome_openblt.dfu
  echo "$SCRIPT_NAME: invoking hex2dfu for composite rusEFI+OpenBLT image"
  $HEX2DFU -i bootloader/blbuild/fome_bl.hex -i build/fome.hex -C 0x1C -o deliver/fome.dfu -b deliver/fome.bin
  #todo: how to create 'signed' hex and srec? Do we need?
fi

echo "$SCRIPT_NAME: build folder content:"
ls -l build

echo "$SCRIPT_NAME: deliver folder content:"
ls -l deliver
