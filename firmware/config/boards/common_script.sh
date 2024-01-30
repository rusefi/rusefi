#!/bin/bash

# this script is supposed to be executed from within 'firmware' folder

PROJECT_BOARD=$1
PROJECT_CPU=$2

# fail on error
set -e

SCRIPT_NAME="common_script.sh"

# check if board dir is set
if [ ! -z "$3" ]; then
  echo "Entering $SCRIPT_NAME with board [$1] at [$3] and CPU [$2]"
  OPTIONAL_BOARD_DIR="BOARD_DIR=$3"
  OPTIONAL_BOARD_DIR_BL="BOARD_DIR=../$3"
else
  echo "Entering $SCRIPT_NAME with board [$1] and CPU [$2]"
  OPTIONAL_BOARD_DIR=""
  OPTIONAL_BOARD_DIR_BL=""
fi

mkdir -p .dep
# todo: start using env variable for number of threads or for '-r'
make -j$(nproc) -r PROJECT_BOARD=$PROJECT_BOARD PROJECT_CPU=$PROJECT_CPU $OPTIONAL_BOARD_DIR
[ -e build/rusefi.hex ] || { echo "FAILED to compile by $SCRIPT_NAME with $PROJECT_BOARD $DEBUG_LEVEL_OPT and $EXTRA_PARAMS"; exit 1; }

if [ ! -z "$4" ]; then
  POST_BUILD_SCRIPT=$4
  echo "$SCRIPT_NAME: invoking post-build script"
  source $POST_BUILD_SCRIPT
fi

if [ "$USE_OPENBLT" = "yes" ]; then
  # TODO: why is this rm necessary?
  # TODO: probably make/gcc do not like two separate projects (primary firmware and bootloader) co-existing in same folder structure?
  rm -f pch/pch.h.gch/*
  cd bootloader
  make PROJECT_BOARD=$PROJECT_BOARD PROJECT_CPU=$PROJECT_CPU $OPTIONAL_BOARD_DIR_BL -j12
  cd ..
  [ -e bootloader/blbuild/openblt_$PROJECT_BOARD.hex ] || { echo "FAILED to compile OpenBLT by $SCRIPT_NAME with $PROJECT_BOARD"; exit 1; }
fi

. common_script_hex2dfu_init.inc

mkdir -p deliver
rm -f deliver/*

CONTROL_SUM_OPTION="-C 0x1C"

if [ "$USE_OPENBLT" = "yes" ]; then
  # this image is suitable for update through bootloader only
  # do not deliver update images in any format that can confuse users
  #cp build/rusefi.bin  deliver/rusefi_update.bin
  #cp build/rusefi.dfu  deliver/rusefi_update.dfu
  #cp build/rusefi.hex  deliver/rusefi_update.hex
  # srec is the only format used by OpenBLT host tools
  cp build/rusefi.srec deliver/rusefi_update.srec

  # bootloader and composite image
  echo "$SCRIPT_NAME: invoking hex2dfu for OpenBLT"
  $HEX2DFU -i bootloader/blbuild/openblt_$PROJECT_BOARD.hex -o bootloader/blbuild/openblt_$PROJECT_BOARD.dfu

  # do we need all these formats?
  cp bootloader/blbuild/openblt_$PROJECT_BOARD.bin  deliver/openblt.bin
  cp bootloader/blbuild/openblt_$PROJECT_BOARD.dfu  deliver/openblt.dfu
  #cp bootloader/blbuild/openblt_$PROJECT_BOARD.hex  deliver/openblt.hex

  echo "$SCRIPT_NAME: invoking hex2dfu for composite rusEFI+OpenBLT image"
  $HEX2DFU -i bootloader/blbuild/openblt_$PROJECT_BOARD.hex -i build/rusefi.hex $CONTROL_SUM_OPTION -o deliver/rusefi.dfu -b deliver/rusefi.bin

else
  echo "$SCRIPT_NAME: invoking hex2dfu for incremental rusEFI image"
  $HEX2DFU -i build/rusefi.hex $CONTROL_SUM_OPTION -o build/rusefi.dfu

  # standalone images (for use with no bootloader)
  cp build/rusefi.bin  deliver/
  cp build/rusefi.dfu  deliver/
  cp build/rusefi.hex  deliver/
  if [ "$INCLUDE_ELF" = "yes" ]; then
   # we definitely need .elf .map .list
   cp build/rusefi.*  deliver/
  fi
fi

echo "$SCRIPT_NAME: build folder content:"
ls -l build

echo "$SCRIPT_NAME: deliver folder content:"
ls -l deliver
