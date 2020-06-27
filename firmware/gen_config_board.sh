#!/bin/sh

#set -x

echo "This batch files reads rusefi_config.txt and produses firmware persistent configuration headers"
echo "the storage section of rusefi.ini is updated as well"

if [ -z "$1" ]; then
	echo "Board name parameter expected"
	exit 1
fi

BOARDNAME=$1
SHORT_BOARDNAME=$2

echo "BOARDNAME=${BOARDNAME} SHORT_BOARDNAME=${SHORT_BOARDNAME}"

sh gen_signature.sh ${SHORT_BOARDNAME}

java -DSystemOut.name=gen_config_board \
	-Drusefi.generator.lazyfile.enabled=true \
	-cp ../java_tools/ConfigDefinition.jar \
	com.rusefi.board_generator.BoardReader \
	-board ${BOARDNAME} \
	-firmware_path . \
	-out tunerstudio \
	-enumInputFile controllers/algo/rusefi_enums.h \
	-enumInputFile controllers/algo/rusefi_hw_enums.h

[ $? -eq 0 ] || (echo "ERROR generating TunerStudio config for ${BOARDNAME}"; exit $?)

java -DSystemOut.name=gen_config_board \
	-jar ../java_tools/ConfigDefinition.jar \
	-definition integration/rusefi_config.txt \
	-tool gen_config.sh \
	-ts_destination tunerstudio \
	-firing_order controllers/algo/firing_order.h \
	-ts_output_name rusefi_${BOARDNAME}.ini \
	-signature tunerstudio/signature_${SHORT_BOARDNAME}.txt \
	-prepend tunerstudio/${BOARDNAME}_prefix.txt \
	-prepend config/boards/${BOARDNAME}/prepend.txt

[ $? -eq 0 ] || (echo "ERROR generating TunerStudio config for ${BOARDNAME}"; exit $?)

if [ -z "${TS_PATH}" ]; then
	echo "TS_PATH not defined"
else
	if [ -d "${TS_PATH}/dev_${BOARDNAME}/" ]; then
		echo "This would automatically copy latest file to 'dev_${BOARDNAME}' TS project $TS_PATH"
		cp -v tunerstudio/rusefi_microrusefi.ini ${TS_PATH}/dev_${BOARDNAME}/projectCfg/mainController.ini
	fi
fi

exit 0
