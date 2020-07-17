#!/bin/bash

SCRIPT_NAME="prepare_bundle.sh"
echo "Entering $SCRIPT_NAME with ${1}"
echo "RUSEFI_BUILD_FTP_USER=$RUSEFI_BUILD_FTP_USER"

TIMESTAMP=$(date "+%Y%m%d_%H%M%S")

export FOLDER="temp/snapshot_${TIMESTAMP}_${1}_rusefi"

export BUNDLE_FULL_NAME="rusefi_bundle_${1}"

bash misc/jenkins/build_working_folder.sh

pwd
echo "exiting $SCRIPT_NAME"
