#!/bin/bash

# file gen_config_board.sh
#        for example ./gen_config_board.sh hellen/hellen128 hellen128 rusefi_hellen128mercedes.ini
#                 or ./gen_config_board.sh hellen/hellen128 hellen128
# which is short for ./gen_config_board.sh hellen/hellen128 hellen128 rusefi_hellen128.ini

set -euo pipefail

echo "This script reads rusefi_config.txt and produces firmware persistent configuration headers"
echo "the storage section of rusefiXXX.ini is updated as well"

if [ -z "$1" ]; then
	echo "Board name parameter expected"
	exit 1
fi

BOARD_DIR=$1
SHORT_BOARDNAME=$2
if [ $# -gt 2 ]; then
  INI="$3"
else
  INI="fome_${SHORT_BOARDNAME}.ini"
fi

echo "BOARD_DIR=${BOARD_DIR} SHORT_BOARDNAME=${SHORT_BOARDNAME}"

bash gen_signature.sh ${SHORT_BOARDNAME}

PREPEND_FILE=${BOARD_DIR}/prepend.txt

# Allow the next command to fail, the board may not have a BOARD_SPECIFIC_URL
set +e
BOARD_SPECIFIC_URL=$(cat $PREPEND_FILE | grep MAIN_HELP_URL | cut -d " " -f 3 | sed -e 's/^"//' -e 's/"$//')
set -euo pipefail

echo "BOARD_SPECIFIC_URL=[$BOARD_SPECIFIC_URL] for [$SHORT_BOARDNAME] from [$BOARD_DIR]"
if [ "" = "$BOARD_SPECIFIC_URL" ]; then
  BOARD_SPECIFIC_URL=https://wiki.fome.tech/
fi
echo "BOARD_SPECIFIC_URL=[$BOARD_SPECIFIC_URL]"

source gen_config_common.sh
echo "Using COMMON_GEN_CONFIG [$COMMON_GEN_CONFIG]"

# work in progress: migrating to fome_${BUNDLE_NAME}.txt
java \
 $COMMON_GEN_CONFIG_PREFIX \
 $COMMON_GEN_CONFIG \
	-enumInputFile controllers/algo/rusefi_hw_enums.h \
  -c_defines        generated/rusefi_generated.h \
  -c_destination    generated/engine_configuration_generated_structures.h

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for ${BOARD_DIR}"; exit 1; }

# we generate both versions of the header but only one would be actually included due to conditional compilation see EFI_USE_COMPRESSED_INI_MSD
# todo: make things consistent by
# 0) having generated content not in the same folder with the tool generating content?
# 1) using unique file name for each configuration?
# 2) leverage consistent caching mechanism so that image is generated only in case of fresh .ini. Laziest approach would be to return exit code from java process above
#
hw_layer/mass_storage/create_ini_image.sh            ./tunerstudio/generated/${INI} ./hw_layer/mass_storage/ramdisk_image.h             128 ${SHORT_BOARDNAME} ${BOARD_SPECIFIC_URL}
hw_layer/mass_storage/create_ini_image_compressed.sh ./tunerstudio/generated/${INI} ./hw_layer/mass_storage/ramdisk_image_compressed.h 1088 ${SHORT_BOARDNAME} ${BOARD_SPECIFIC_URL}

echo "Happy ${SHORT_BOARDNAME}!"
exit 0
