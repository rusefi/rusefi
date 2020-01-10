#!/bin/sh

#set -x

echo "This batch files reads rusefi_config.txt and produses firmware persistent configuration headers"
echo "the storage section of rusefi.ini is updated as well"

if [ -z "$1" ]; then
	echo "Board name parameter expected"
	exit 1
fi

BOARDNAME=$1

echo "BOARDNAME=${BOARDNAME}"

echo lazy is broken - TS input is not considered a change
rm build/config.gen

java -DSystemOut.name=gen_config_board \
	-cp ../java_tools/ConfigDefinition.jar:../java_tools/configuration_definition/lib/snakeyaml.jar \
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
	-ts_destination tunerstudio \
	-ts_output_name rusefi_${BOARDNAME}.ini \
	-prepend tunerstudio/${BOARDNAME}_prefix.txt \
	-prepend config/boards/${BOARDNAME}/prepend.txt \
	-skip build/config.gen

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
