#!/bin/bash

#
# file build_working_folder.sh
#
# LTS: at some we had a dream of Long-term support branches. As of Jan 2024 this is not being used
#
# export FOLDER="temp/rusefi.999.hello" ; export BUNDLE_FULL_NAME=rusefi_test ; export INI_FILE_OVERRIDE=firmware/tunerstudio/generated/rusefi_uaefi.ini ; bash misc/jenkins/build_working_folder.sh
#

set -e

if [[ -z "$FOLDER" ]]; then
    echo "FOLDER variable not defined"
    exit -1
fi

if [[ -z "BUNDLE_FULL_NAME" ]]; then
    echo "BUNDLE_FULL_NAME variable not defined"
    exit -1
fi

if [ -z $INI_FILE_OVERRIDE ]; then
    echo "$SCRIPT_NAME: No ini_file specified!"
    exit -1
fi

echo "$SCRIPT_NAME: Will use $INI_FILE_OVERRIDE"

if [ ! -f $INI_FILE_OVERRIDE ]; then
   echo "$INI_FILE_OVERRIDE does not exist"
   exit -1
fi

FULL_BUNDLE_SHORT_FILE="${BUNDLE_FULL_NAME}.zip"
UPDATE_BUNDLE_SHORT_FILE="${BUNDLE_FULL_NAME}_autoupdate.zip"

FULL_BUNDLE_FILE="artifacts/${FULL_BUNDLE_SHORT_FILE}"
UPDATE_BUNDLE_FILE="artifacts/${UPDATE_BUNDLE_SHORT_FILE}"

echo "${BUNDLE_FULL_NAME}: Packaging $FULL_BUNDLE_FILE file"

rm -rf temp
mkdir temp

SCRIPT_NAME=build_working_folder.sh
echo "Entering $SCRIPT_NAME"

echo "Exploring build content"
ls -l firmware/build
echo "Exploring deliver content"
ls -l firmware/deliver

pwd
# This working folder name starts with 'temp/'
echo "$SCRIPT_NAME: Working folder [$FOLDER]"
mkdir $FOLDER

CONSOLE_FOLDER="$FOLDER/console"
DRIVERS_FOLDER="$FOLDER/drivers"

update_ts_cacerts_FOLDER="$FOLDER/update-ts-cacerts"

mkdir $CONSOLE_FOLDER

mkdir -p artifacts
rm -rf artifacts/*

mkdir $update_ts_cacerts_FOLDER

if [ -z $RUSEFI_CONSOLE_SETTINGS ]; then
  echo "$SCRIPT_NAME: No rusefi_console_settings"
else
  echo "Using rusefi_console_settings [$RUSEFI_CONSOLE_SETTINGS]"
  cp $RUSEFI_CONSOLE_SETTINGS $CONSOLE_FOLDER
fi

cp $INI_FILE_OVERRIDE $FOLDER

cp java_console_binary/rusefi_autoupdate.jar $CONSOLE_FOLDER
cp java_console_binary/rusefi_console.jar $CONSOLE_FOLDER
cp misc/console_launcher/rusefi_updater.exe     $FOLDER
cp misc/console_launcher/update-ts-cacerts/* $update_ts_cacerts_FOLDER
cp misc/console_launcher/readme.html      $FOLDER

# Unsetting since would not be used anywhere else
INI_FILE_OVERRIDE=""
RUSEFI_CONSOLE_SETTINGS=""

pwd

cp firmware/deliver/rusefi.bin $FOLDER

if [ -e firmware/deliver/rusefi.elf ]; then
 # ELF is useful for debug bundles
 cp firmware/deliver/rusefi.elf $FOLDER
fi

# bootloader
[ -e firmware/deliver/openblt.bin ] && { cp firmware/deliver/openblt.bin $FOLDER ; }
# USE_OPENBLT is not available this script is invoked as a separate GHA step
if [ -f firmware/deliver/openblt.bin ]; then
  echo "Taking .srec under different name"
  # srec is the only format used by OpenBLT host tools
  cp firmware/build/rusefi.srec $FOLDER/rusefi_update.srec
else
  echo "Do not need .srec"
fi
ls -l $FOLDER

[ -e firmware/deliver/rusefi.bin ] || { echo "$SCRIPT_NAME: rusefi.bin not found"; exit 1; }


# for autoupdate we do not want the unique folder name with timestamp
cd $FOLDER
pwd
zip -r ../../$UPDATE_BUNDLE_FILE *
cd ../..

echo "$SCRIPT_NAME: We are back in root directory. Now let's add files to make full bundle!"

mkdir $DRIVERS_FOLDER
# this magic file is created manually using 'make_package2.bat'
wget https://rusefi.com/build_server/st_files/silent_st_drivers2.exe -P $DRIVERS_FOLDER
[ -e $DRIVERS_FOLDER/silent_st_drivers2.exe ] || { echo "$SCRIPT_NAME: ERROR DOWNLOADING silent_st_drivers2.exe"; exit 1; }

if [ -f simulator/build/rusefi_simulator.exe ]; then
     cp simulator/build/rusefi_simulator.exe   $CONSOLE_FOLDER
fi
cp misc/console_launcher/rusefi_autoupdate.exe     $CONSOLE_FOLDER
cp misc/console_launcher/rusefi_console.exe     $CONSOLE_FOLDER
cp -r misc/install/openocd $CONSOLE_FOLDER
cp -r misc/install/STM32_Programmer_CLI $CONSOLE_FOLDER

cp java_console/*.dll                     $CONSOLE_FOLDER
cp -r java_console/bin                    $FOLDER
cp firmware/ext/openblt/Host/libopenblt.dll        $CONSOLE_FOLDER
# todo: proper build of build-libopenblt #5866 instead of having magic pre-compiled artifacts?
cp firmware/ext/openblt/Host/libopenblt.so         $CONSOLE_FOLDER
# *.dylib are Mac OS dynamic library files
cp firmware/ext/openblt/Host/libopenblt.dylib      $CONSOLE_FOLDER
cp firmware/ext/openblt/Host/openblt_jni.dll    $CONSOLE_FOLDER
cp firmware/ext/openblt/Host/libopenblt_jni.so     $CONSOLE_FOLDER
cp firmware/ext/openblt/Host/libopenblt_jni.dylib  $CONSOLE_FOLDER
# this is a bit dead cp java_tools/ts_plugin_launcher/build/jar/rusefi_ts_plugin_launcher.jar $FOLDER

cd temp
echo "Building bundle inside 'temp' folder"
pwd
zip -r ../$FULL_BUNDLE_FILE *
[ $? -eq 0 ] || (echo "$SCRIPT_NAME: ERROR INVOKING zip"; exit 1)

echo "$SCRIPT_NAME: Bundle $FULL_BUNDLE_FILE ready"
cd ..
ls -l $FULL_BUNDLE_FILE

[ -e $FULL_BUNDLE_FILE ] || { echo "$SCRIPT_NAME: ERROR not found $FULL_BUNDLE_FILE"; exit 1; }
ls -l $UPDATE_BUNDLE_FILE

echo "$SCRIPT_NAME: We are back in root directory"

pwd
ls -l artifacts
echo "Exiting $SCRIPT_NAME"
