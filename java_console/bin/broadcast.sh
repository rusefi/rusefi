#!/bin/bash
while true
do
 java -jar console/rusefi_console.jar network_connector
 exit_status=$?
 echo Exit code: ${exit_status}
 # in java code that's UPDATE_SOFTWARE_EXIT_CODE magic number
 if [ $exit_status -eq 15 ]; then
    echo Software update was requested
    bin/update_bundle.sh
 fi
done