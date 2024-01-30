#!/bin/bash

# LTS: at some we had a dream of Long-term support branches. As of Jan 2024 this is not being used

set -e

#
# file build_working_folder.sh
#

FULL_BUNDLE_FILE="${BUNDLE_FULL_NAME}.zip"
UPDATE_BUNDLE_FILE="${BUNDLE_FULL_NAME}_autoupdate.zip"

LTS_NAME=$1
LTS_FOLDER="build_server/lts/${LTS_NAME}"
LTS=$2

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

update_ts_cacerts_FOLDER="$FOLDER/update-ts-cacerts"

mkdir $CONSOLE_FOLDER
mkdir $DRIVERS_FOLDER

mkdir $update_ts_cacerts_FOLDER
ls -l $FOLDER

# this magic file is created manually using 'make_package2.bat'
wget https://rusefi.com/build_server/st_files/silent_st_drivers2.exe -P $DRIVERS_FOLDER
[ -e $DRIVERS_FOLDER/silent_st_drivers2.exe ] || { echo "$SCRIPT_NAME: ERROR DOWNLOADING silent_st_drivers2.exe"; exit 1; }

if [ -z $INI_FILE_OVERRIDE ]; then
    echo "$SCRIPT_NAME: No ini_file specified!"
    exit -1
fi
echo "$SCRIPT_NAME: Will use $INI_FILE_OVERRIDE"

if [ ! -f $INI_FILE_OVERRIDE ]; then
   echo "$INI_FILE_OVERRIDE does not exist"
   exit -1
fi

if [ -z $RUSEFI_CONSOLE_SETTINGS ]; then
  echo "$SCRIPT_NAME: No rusefi_console_settings"
else
  echo "Using rusefi_console_settings [$RUSEFI_CONSOLE_SETTINGS]"
  cp $RUSEFI_CONSOLE_SETTINGS $CONSOLE_FOLDER
fi

cp java_console_binary/rusefi_autoupdate.jar $CONSOLE_FOLDER
cp java_console_binary/rusefi_console.jar $CONSOLE_FOLDER
cp java_tools/ts_plugin_launcher/build/jar/rusefi_ts_plugin_launcher.jar $FOLDER
if [ -f simulator/build/rusefi_simulator.exe ]; then
     cp simulator/build/rusefi_simulator.exe   $CONSOLE_FOLDER
