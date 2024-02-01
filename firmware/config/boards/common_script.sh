#!/bin/bash

# this script is supposed to be executed from within 'firmware' folder

export BOARD_META_PATH=${1:-$BOARD_META_PATH}
. config/boards/common_script_read_meta_env.inc $BOARD_META_PATH

# fail on error
set -e

SCRIPT_NAME="common_script.sh"

# temporary
if [ -z "$PROJECT_BOARD" ]; then
  export PROJECT_BOARD="$SHORT_BOARD_NAME"
fi

# check if board dir is set
echo "Entering $SCRIPT_NAME with board [$PROJECT_BOARD] and CPU [$PROJECT_CPU] at [$BOARD_DIR]"

mkdir -p .dep

if [ "$USE_OPENBLT" = "yes" ]; then
  # TODO: why is this rm necessary?
  # TODO: probably make/gcc do not like two separate projects (primary firmware and bootloader) co-existing in same folder structure?
  cd bootloader
  make -j$(nproc)
  cd ..
  [ -e bootloader/blbuild/openblt_$PROJECT_BOARD.hex ] || { echo "FAILED to compile OpenBLT by $SCRIPT_NAME with $PROJECT_BOARD"; exit 1; }
fi

rm -f pch/pch.h.gch/*
# todo: start using env variable for number of threads or for '-r'
make bundle -j$(nproc) -r

[ -e build/rusefi.hex ] || { echo "FAILED to compile by $SCRIPT_NAME with $PROJECT_BOARD $DEBUG_LEVEL_OPT and $EXTRA_PARAMS"; exit 1; }

if [ ! -z $POST_BUILD_SCRIPT ]; then
  echo "$SCRIPT_NAME: invoking post-build script"
  source $POST_BUILD_SCRIPT
fi

echo "$SCRIPT_NAME: build folder content:"
ls -l build

echo "$SCRIPT_NAME: deliver folder content:"
ls -l deliver
