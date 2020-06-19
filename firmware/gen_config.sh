#!/bin/sh

#set -x
#TS_PATH="/home/<user>/TunerStudioProjects/"

echo "This batch files reads rusefi_config.txt and produses firmware persistent configuration headers"
echo "the storage section of rusefi.ini is updated as well"

rm gen_config.log
rm gen_config_board.log

mkdir build

java $(< ../gen_config.conf)

[ $? -eq 0 ] || (echo "ERROR generating"; exit $?)

if [ -z "${TS_PATH}" ]; then
 echo "TS_PATH not defined"
else
 echo "This would automatically copy latest file to 'dev' TS project at ${TS_PATH}"
 cp -v tunerstudio/rusefi.ini $TS_PATH/dev/projectCfg/mainController.ini
 cp -v tunerstudio/rusefi_microrusefi.ini $TS_PATH/dev_mre/projectCfg/mainController.ini
fi

for BOARD in "microrusefi" "frankenso" "prometheus" "proteus"; do
 sh gen_config_board.sh $BOARD
 [ $? -eq 0 ] || (echo "ERROR generating $BOARD"; exit $?)
done

cd config/boards/kinetis/config
sh gen_config.sh

exit 0
