#!/bin/bash

# file gen_config_board.sh

#set -x

echo "This script reads rusefi_config.txt and produces firmware persistent configuration headers"
echo "the storage section of rusefiXXX.ini is updated as well"

if [ -z "$1" ]; then
	echo "Board name parameter expected"
	exit 1
fi

BOARDNAME=$1
SHORT_BOARDNAME=$2

echo "BOARDNAME=${BOARDNAME} SHORT_BOARDNAME=${SHORT_BOARDNAME}"

bash gen_signature.sh ${SHORT_BOARDNAME}

java -DSystemOut.name=gen_config_board \
	-Drusefi.generator.lazyfile.enabled=true \
	-cp ../java_tools/ConfigDefinition.jar \
	com.rusefi.board_generator.BoardReader \
	-yaml config/boards/${BOARDNAME}/mapping.yaml \
	-firmware_path . \
  -output_file tunerstudio/generated/${BOARDNAME}_prefix.txt \
	-enumInputFile controllers/algo/rusefi_enums.h \
	-enumInputFile controllers/algo/rusefi_hw_enums.h

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for ${BOARDNAME}"; exit 1; }

# work in progress: migrating to rusefi_${BUNDLE_NAME}.txt
java -DSystemOut.name=gen_config_board \
	-jar ../java_tools/ConfigDefinition.jar \
	-definition integration/rusefi_config.txt \
	-tool gen_config.sh \
	-ts_destination tunerstudio \
	-cache ${SHORT_BOARDNAME} \
	-cache_zip_file tunerstudio/generated/cache.zip \
	-firing_order controllers/algo/firing_order.h \
	-ts_output_name generated/rusefi_${SHORT_BOARDNAME}.ini \
	-signature tunerstudio/generated/signature_${SHORT_BOARDNAME}.txt \
	-signature_destination controllers/generated/signature_${SHORT_BOARDNAME}.h \
	-enumInputFile controllers/algo/rusefi_enums.h \
	-prepend tunerstudio/generated/${BOARDNAME}_prefix.txt \
	-prepend config/boards/${BOARDNAME}/prepend.txt

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for ${BOARDNAME}"; exit 1; }

exit 0
