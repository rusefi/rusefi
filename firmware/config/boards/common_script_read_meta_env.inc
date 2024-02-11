#!/bin/bash

# this script is supposed to be executed from within 'firmware' folder

if [ -z "$1" ]; then
  echo "Read meta.env: The file path was not given as a parameter!"
  exit -3
fi

BOARD_META_PATH=./$1
export BOARD_DIR=${BOARD_META_PATH%/*}

if [ ! -f ${BOARD_META_PATH} ]; then
  echo "Read meta.env: The file was not found!"
  exit -4
fi

if [ ! -f ${BOARD_DIR}/board.mk ]; then
  echo "WARNING! board.mk not found @ ${BOARD_DIR}..."
fi

echo "[META] Reading meta env from ${BOARD_META_PATH}"

export SHORT_BOARD_NAME="$(grep "SHORT_BOARD_NAME=" ${BOARD_META_PATH} | cut -d'=' -f2)"
export PROJECT_CPU="$(grep "BOARD_CPU=" ${BOARD_META_PATH} | cut -d'=' -f2)"
export POST_BUILD_SCRIPT="$(grep "POST_BUILD_SCRIPT=" ${BOARD_META_PATH} | cut -d'=' -f2)"

# todo: is PROJECT_BOARD always the same as SHORT_BOARD_NAME?
export PROJECT_BOARD=${SHORT_BOARD_NAME}

echo "[META] PROJECT_BOARD=${PROJECT_BOARD}"
echo "[META] PROJECT_CPU=${PROJECT_CPU}"
echo "[META] POST_BUILD_SCRIPT=${POST_BUILD_SCRIPT}"
echo "[META] BOARD_DIR=${BOARD_DIR}"