fi
cp misc/console_launcher/rusefi_autoupdate.exe     $CONSOLE_FOLDER
cp misc/console_launcher/rusefi_console.exe     $CONSOLE_FOLDER
cp misc/console_launcher/rusefi_updater.exe     $FOLDER
cp misc/console_launcher/update-ts-cacerts/* $update_ts_cacerts_FOLDER
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

cp misc/console_launcher/readme.html      $FOLDER

cp $INI_FILE_OVERRIDE $FOLDER
# Unsetting since would not be used anywhere else
INI_FILE_OVERRIDE=""
RUSEFI_CONSOLE_SETTINGS=""

# users probably do not really care for this file
# cp firmware/svnversion.h $FOLDER

cp -r misc/install/openocd $CONSOLE_FOLDER
cp -r misc/install/STM32_Programmer_CLI $CONSOLE_FOLDER
# 407 has additional version of firmware
#cp firmware/deliver/rusefi_no_asserts.bin $FOLDER
#cp firmware/deliver/rusefi_no_asserts.dfu $FOLDER
# just for now - DFU work in progress
#cp firmware/deliver/rusefi_no_asserts.hex $FOLDER

pwd
echo "Exploring build content"
ls -l firmware/build
echo "Exploring deliver content"
ls -l firmware/deliver

cp firmware/deliver/rusefi.bin $FOLDER

cp firmware/deliver/rusefi.dfu $FOLDER

if [ -e firmware/deliver/rusefi.hex ]; then
 # just for now - DFU work in progress
 cp firmware/deliver/rusefi.hex $FOLDER
fi

if [ -e firmware/deliver/rusefi.elf ]; then
 # ELF is useful for debug bundles
 cp firmware/deliver/rusefi.elf $FOLDER
fi

# bootloader
[ -e firmware/deliver/openblt.bin ] && { cp firmware/deliver/openblt.bin $FOLDER ; }
[ -e firmware/deliver/openblt.dfu ] && { cp firmware/deliver/openblt.dfu $FOLDER ; }
if [ "$USE_OPENBLT" = "yes" ]; then
  # srec is the only format used by OpenBLT host tools
  cp firmware/build/rusefi_update.srec $FOLDER
fi

if [ -n "$BUNDLE_NAME" ]; then
    mv $FOLDER/rusefi.dfu $FOLDER/rusefi_$BUNDLE_NAME.dfu
fi


[ -e firmware/deliver/rusefi.bin ] || { echo "$SCRIPT_NAME: rusefi.bin not found"; exit 1; }

cd temp


echo "Building bundle"
pwd
zip -r $FULL_BUNDLE_FILE *
[ $? -eq 0 ] || (echo "$SCRIPT_NAME: ERROR INVOKING zip"; exit 1)

echo "$SCRIPT_FILE: Bundle $FULL_BUNDLE_FILE ready"
ls -l $FULL_BUNDLE_FILE

[ -e $FULL_BUNDLE_FILE ] || { echo "$SCRIPT_NAME: ERROR not found $FULL_BUNDLE_FILE"; exit 1; }

if [ -n "$RUSEFI_SSH_USER" ]; then
 echo "$SCRIPT_NAME: Uploading full bundle"
 retVal=0
 if [ "${LTS}" = "true" ]; then
   tar -czf - $FULL_BUNDLE_FILE  | sshpass -p $RUSEFI_SSH_PASS ssh -o StrictHostKeyChecking=no $RUSEFI_SSH_USER@$RUSEFI_SSH_SERVER "mkdir -p ${LTS_FOLDER}; tar -xzf - -C ${LTS_FOLDER}"
   retVal=$?
 else
   tar -czf - $FULL_BUNDLE_FILE  | sshpass -p $RUSEFI_SSH_PASS ssh -o StrictHostKeyChecking=no $RUSEFI_SSH_USER@$RUSEFI_SSH_SERVER "tar -xzf - -C build_server"
   retVal=$?
 fi
 if [ $retVal -ne 0 ]; then
  echo "full bundle upload failed"
  exit 1
 fi
else
  echo "Upload not configured"
fi

cd ..

mkdir -p artifacts
mv temp/$FULL_BUNDLE_FILE artifacts

echo "Removing static content from ${CONSOLE_FOLDER} and $DRIVERS_FOLDER"
rm -rf $CONSOLE_FOLDER/rusefi_autoupdate.exe
rm -rf $CONSOLE_FOLDER/rusefi_console.exe
rm -rf $CONSOLE_FOLDER/DfuSe
rm -rf $CONSOLE_FOLDER/openocd
rm -rf $DRIVERS_FOLDER

# for autoupdate we do not want the unique folder name with timestamp
cd $FOLDER
zip -r ../$UPDATE_BUNDLE_FILE *
cd ..
ls -l $UPDATE_BUNDLE_FILE
if [ -n "$RUSEFI_SSH_USER" ]; then
 retVal=0
 if [ "${LTS}" = "true" ]; then
   tar -czf - $UPDATE_BUNDLE_FILE  | sshpass -p $RUSEFI_SSH_PASS ssh -o StrictHostKeyChecking=no $RUSEFI_SSH_USER@$RUSEFI_SSH_SERVER "mkdir -p ${LTS_FOLDER}/autoupdate; tar -xzf - -C ${LTS_FOLDER}/autoupdate"
   retVal=$?
 else
   tar -czf - $UPDATE_BUNDLE_FILE  | sshpass -p $RUSEFI_SSH_PASS ssh -o StrictHostKeyChecking=no $RUSEFI_SSH_USER@$RUSEFI_SSH_SERVER "mkdir -p build_server/autoupdate; tar -xzf - -C build_server/autoupdate"
   retVal=$?
 fi
 if [ $retVal -ne 0 ]; then
  echo "autoupdate upload failed"
  exit 1
 fi
else
  echo "Upload not configured"
fi
cd ..
mv temp/$UPDATE_BUNDLE_FILE artifacts

echo "$SCRIPT_NAME: We are back in root directory"

pwd
echo "Exiting $SCRIPT_NAME"
