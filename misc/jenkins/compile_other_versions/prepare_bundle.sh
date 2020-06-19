SCRIPT_NAME="prepare_bundle.sh"
echo "Entering $SCRIPT_NAME with $BUNDLE_NAME"
echo "RUSEFI_BUILD_FTP_USER=$RUSEFI_BUILD_FTP_USER"

if [ -z $RUSEFI_BUILD_FTP_USER ]; then
    echo "RUSEFI_BUILD_FTP_USER not set - not packaging"
    exit 0
fi

TIMESTAMP=$(date "+%Y%m%d_%H%M%S")

export FOLDER="temp/snapshot_$TIMESTAMP_$BUNDLE_NAME_rusefi"

export BUNDLE_FULL_NAME="rusefi_bundle_$BUNDLE_NAME"

sh misc/jenkins/build_working_folder.bat

pwd
echo "exiting $SCRIPT_NAME"
