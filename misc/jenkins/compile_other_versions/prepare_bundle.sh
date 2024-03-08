#!/bin/bash

BUNDLE_NAME="$1"
export INI_FILE_OVERRIDE="$2"
SCRIPT_NAME="prepare_bundle.sh"
echo "Entering $SCRIPT_NAME with ${BUNDLE_NAME}"
echo "RUSEFI_BUILD_FTP_USER=$RUSEFI_BUILD_FTP_USER"

TIMESTAMP=$(date "+%Y%m%d_%H%M%S")

# No arg 3 means this is a snapshot "no name", just use the timestamp
if [ -z $3 ] || [ $3 = "null" ]; then
  export FOLDER="temp/fome.snapshot.${TIMESTAMP}.${BUNDLE_NAME}"
else
  export FOLDER="temp/fome.${3}.${BUNDLE_NAME}"
fi

export BUNDLE_FULL_NAME="fome_bundle_${BUNDLE_NAME}"

bash misc/jenkins/build_working_folder.sh
