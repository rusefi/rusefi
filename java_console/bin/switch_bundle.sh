#!/bin/bash

#
# switch from one kind of bundle to another, for instance from microRusEFI to Hellen72
#

if [ -z "$1" ]; then
	echo "New bundle name expected"
	exit 1
fi

BUNDLE=$1
CURRENT=${PWD##*/}
CURRENT=${CURRENT:-/}
CURRENT_BRANCH=$(echo "$CURRENT" | cut -d '.' -f 2)
CURRENT_BUNDLE=$(echo "$CURRENT" | cut -d '.' -f 3)
cd ..
mv "rusefi.${CURRENT_BRANCH}.${CURRENT_BUNDLE}" "rusefi.${CURRENT_BRANCH}.${BUNDLE}"
cd "rusefi.${CURRENT_BRANCH}.${BUNDLE}"

rm -rf rusefi*bin
rm -rf rusefi*hex
rm -rf rusefi*dfu
rm -rf rusefi*ini
bash
