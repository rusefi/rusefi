#!/bin/bash

SCRIPT_NAME="common_make.sh"
echo "Entering $SCRIPT_NAME"

cd ../../..

mkdir .dep
# todo: start using env variable for number of threads or for '-r'
make -j$(nproc) -r
[ -e build/rusefi.hex ] || { echo "FAILED to compile by $SCRIPT_NAME with $PROJECT_BOARD $DEBUG_LEVEL_OPT and $EXTRA_PARAMS"; exit 1; }
if [ $USE_OPENBLT = "yes" ]; then
  make openblt
  [ -e build/openblt_$PROJECT_BOARD.hex ] || { echo "FAILED to compile OpneBLT by $SCRIPT_NAME with $PROJECT_BOARD"; exit 1; }
fi

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

# bootloader
if [ $USE_OPENBLT = "yes" ]; then
  rm -f deliver/openblt_$PROJECT_BOARD.dfu
  echo "$SCRIPT_NAME: invoking hex2dfu for OpenBLT"
  $HEX2DFU -i build/openblt_$PROJECT_BOARD.hex -o deliver/openblt_$PROJECT_BOARD.dfu

  cp build/openblt_$PROJECT_BOARD.hex deliver/
  cp build/openblt_$PROJECT_BOARD.bin deliver/
  cp build/openblt_$PROJECT_BOARD.srec deliver/

  rm -f deliver/rusefi_openblt.dfu
  echo "$SCRIPT_NAME: invoking hex2dfu for composite RusEFI+OpenBLT image"
  $HEX2DFU -i build/openblt_$PROJECT_BOARD.hex -i build/rusefi.hex -o deliver/rusefi_openblt.dfu
fi

echo "$SCRIPT_NAME: build folder content:"
ls -l build

echo "$SCRIPT_NAME: deliver folder content:"
ls -l deliver

bash config/boards/clean_env_variables.sh
