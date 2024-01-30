echo "$SCRIPT_NAME: invoking hex2dfu for no-blt rusEFI image"
$HEX2DFU -i build/rusefi.hex $CONTROL_SUM_OPTION -o build/rusefi.dfu

# standalone images (for use with no bootloader)
cp build/rusefi.bin  deliver/
cp build/rusefi.dfu  deliver/
cp build/rusefi.hex  deliver/
if [ "$INCLUDE_ELF" = "yes" ]; then
 # we definitely need .elf .map .list
 cp build/rusefi.*  deliver/
fi
