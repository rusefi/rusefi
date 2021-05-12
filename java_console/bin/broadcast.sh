#!/bin/bash
#
# broadcast.sh
# this file is part of rusEFI
#
while true
do
 echo Starting network_connector
 java -jar console/rusefi_console.jar network_connector
 exit_status=$?
 echo Exit code: ${exit_status}
 # in java code that's UPDATE_SBC_EXIT_CODE magic number
 if [ $exit_status -eq 15 ]; then
    echo Connector software update to latest was requested
    bin/update_bundle.sh
 fi
 # in java code that's UPDATE_RELEASE_SBC_EXIT_CODE magic number
 if [ $exit_status -eq 17 ]; then
    echo Connector software update to release was requested
    bin/update_bundle_release.sh
 fi
 # in java code that's UPDATE_FIRMWARE_EXIT_CODE magic number
 if [ $exit_status -eq 16 ]; then
    echo Firmware update was requested
    bin/update_bundle.sh
    # todo: we need to start validating that SBC software matches board type, maybe update bundle type based on existing controller?
    bin/dfu_switch_and_program.sh
 fi
 # in java code that's UPDATE_RELEASE_FIRMWARE_EXIT_CODE magic number
 if [ $exit_status -eq 18 ]; then
    echo Firmware update was requested
    bin/update_bundle_release.sh
    # todo: we need to start validating that SBC software matches board type, maybe update bundle type based on existing controller?
    bin/dfu_switch_and_program.sh
 fi
done