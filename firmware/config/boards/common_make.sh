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
  [ -e build-openblt/openblt_$PROJECT_BOARD.hex ] || { echo "FAILED to compile OpneBLT by $SCRIPT_NAME with $PROJECT_BOARD"; exit 1; }
fi

if uname | grep "NT"; then
  HEX2DFU=../misc/encedo_hex2dfu/hex2dfu.exe
else
  HEX2DFU=../misc/encedo_hex2dfu/hex2dfu.bin
fi
chmod u+x $HEX2DFU

mkdir -p deliver
rm -f deliver/*

echo "$SCRIPT_NAME: invoking hex2dfu for RusEFI"
$HEX2DFU -i build/rusefi.hex -C 0x1C -o build/rusefi.dfu

if [ $USE_OPENBLT = "yes" ]; then
  # this image is suitable for update through bootloader only
  # do not deliver update images in any format that can confuse users
  #cp build/rusefi.bin  deliver/rusefi_update.bin
  #cp build/rusefi.dfu  deliver/rusefi_update.dfu
  #cp build/rusefi.hex  deliver/rusefi_update.hex
  # srec is the only format used by OpenBLT host tools
  cp build/rusefi.srec deliver/rusefi_update.srec
else
  # standalone images (for use with no bootloader)
  cp build/rusefi.bin  deliver/
  cp build/rusefi.dfu  deliver/
  # rusEFI console does not use .hex files but for Cypress that's the primary binary format
  cp build/rusefi.hex  deliver/
fi

# bootloader and composite image
if [ $USE_OPENBLT = "yes" ]; then
  rm -f deliver/openblt.dfu
  echo "$SCRIPT_NAME: invoking hex2dfu for OpenBLT"
  $HEX2DFU -i build-openblt/openblt_$PROJECT_BOARD.hex -o build-openblt/openblt_$PROJECT_BOARD.dfu

  # do we need all these formats?
  cp build-openblt/openblt_$PROJECT_BOARD.bin  deliver/openblt.bin
  cp build-openblt/openblt_$PROJECT_BOARD.dfu  deliver/openblt.dfu
  #cp build-openblt/openblt_$PROJECT_BOARD.hex  deliver/openblt.hex

  rm -f deliver/rusefi_openblt.dfu
  echo "$SCRIPT_NAME: invoking hex2dfu for composite RusEFI+OpenBLT image"
  $HEX2DFU -i build-openblt/openblt_$PROJECT_BOARD.hex -i build/rusefi.hex -C 0x1C -o deliver/rusefi.dfu -b deliver/rusefi.bin
  #todo: how to create 'signed' hex and srec? Do we need?
fi

echo "$SCRIPT_NAME: build folder content:"
ls -l build

echo "$SCRIPT_NAME: deliver folder content:"
ls -l deliver

bash config/boards/clean_env_variables.sh
