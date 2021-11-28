#!/bin/bash

# This script would compile firmware, dev console and win32 simulator into a single bundle file
# This script depends on Cygwin tools: zip

SCRIPT_NAME="build_current_bundle"
ROOT_FOLDER=$(pwd)
echo "$SCRIPT_NAME Entering ROOT_FOLDER=$ROOT_FOLDER"

echo "$SCRIPT_NAME.sh: Hello rusEfi build full bundle"
date "+%a %D %T.%2S"

export BUNDLE_NAME="default"

cd firmware/bootloader
bash clean_bootloader.sh
bash compile_bootloader_discovery407.sh
[ -e bootloader_generated.hxx ] || { echo "FAILED TO COMPILE BOOTLOADER"; exit 1; }
pwd
cd ../..
# At root folder here

cd firmware
date "+%a %D %T.%2S"

echo "$SCRIPT_NAME: will be Erasing chip"

[ -e flash_erase407.sh ] || { echo "NOT FOUND flash_erase.sh"; exit 1; }
echo "$SCRIPT_NAME: Erasing chip"
bash flash_erase407.sh

echo "$SCRIPT_NAME: Building firmware"
bash clean.sh

bash update_version.sh

bash clean_compile_two_versions.sh
#[ -e deliver/rusefi_no_asserts.hex ] || { echo "Just to confirm - FAILED to compile no_asserts"; exit 1; }

[ -e deliver/rusefi.hex ] || { echo "Just to confirm - FAILED to compile default DEBUG"; exit 1; }

echo "$SCRIPT_NAME: Building DFU"
if uname | grep "NT"; then
 chmod u+x ../misc/encedo_hex2dfu/hex2dfu.exe

# ../misc/encedo_hex2dfu/hex2dfu.exe -i deliver/rusefi_no_asserts.hex -o deliver/rusefi_no_asserts.dfu
 ../misc/encedo_hex2dfu/hex2dfu.exe -i deliver/rusefi.hex -o deliver/rusefi.dfu
else
 chmod u+x ../misc/encedo_hex2dfu/hex2dfu.bin
# ../misc/encedo_hex2dfu/hex2dfu.bin -i deliver/rusefi_no_asserts.hex -o deliver/rusefi_no_asserts.dfu
 ../misc/encedo_hex2dfu/hex2dfu.bin -i deliver/rusefi.hex -o deliver/rusefi.dfu
fi

echo "${SCRIPT_NAME} ls -l deliver"
ls -l deliver
cd ..

# At root folder here

bash misc/jenkins/build_java_console.sh
[ -e java_console_binary/rusefi_console.jar ] || { echo "rusefi_console.jar build FAILED"; exit 1; }

bash misc/jenkins/build_simulator.sh
[ -e simulator/build/rusefi_simulator.exe ] || { echo "rusefi_simulator.exe build FAILED"; exit 1; }

STM_ARCH="stm32f407"
TIMESTAMP=$(date "+%Y%m%d_%H%M%S")

FOLDER="snapshot_${TIMESTAMP}_${STM_ARCH}_rusefi"
echo "$SCRIPT_NAME: folder variable1=$FOLDER"
export FOLDER="temp/$FOLDER"
echo "$SCRIPT_NAME: folder variable3=$FOLDER"

pwd
# DfuFlasher.java validates this prefix
export BUNDLE_FULL_NAME="rusefi_bundle"
bash misc/jenkins/build_working_folder.sh
[ $? -eq 0 ] || { echo "$SCRIPT_NAME: ERROR: invoking build_working_folder.sh"; exit 1; }

echo "$SCRIPT_NAME: Going back to root folder"
cd "$ROOT_FOLDER"
pwd

echo "TIMESTAMP $(date "+%a %D %T.%2S")"
pwd
echo "exiting $SCRIPT_NAME"
