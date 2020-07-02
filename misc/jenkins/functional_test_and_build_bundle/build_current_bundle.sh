#!/bin/sh

# This script would compile firmware, dev console and win32 simulator into a single bundle file
# This script depends on Cygwin tools: zip

SCRIPT_NAME="build_current_bundle"
ROOT_FOLDER=$(pwd)
echo "$SCRIPT_NAME Entering ROOT_FOLDER=$ROOT_FOLDER"

echo "$SCRIPT_NAME.sh: Hello rusEfi build full bundle"
date "+%a %D %T.%2S"

cd firmware/bootloader
sh clean_bootloader.sh
sh compile_bootloader_discovery407.sh
[ -e bootloader_generated.hxx ] || { echo "FAILED TO COMPILE BOOTLOADER"; exit 1; }
pwd
cd ../..
# At root folder here

cd firmware
date "+%a %D %T.%2S"

echo "$SCRIPT_NAME: will be Erasing chip"
[ -e flash_erase407.sh ] || { echo "NOT FOUND flash_erase.sh"; exit 1; ]
echo "$SCRIPT_NAME: Erasing chip"
sh flash_erase407.sh

echo "$SCRIPT_NAME: Building firmware"
sh clean.sh

sh update_version.sh

sh clean_compile_two_versions.sh
[ -e deliver/rusefi_no_asserts.hex ] || { echo "Just to confirm - FAILED to compile no_asserts"; exit 1; }

[ -e deliver/rusefi.hex ] { echo "Just to confirm - FAILED to compile default DEBUG"; exit 1; }

echo "$SCRIPT_NAME: Building DFU"
if uname | grep "NT"; then
 chmod u+x ../misc/encedo_hex2dfu/hex2dfu.exe
 ../misc/encedo_hex2dfu/hex2dfu.exe -i deliver/rusefi_no_asserts.hex -o deliver/rusefi_no_asserts.dfu
 ../misc/encedo_hex2dfu/hex2dfu.exe -i build/rusefi.hex -o deliver/rusefi.dfu
else
 chmod u+x ../misc/encedo_hex2dfu/hex2dfu.bin
 ../misc/encedo_hex2dfu/hex2dfu.bin -i deliver/rusefi_no_asserts.hex -o deliver/rusefi_no_asserts.dfu
 ../misc/encedo_hex2dfu/hex2dfu.bin -i build/rusefi.hex -o deliver/rusefi.dfu
fi

ls -l deliver
cd ..

# At root folder here

sh misc/jenkins/build_java_console.sh
[ -e java_console_binary/rusefi_console.jar ] || { echo "rusefi_console.jar build FAILED"; exit -1; }

sh misc/jenkins/build_simulator.sh
[ -e simulator/build/rusefi_simulator.exe ] || { echo "rusefi_simulator.exe build FAILED"; exit -1; }

STM_ARCH="stm32f407"
TIMESTAMP=$(date +%Y%m%d_%H%M%S')

FOLDER="snapshot_${TIMESTAMP}_${STM_ARCH}_rusefi"
echo "$SCRIPT_NAME: folder variable1=$FOLDER"
export FOLDER="temp/$FOLDER"
echo "$SCRIPT_NAME: folder variable3=$FOLDER"

pwd
export BUNDLE_FULL_NAME="rusefi_bundle"
sh misc/jenkins/build_working_folder.sh
[ $? -eq 0 ] || { echo "$SCRIPT_NAME: ERROR: invoking build_working_folder.sh"; exit 1; }

echo "$SCRIPT_NAME: Building only console"
pwd
ls
zip $ROOT_FOLDER/temp/rusefi_console.zip $ROOT_FOLDER/java_console_binary/rusefi_console.jar $ROOT_FOLDER/java_console/rusefi.xml

[ -e $ROOT_FOLDER/temp/rusefi_console.zip ] || { echo "CONSOLE ZIP FAILED"; exit 1; }

echo "$SCRIPT_NAME: only console ready"

echo "$SCRIPT_NAME: Going back to root folder"
cd $ROOT_FOLDER
pwd

echo "TIMESTAMP $(date "+%a %D %T.%2S")"
pwd
echo "exiting $SCRIPT_NAME"
