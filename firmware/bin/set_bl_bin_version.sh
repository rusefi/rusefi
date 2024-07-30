#!/usr/bin/env bash

BIN_FILE=$1

SCRIPT_NAME=$(basename "$0")
echo "$SCRIPT_NAME: ${BIN_FILE}"

# search:openblt_version
# set ascii 'BL01' into third reserved DWORD of vector table
printf '\x42\x4c\x30\x31' | dd of=${BIN_FILE} bs=1 seek=$((0x24)) count=4 conv=notrunc
