#!/bin/bash

SCRIPT_NAME="common_make.sh"
echo "Entering $SCRIPT_NAME"

cd ../../..

mkdir .dep
# todo: start using env variable for number of threads or for '-r'
make -j$(nproc) -r
[ -e build/rusefi.hex ] || { echo "FAILED to compile by $SCRIPT_NAME with $PROJECT_BOARD $DEBUG_LEVEL_OPT and $EXTRA_PARAMS"; exit 1; }

if uname | grep "NT"; then
  HEX2DFU=../misc/encedo_hex2dfu/hex2dfu.exe
else
  HEX2DFU=../misc/encedo_hex2dfu/hex2dfu.bin
fi
chmod u+x $HEX2DFU

mkdir -p deliver

rm -f deliver/rusefi.dfu
echo "$SCRIPT_NAME: invoking hex2dfu for RusEFI"
$HEX2DFU -i build/rusefi.hex -o deliver/rusefi.dfu

# rusEFI console does not use .hex files but for Cypress that's the primary binary format
cp build/rusefi.hex deliver/
cp build/rusefi.bin deliver/
cp build/rusefi.srec deliver/

echo "$SCRIPT_NAME: build folder content:"
ls -l build

echo "$SCRIPT_NAME: deliver folder content:"
ls -l deliver

bash config/boards/clean_env_variables.sh
