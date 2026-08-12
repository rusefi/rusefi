#!/usr/bin/env bash

# for instance
# .github/workflows/hw-ci/run_hw_ci.sh com.rusefi.HwCiProteus

HW_SUITE=$1
export SIMULATOR_INI_FILE=$2

set -e

./gradlew :autotest:shadowJar

if [ -n "$HARDWARE_CI_SERIAL" ]; then
	for t in /sys/class/tty/tty*/device/../serial; do
	  if [ "$(cat $t)" == "$HARDWARE_CI_SERIAL" ]; then
	    export HARDWARE_CI_SERIAL_DEVICE=/dev/$(echo $t | cut -d '/' -f 5)
			break
		fi
	done
fi

java -cp java_console/autotest/build/libs/autotest-all.jar $HW_SUITE
