#!/bin/bash

# fail on error
set -e

SCRIPT_NAME="common_script.sh"

echo "Entering $SCRIPT_NAME with board [$PROJECT_BOARD] and CPU [$PROJECT_CPU]"

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

if [ ! -z "$4" ]; then
  POST_BUILD_SCRIPT=$4
  echo "$SCRIPT_NAME: invoking post-build script"
  source $POST_BUILD_SCRIPT
fi

echo "$SCRIPT_NAME: build folder content:"
ls -l build

echo "$SCRIPT_NAME: deliver folder content:"
ls -l deliver
