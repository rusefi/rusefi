#!/bin/bash
dfu_file=$(ls *dfu | head -1)
if [ -z "$dfu_file" ]
then
      echo "No .dfu file(s) found"
      exit 1
fi
echo Found ${dfu_file}
java -jar console/rusefi_console.jar reboot_dfu
java -jar console/rusefi_console.jar dfu ${dfu_file}