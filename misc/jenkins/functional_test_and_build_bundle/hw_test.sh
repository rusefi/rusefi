#!/bin/bash

echo "TIMESTAMP $(date "+%a %D %T.%2S")"
SCRIPT_NAME="hw_test.sh"
echo "Entering $SCRIPT_NAME"
pwd

cd firmware
echo "$SCRIPT_NAME: erasing first"
bash flash_erase407.sh
echo "$SCRIPT_NAME: trying to flash"
# This script depends on someone else building firmware
bash flash_openocd407.sh
[ $? -eq 0 ] || { echo "ERROR invoking flash_openocd407.sh"; exit 1; }

[ -e build/rusefi.bin ] || { echo "FIRMWARE NOT FOUND"; exit 1; }

pwd

cd ..

# echo Running some commands
# pwd
# java -cp java_console_binary\rusefi_console.jar com.rusefi.CmdLine "set_led_blinking_period 10"

cd java_console

echo "Running tests"
which java
which javac
echo Sleeping few seconds to give OS time to connect VCP driver
sleep 20s
ant hardware_ci_f4_discovery

[ $? -eq 0 ] || { echo "ERROR DETECTED"; exit 1; }

echo "TIMESTAMP $(date "+%a %D %T.%2S")"
pwd
echo "exiting $SCRIPT_NAME"
