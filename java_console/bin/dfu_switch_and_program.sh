#!/bin/bash
dfu_file=$(ls *dfu | head -1)
echo dfu_switch_and_program.sh $dfu_file
if [ -z "$dfu_file" ]
then
      echo "No .dfu file(s) found"
      exit 1
fi
echo Found ${dfu_file}
java -jar console/rusefi_console.jar reboot_dfu
java -jar console/rusefi_console.jar dfu ${dfu_file}