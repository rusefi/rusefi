#!/bin/bash

echo "This script reads rusefi_config.txt and produces firmware persistent configuration headers"
echo "The storage section of rusefi_xxx.ini is updated as well"

rm -f gen_config.log
rm -f gen_config_board.log

cd ..
# gen_live_documentation.sh (run per board below) needs config_definition_base-all.jar and
# enum_to_string-all.jar in addition to the main config_definition-all.jar
./gradlew :config_definition:shadowJar :config_definition_base:shadowJar :enum_to_string:shadowJar
[ $? -eq 0 ] || { echo "ERROR building code generation jars"; exit 1; }
cd firmware

genConfig ()
{
	source config/boards/common_script_read_meta_env.inc $1 >/dev/null

	if [ -n "$CUSTOM_GEN_CONFIG" ]; then
		bash $CUSTOM_GEN_CONFIG
	else
		# [tag:live_data_board_prepend] live data outputs feed this board's .ini, so they are
		# regenerated with this board's prepend (BOARD_DIR is exported by the meta env reader)
		bash gen_live_documentation.sh
		[ $? -eq 0 ] || { echo "ERROR generating live data for $1"; exit 1; }
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

# [tag:live_data_board_prepend] the shared live data outputs (headers, java, .ini fragments)
# must not keep the last board's overrides: regenerate them without any board prepend
env -u BOARD_DIR bash gen_live_documentation.sh
[ $? -eq 0 ] || { echo "ERROR generating default live data"; exit 1; }
