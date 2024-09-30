#!/usr/bin/env bash

BOARD_DIR=${1:-$BOARD_DIR}
BUNDLE_NAME=${2:-$BUNDLE_NAME}

META_INFO="${BOARD_DIR}/meta-info-${BUNDLE_NAME}.env"
if [ -f "$META_INFO" ]; then
  echo "[$META_INFO] found!" >&2
  echo "$META_INFO"
else
  echo "Using default meta-info.env." >&2
  echo "${BOARD_DIR}/meta-info.env"
fi
