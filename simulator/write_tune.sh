#!/bin/bash

# [CannedTunes]
set -e
if [ -z "$1" ]
then
	echo ".ini file argument is expected"
	exit -1
fi
java -cp ../java_tools/tune-tools/build/libs/tune-tools-all.jar com.rusefi.tools.tune.WriteSimulatorConfiguration $1
