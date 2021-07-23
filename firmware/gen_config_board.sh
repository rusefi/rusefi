#!/bin/bash

# file gen_config_board.sh
#        for example ./gen_config_board.sh hellen/hellen128 hellen128 rusefi_hellen128mercedes.ini
#                 or ./gen_config_board.sh hellen/hellen128 hellen128
# which is short for ./gen_config_board.sh hellen/hellen128 hellen128 rusefi_hellen128.ini

set -e

echo "This script reads rusefi_config.txt and produces firmware persistent configuration headers"
echo "the storage section of rusefiXXX.ini is updated as well"

if [ -z "$1" ]; then
	echo "Board name parameter expected"
	exit 1
fi

BOARDNAME=$1
SHORT_BOARDNAME=$2
if [ -n "$3" ]; then
  INI="$3"
else
  INI="rusefi_${SHORT_BOARDNAME}.ini"
fi

echo "BOARDNAME=${BOARDNAME} SHORT_BOARDNAME=${SHORT_BOARDNAME}"

bash gen_signature.sh ${SHORT_BOARDNAME}

# work in progress: migrating to rusefi_${BUNDLE_NAME}.txt
java -DSystemOut.name=gen_config_board \
	-jar ../java_tools/ConfigDefinition.jar \
	-definition integration/rusefi_config.txt \
	-tool gen_config.sh \
	-ts_destination tunerstudio \
	-cache ${SHORT_BOARDNAME} \
	-cache_zip_file tunerstudio/generated/cache.zip \
	-firing_order controllers/algo/firing_order.h \
	-ts_output_name generated/${INI} \
	-signature tunerstudio/generated/signature_${SHORT_BOARDNAME}.txt \
	-signature_destination controllers/generated/signature_${SHORT_BOARDNAME}.h \
	-enumInputFile controllers/algo/rusefi_enums.h \
	-enumInputFile controllers/algo/rusefi_hw_enums.h \
	-board ${BOARDNAME} \
	-prepend config/boards/${BOARDNAME}/prepend.txt

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for ${BOARDNAME}"; exit 1; }

# todo: make things consistent by
# 0) having generated content not in the same folder with the tool generating content?
# 1) using unique file name for each configuration?
# 2) leverage consistent caching mechanism so that image is generated only in case of fresh .ini. Laziest approach would be to return exit code from java process above
#
hw_layer/mass_storage/create_ini_image.sh ./tunerstudio/generated/${INI} ./hw_layer/mass_storage/ramdisk_image.h
hw_layer/mass_storage/create_ini_image_compressed.sh ./tunerstudio/generated/${INI} ./hw_layer/mass_storage/ramdisk_image_compressed.h

exit 0
