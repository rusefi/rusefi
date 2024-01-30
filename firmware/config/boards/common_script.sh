#!/bin/bash

# fail on error
set -e

SCRIPT_NAME="common_script.sh"

echo "Entering $SCRIPT_NAME with board [$PROJECT_BOARD] and CPU [$PROJECT_CPU]"

mkdir -p .dep
# todo: start using env variable for number of threads or for '-r'
make -j$(nproc) -r PROJECT_BOARD=$PROJECT_BOARD PROJECT_CPU=$PROJECT_CPU
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
  make PROJECT_BOARD=$PROJECT_BOARD PROJECT_CPU=$PROJECT_CPU -j12
  cd ..
  [ -e bootloader/blbuild/openblt_$PROJECT_BOARD.hex ] || { echo "FAILED to compile OpenBLT by $SCRIPT_NAME with $PROJECT_BOARD"; exit 1; }
fi

. config/boards/common_script_hex2dfu_init.inc

if [ "$USE_OPENBLT" = "yes" ]; then
  # this image is suitable for update through bootloader only
  # do not deliver update images in any format that can confuse users
  #cp build/rusefi.bin  deliver/rusefi_update.bin
  #cp build/rusefi.dfu  deliver/rusefi_update.dfu
  #cp build/rusefi.hex  deliver/rusefi_update.hex

  # bootloader and composite image
#  echo "$SCRIPT_NAME: invoking hex2dfu for OpenBLT"
#  $HEX2DFU -i bootloader/blbuild/openblt_$PROJECT_BOARD.hex -o bootloader/blbuild/openblt_$PROJECT_BOARD.dfu

# todo: no need for complex `PROJECT = openblt_$(PROJECT_BOARD)` and complex binary name if we copy into static file name anyway!
  # do we need all these formats?
  cp bootloader/blbuild/openblt_$PROJECT_BOARD.bin  deliver/openblt.bin
#  cp bootloader/blbuild/openblt_$PROJECT_BOARD.dfu  deliver/openblt.dfu
  #cp bootloader/blbuild/openblt_$PROJECT_BOARD.hex  deliver/openblt.hex

  echo "$SCRIPT_NAME: invoking hex2dfu for composite rusEFI+OpenBLT image"
  $HEX2DFU -i bootloader/blbuild/openblt_$PROJECT_BOARD.hex -i build/rusefi.hex $CONTROL_SUM_OPTION -o deliver/rusefi.dfu -b deliver/rusefi.bin

else
  . config/boards/common_script_post_build_without_blt.inc
fi

echo "$SCRIPT_NAME: build folder content:"
ls -l build

echo "$SCRIPT_NAME: deliver folder content:"
ls -l deliver
