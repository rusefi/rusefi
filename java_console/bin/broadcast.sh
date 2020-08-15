#!/bin/bash
while true
do
 java -jar console/rusefi_console.jar network_connector
 exit_status=$?
 echo Exit code: ${exit_status}
done