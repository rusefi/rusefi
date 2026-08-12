#!/bin/bash

echo "This script reads rusefi_config.txt and produces firmware persistent configuration headers"
echo "The storage section of rusefi_xxx.ini is updated as well"

rm -f gen_config.log
rm -f gen_config_board.log

cd ..
./gradlew :config_definition:shadowJar
cd firmware

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
