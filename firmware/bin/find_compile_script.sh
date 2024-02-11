#!/usr/bin/env bash

BOARD_DIR=${1:-$BOARD_DIR}
BUNDLE_NAME=${2:-$BUNDLE_NAME}

COMPILE_SCRIPT="${BOARD_DIR}/compile_${BUNDLE_NAME}.sh"
if [ -f "$COMPILE_SCRIPT" ]; then
  # detailed compile script is useful for instance when same board has multiple MCU targets
  echo "[$COMPILE_SCRIPT] found!" >&2
  echo "$COMPILE_SCRIPT"
else
  echo "Using default script name..." >&2
  echo "${BOARD_DIR}/compile_firmware.sh"
fi
