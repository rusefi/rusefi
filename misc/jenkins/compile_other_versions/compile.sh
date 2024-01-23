#!/bin/bash

# fail on error!
set -e

# for example 'config/boards/proteus'
BOARD_DIR="$1"

# for example 'proteus_f4'
export BUNDLE_NAME="$2"

SCRIPT_NAME=compile.sh
echo "Entering $SCRIPT_NAME with folder $BOARD_DIR and bundle name $BUNDLE_NAME"

[ -n $BOARD_DIR ] || { echo "BOARD_DIR parameter expected"; exit 1; }

[ -n $BUNDLE_NAME ] || { echo "BUNDLE_NAME parameter expected"; exit 1; }

cd firmware
bash clean.sh
cd ..

cd firmware/$BOARD_DIR
pwd

COMPILE_SCRIPT="compile_$BUNDLE_NAME.sh"
if [ -f $COMPILE_SCRIPT ]; then
  # detailed compile script is useful for instance when same board has multiple MCU targets
  echo "[$COMPILE_SCRIPT] found!"
else
  echo "Using default script name..."
  COMPILE_SCRIPT="compile_firmware.sh"
fi

echo "Invoking $COMPILE_SCRIPT"

bash $COMPILE_SCRIPT

echo "Success for $SCRIPT_NAME!"
