#!/bin/bash

#
# switch from one kind of bundle to another, for instance from microRusEFI to Hellen72
#

if [ -z "$1" ]; then
	echo "New bundle name expected"
	exit 1
fi

BUNDLE=$1

echo $BUNDLE > bundle_name.txt


rm -rf rusefi*bin
rm -rf rusefi*hex
rm -rf rusefi*dfu
rm -rf rusefi*ini