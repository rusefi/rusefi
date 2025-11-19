#!/usr/bin/env bash

BIN_FILE=$1

SCRIPT_NAME=$(basename "$0")
echo "$SCRIPT_NAME: ${BIN_FILE}"

# search:openblt_version
# set ascii 'BL04' into third reserved DWORD of vector table
# please update BLT_CURRENT_VERSION in C++ when updating here until we improve and make it single source of truth
printf '\x42\x4c\x30\x34' | dd of=${BIN_FILE} bs=1 seek=$((0x24)) count=4 conv=notrunc
