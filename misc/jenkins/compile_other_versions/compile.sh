#!/bin/bash

# for example 'proteus'
BOARD_NAME="$1"

# for example 'mre-f4'
export BUNDLE_NAME="$2"

export INI_FILE_OVERRIDE="$3"
export RUSEFI_CONSOLE_SETTINGS="$4"

SCRIPT_NAME=compile_and_upload.sh
echo "Entering $SCRIPT_NAME with 1=$1 2=$2 3=$3 4=$4"

[ -n $BOARD_NAME ] || { echo "BOARD_NAME parameter expected"; exit 1; }

[ -n $BUNDLE_NAME ] || { echo "BUNDLE_NAME parameter expected"; exit 1; }

COMPILE_SCRIPT="compile_$BUNDLE_NAME.sh"

cd firmware
bash clean.sh
cd ..

root_dir=$(pwd)

cd firmware/config/boards
pwd
cd $BOARD_NAME
pwd
echo "Invoking $COMPILE_SCRIPT"

bash $COMPILE_SCRIPT

echo "We are in"
pwd
cd ${root_dir}
echo "We are now in"
pwd

[ -e firmware/build/rusefi.hex ] || { echo "Just to confirm - FAILED to compile with $COMPILE_SCRIPT"; exit 1; }

# We are back at root rusEFI folder
pwd

echo "exiting $SCRIPT_NAME"
