rem
rem auto-detects connected running rusEfi serial port and send text 'reboot' command
rem flashes DFU
rem

echo Sending rusEFI DFU request
java -jar ../console/rusefi_console.jar reboot_dfu
echo Now sleeping before DFU
sleep 5
echo Invoking DFU process
call flash_dfu.bat
