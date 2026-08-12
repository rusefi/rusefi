#!/bin/bash

SCRIPT_NAME=$(basename "$0")

# file gen_config_board.sh
#        for example ./gen_config_board.sh config/boards/hellen/hellen128 hellen128
#                    ./gen_config_board.sh config/boards/hellen/hellen-honda-k hellen-honda-k
#                 or ./gen_config_board.sh config/boards/atlas atlas
#                    ./gen_config_board.sh config/boards/proteus proteus_f7
#                    ./gen_config_board.sh config/boards/hellen/uaefi uaefi
#                    ./gen_config_board.sh config/boards/hellen/small-can-board small-can-board
#                    ./gen_config_board.sh config/boards/f407-discovery f407-discovery
#                    ./gen_config_board.sh config/boards/nucleo_f767 nucleo_f767

set -e

echo "This script reads rusefi_config.txt and produces firmware persistent configuration headers"
echo "the storage section of rusefiXXX.ini is updated as well"

if [ -n "$1" ]; then
    BOARD_DIR="$1"
    echo "$SCRIPT_NAME: Argument was provided, BOARD_DIR is now $BOARD_DIR"
fi

if [ -n "$2" ]; then
    SHORT_BOARD_NAME="$2"
    echo "$SCRIPT_NAME: Argument 2 was provided, SHORT_BOARD_NAME is now $SHORT_BOARD_NAME"
fi
if [ -n "$3" ]; then
    INI="$3"
    echo "$SCRIPT_NAME: Argument 3 was provided, INI is now $INI"
else
    # If $3 is empty, construct the default filename using the name we just determined
    INI="rusefi_${SHORT_BOARD_NAME}.ini"
    echo "$SCRIPT_NAME: Argument 3 was NOT provided, using default INI: $INI"
fi

if [ -z "$BOARD_DIR" ]; then
	echo "Board dir parameter expected"
	exit 1
fi

if [ -z "$SHORT_BOARD_NAME" ]; then
	echo "Short board name parameter expected"
	exit 1
fi

echo "$SCRIPT_NAME: BOARD_DIR=${BOARD_DIR} SHORT_BOARD_NAME=${SHORT_BOARD_NAME}"

shopt -s expand_aliases
if which grealpath >/dev/null 2>&1; then alias realpath='grealpath'; fi
FDIR=$(realpath $(dirname "$0"))
BOARD_DIR=$(realpath --relative-to "$FDIR" "$BOARD_DIR")

cd "$FDIR"

source gen_config_common.sh
echo "Using COMMON_GEN_CONFIG [$COMMON_GEN_CONFIG]"

JAVA_REMOTE_DEBUG="-agentlib:jdwp=transport=dt_socket,server=y,suspend=y,address=*:5005"

# in rare cases order of arguments is important - '-tool' should be specified before '-definition'
 #java $JAVA_REMOTE_DEBUG \
java \
 $COMMON_GEN_CONFIG_PREFIX \
 	-tool gen_config.sh \
 $COMMON_GEN_CONFIG \
	-enumInputFile controllers/algo/rusefi_hw_stm32_enums.h \
	-enumInputFile controllers/algo/rusefi_hw_adc_enums.h \
  -c_defines        controllers/generated/rusefi_generated_${SHORT_BOARD_NAME}.h \
  -c_destination    controllers/generated/engine_configuration_generated_structures_${SHORT_BOARD_NAME}.h

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for ${BOARD_DIR}"; exit 1; }


echo "$SCRIPT_NAME: Happy ${SHORT_BOARD_NAME}!"
exit 0
