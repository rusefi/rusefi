#!/usr/bin/env bash

BOARD_DIR=${1:-$BOARD_DIR}
SHORT_BOARD_NAME=${2:-$SHORT_BOARD_NAME}
INI=${3:-"rusefi_$SHORT_BOARD_NAME.ini"}

if which grealpath >/dev/null 2>&1; then alias realpath='grealpath'; fi
FDIR=$(realpath $(dirname "$0")/..)
BOARD_DIR=$(realpath --relative-to "$FDIR" "$BOARD_DIR")

cd "$FDIR"

PREPEND_FILE=${BOARD_DIR}/prepend.txt

BOARD_SPECIFIC_URL=$(cat $PREPEND_FILE | grep MAIN_HELP_URL | cut -d " " -f 3 | sed -e 's/^"//' -e 's/"$//')

echo "BOARD_SPECIFIC_URL=[$BOARD_SPECIFIC_URL] for [$SHORT_BOARD_NAME] from [$BOARD_DIR]"
if [ "" = "$BOARD_SPECIFIC_URL" ]; then
  BOARD_SPECIFIC_URL=https://rusefi.com/s/wiki
fi
echo "BOARD_SPECIFIC_URL=[$BOARD_SPECIFIC_URL]"

# we generate both versions of the header but only one would be actually included due to conditional compilation see EFI_USE_COMPRESSED_INI_MSD
# todo: make things consistent by
# 0) having generated content not in the same folder with the tool generating content?
# 1) using unique file name for each configuration?
# 2) leverage consistent caching mechanism so that image is generated only in case of fresh .ini. Laziest approach would be to return exit code from java process above
#
hw_layer/mass_storage/create_ini_image.sh            ${META_OUTPUT_ROOT_FOLDER}tunerstudio/generated/${INI} ./hw_layer/mass_storage/ramdisk_image.h             128 ${SHORT_BOARD_NAME} ${BOARD_SPECIFIC_URL}
hw_layer/mass_storage/create_ini_image_compressed.sh ${META_OUTPUT_ROOT_FOLDER}tunerstudio/generated/${INI} ./hw_layer/mass_storage/ramdisk_image_compressed.h 1088 ${SHORT_BOARD_NAME} ${BOARD_SPECIFIC_URL}
