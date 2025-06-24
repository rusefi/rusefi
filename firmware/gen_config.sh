#!/bin/bash

echo "This script reads rusefi_config.txt, *.activeConfiguration.txt and *.persistentConfiguration.txt and produces firmware persistent configuration headers"
echo "The storage section of rusefi_xxx.ini is updated as well"

rm -f gen_config.log
rm -f gen_config_board.log
rm -f integration/rusefi_config.generated.txt
# base file:
cp integration/rusefi_config.txt integration/rusefi_config.generated.txt

# first find all *.activeConfiguration.txt & *.persistentConfiguration.txt
for activeConfigurationFile in **/*.activeConfiguration.txt; do
    echo processing $activeConfigurationFile
    printf "%s\n" "/include_file@@BOARD_ENGINE_CONFIGURATION_FROM_FILE@@/r $activeConfigurationFile" w | ed -s integration/rusefi_config.generated.txt
done

for persistenConfigurationFile in **/*.persistentConfiguration.txt; do
    echo processing persistenConfigurationFile
    printf "%s\n" "/include_file@@BOARD_CONFIG_FROM_FILE@@/r $persistenConfigurationFile" w | ed -s integration/rusefi_config.generated.txt
done

cd ../java_tools
./gradlew :config_definition:shadowJar
cd ../firmware

genConfig ()
{
	source config/boards/common_script_read_meta_env.inc $1 >/dev/null

	if [ -n "$CUSTOM_GEN_CONFIG" ]; then
		bash $CUSTOM_GEN_CONFIG
	else
		bash gen_signature.sh ${SHORT_BOARD_NAME}
		[ $? -eq 0 ] || { echo "ERROR generating signature for $1"; exit 1; }
		bash gen_config_board.sh $BOARD_DIR $SHORT_BOARD_NAME
		[ $? -eq 0 ] || { echo "ERROR generating configs for $1"; exit 1; }
		bash bin/gen_image_board.sh $BOARD_DIR $SHORT_BOARD_NAME
		[ $? -eq 0 ] || { echo "ERROR generating images for $1"; exit 1; }
	fi
}

find config/boards -name "meta-info*.env" -print0 | while IFS= read -r -d '' f; do
	echo -n "$(genConfig $f)"
done
