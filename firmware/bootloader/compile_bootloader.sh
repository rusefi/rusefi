#!/bin/bash

SCRIPT_NAME="compile_bootloader.sh"
echo "Entering $SCRIPT_NAME"

# Needed for board overrides
[ $BOOTLOADER_CODE_DESTINATION_PATH ] || { BOOTLOADER_CODE_DESTINATION_PATH=".."; }

BOOTLOADER_CODE_DESTINATION_FILE=$BOOTLOADER_CODE_DESTINATION_PATH/bootloader_generated.hxx

echo "$SCRIPT_NAME: Starting bootloader compilation..."
make -j4 -f src/Makefile $1 $2 $3

[ $? -eq 0 ] || { echo "make compilation failed"; exit 1; }
 
echo "$SCRIPT_NAME: Bootloader build success."

cd blbuild
# Generate a header file with binary bootloader code
rm -f $BOOTLOADER_CODE_DESTINATION_FILE
java -jar ../../../java_tools/bin2header.jar bootloader.bin "$BOOTLOADER_CODE_DESTINATION_FILE" "$BOOTLOADER_COMMENT static const volatile uint8_t bootloader_code[] BOOTLOADER_SECTION"
[ $? -eq 0 ] || { echo "$SCRIPT_NAME: error generating header file"; exit 1; }
cd ..

# Touch 'bootloader_storage.c' to update its modification date (needed for make)
touch bootloader_storage.c

pwd
bash ../config/boards/clean_env_variables.sh
