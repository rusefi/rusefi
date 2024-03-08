#!/bin/bash

#
# file build_working_folder.sh
#

set -euo pipefail

FULL_BUNDLE_FILE="${BUNDLE_FULL_NAME}.zip"

echo "${BUNDLE_FULL_NAME}: Packaging temp/$FULL_BUNDLE_FILE file"

rm -rf temp
mkdir temp

SCRIPT_NAME=build_working_folder.sh
echo "Entering $SCRIPT_NAME"

pwd
# This working folder name starts with 'temp/'
echo "$SCRIPT_NAME: Working folder: $FOLDER"
mkdir $FOLDER

CONSOLE_FOLDER="$FOLDER/console"
DRIVERS_FOLDER="$FOLDER/drivers"

mkdir $CONSOLE_FOLDER
mkdir $DRIVERS_FOLDER
ls -l $FOLDER

# this magic file is created manually using 'make_package2.bat'
wget https://rusefi.com/build_server/st_files/silent_st_drivers2.exe -P $DRIVERS_FOLDER
[ -e $DRIVERS_FOLDER/silent_st_drivers2.exe ] || { echo "$SCRIPT_NAME: ERROR DOWNLOADING silent_st_drivers2.exe"; exit 1; }

if [ -z $INI_FILE_OVERRIDE ]; then
    INI_FILE_OVERRIDE="rusefi.ini"
    echo "$SCRIPT_NAME: No ini_file_override specified"
fi
echo "$SCRIPT_NAME: Will use $INI_FILE_OVERRIDE"

cp java_console_binary/fome_console.jar $CONSOLE_FOLDER
cp simulator/build/fome_simulator.exe     $CONSOLE_FOLDER
cp misc/console_launcher/fome_*.exe     $CONSOLE_FOLDER
cp firmware/ext/openblt/Host/libopenblt.dll        $CONSOLE_FOLDER
cp firmware/ext/openblt/Host/libopenblt.so         $CONSOLE_FOLDER
cp firmware/ext/openblt/Host/libopenblt.dylib      $CONSOLE_FOLDER
cp firmware/ext/openblt/Host/openblt_jni.dll    $CONSOLE_FOLDER
cp firmware/ext/openblt/Host/libopenblt_jni.so     $CONSOLE_FOLDER
cp firmware/ext/openblt/Host/libopenblt_jni.dylib  $CONSOLE_FOLDER

cp firmware/tunerstudio/generated/$INI_FILE_OVERRIDE $FOLDER
# Unsetting since would not be used anywhere else
INI_FILE_OVERRIDE=""

cp -r misc/install/STM32_Programmer_CLI $CONSOLE_FOLDER

cp firmware/deliver/fome.bin $FOLDER

# bootloader
[ -e firmware/deliver/fome_bl.bin ] && { cp firmware/deliver/fome_bl.bin $FOLDER ; }
# update srec
[ -e firmware/deliver/fome_update.srec ] && { cp firmware/deliver/fome_update.srec $FOLDER ; }

[ -e firmware/deliver/fome.bin ] || { echo "$SCRIPT_NAME: fome.bin not found"; exit 1; }

cd temp


echo "Building bundle"
pwd
zip -r $FULL_BUNDLE_FILE *
[ $? -eq 0 ] || (echo "$SCRIPT_NAME: ERROR INVOKING zip"; exit 1)

echo "Bundle $FULL_BUNDLE_FILE ready"
ls -l $FULL_BUNDLE_FILE

[ -e $FULL_BUNDLE_FILE ] || { echo "$SCRIPT_NAME: ERROR not found $FULL_BUNDLE_FILE"; exit 1; }

cd ..

mkdir -p artifacts
mv temp/$FULL_BUNDLE_FILE artifacts

echo "Removing static content from ${CONSOLE_FOLDER} and $DRIVERS_FOLDER"
rm -rf $CONSOLE_FOLDER/fome_console.exe
rm -rf $CONSOLE_FOLDER/DfuSe
rm -rf $DRIVERS_FOLDER

echo "$SCRIPT_NAME: We are back in root directory"
