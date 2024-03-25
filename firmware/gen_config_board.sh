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
INI="fome_${SHORT_BOARDNAME}.ini"

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

# work in progress: migrating to fome_${BUNDLE_NAME}.txt
java \
	-DSystemOut.name=logs/gen_config_${SHORT_BOARDNAME} \
	-jar ../java_tools/ConfigDefinition.jar \
	-readfile OUTPUTS_SECTION_FROM_FILE generated/output_channels.ini \
	-readfile DATALOG_SECTION_FROM_FILE generated/data_logs.ini \
	-readfile LIVE_DATA_MENU_FROM_FILE generated/fancy_menu.ini \
	-readfile LIVE_DATA_PANELS_FROM_FILE generated/fancy_content.ini \
	-triggerInputFolder ../unit_tests \
	-with_c_defines false \
	-field_lookup_file generated/value_lookup_generated.cpp generated/value_lookup_generated.md \
	-java_destination ../java_console/models/src/main/java/com/rusefi/config/generated/Fields.java \
	-initialize_to_zero false \
	-signature tunerstudio/generated/signature_${SHORT_BOARDNAME}.txt \
	-signature_destination generated/signature_${SHORT_BOARDNAME}.h \
	-ts_template ./tunerstudio/tunerstudio.template.ini \
	-ts_output_name ./tunerstudio/generated/${INI} \
	-board ${BOARD_DIR} \
	-prepend generated/total_live_data_generated.h \
	-prepend integration/rusefi_config_shared.txt \
	-prepend ${BOARD_DIR}/prepend.txt \
	-definition integration/rusefi_config.txt \
	-enumInputFile controllers/algo/engine_types.h \
	-enumInputFile controllers/algo/rusefi_enums.h \
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
