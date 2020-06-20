#!/bin/sh

# for example 'proteus'
COMPILE_FOLDER="$1"

# for example 'mre-f4'
export BUNDLE_NAME="$2"

export INI_FILE_OVERRIDE="$3"
export RUSEFI_CONSOLE_SETTINGS="$4"

SCRIPT_NAME=compile_and_upload.bat
echo "Entering $SCRIPT_NAME with 1=$1 2=$2 3=$3 4=$4"

[ -n $COMPILE_FOLDER ] || (echo "COMPILE_FOLDER parameter expected"; exit -1)

[ -n $BUNDLE_NAME ] || (echo "BUNDLE_NAME parameter expected"; exit -1)

COMPILE_SCRIPT="compile-$BUNDLE_NAME.sh"

cd firmware
sh clean.sh
cd ..

cd firmware/config/boards
pwd
cd $COMPILE_FOLDER
pwd
echo "Invoking $COMPILE_SCRIPT"

sh $COMPILE_SCRIPT
[ -e build/rusefi.hex ] || (echo "Just to confirm - FAILED to compile with $COMPILE_SCRIPT"; exit -1)

cd ..
# We are back at root rusEfi folder
pwd

sh misc/jenkins/compile_other_versions/prepare_bundle.sh

echo "exiting $SCRIPT_NAME"
