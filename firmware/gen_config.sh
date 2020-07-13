#!/bin/bash

#set -x
#TS_PATH="/home/<user>/TunerStudioProjects/"

echo "This batch files reads rusefi_config.txt and produses firmware persistent configuration headers"
echo "the storage section of rusefi.ini is updated as well"

rm gen_config.log
rm gen_config_board.log

if [ -z "${TS_PATH}" ]; then
 echo "TS_PATH not defined"
 # it's nice to have default location
 TS_PATH="${HOMEDRIVE}${HOMEPATH}\Documents\TunerStudioProjects"
fi

echo "This would automatically copy latest file to 'dev' TS projects to ${TS_PATH}"
cp -v tunerstudio/generated/rusefi.ini $TS_PATH/dev/projectCfg/mainController.ini
cp -v tunerstudio/generated/rusefi_mre_f4.ini $TS_PATH/mre_f4/projectCfg/mainController.ini

if [ "$1" = "kinetis" ]; then
  cd config/boards/kinetis/config
  bash gen_config.sh
  [ $? -eq 0 ] || { echo "ERROR generating board kinetis kin"; exit 1; }
else
  bash gen_config_board.sh $1 $2
  [ $? -eq 0 ] || { echo "ERROR generating board $BOARD_NAME $BOARD_SHORT_NAME"; exit 1; }
fi

exit 0
