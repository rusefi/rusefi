#!/bin/bash

# this script is supposed to be executed from within 'firmware' folder

if [ -z "$1" ]; then
  echo "Read meta.env: The file path was not given as a parameter!"
  exit -3
fi

BOARD_META_PATH=./$1


if [ -z "${BOARD_DIR}" ]; then
    # magical version of BOARD_DIR=$(dirname "$BOARD_META_PATH")
    export BOARD_DIR="${BOARD_META_PATH%/*}"
    echo "common_script_read_meta_env.inc: BOARD_DIR was empty. Setting to fallback: $BOARD_DIR"
fi

if [ ! -f ${BOARD_META_PATH} ]; then
  echo "Read $BOARD_META_PATH: The file was not found!"
  exit -4
fi

if [ ! -f ${BOARD_DIR}/board.mk ]; then
  echo "WARNING! board.mk not found @ ${BOARD_DIR}..."
fi

echo "[META] Reading meta env from ${BOARD_META_PATH}"

VARS=$(grep "=" "$BOARD_META_PATH")

while IFS= read -r L; do
		# get the key and delete all spaces
    K=$(echo "$L" | cut -d '=' -f "-1" | tr -d ' ')
		# get the value and delete any leading space
    V=$(echo "$L" | cut -d '=' -f "2-" | awk '{$1=$1};1')
		# put them together, closing the value in quotes
		export $K="$V"
done <<< "$VARS"

if [ -z "$BUNDLE_NAME" ]; then
	BUNDLE_NAME=$(echo "$BOARD_META_PATH" | sed -n 's/.*meta-info-\(.*\)\.env/\1/p')
	if [ -z "$BUNDLE_NAME" ]; then
		export BUNDLE_NAME="$SHORT_BOARD_NAME"
	else
		export BUNDLE_NAME
	fi
fi

echo "[META] PROJECT_BOARD=${PROJECT_BOARD}"
echo "[META] PROJECT_CPU=${PROJECT_CPU}"
echo "[META] POST_BUILD_SCRIPT=${POST_BUILD_SCRIPT}"
echo "[META] BOARD_DIR=${BOARD_DIR}"
echo "[META] USE_OPENBLT=${USE_OPENBLT}"
