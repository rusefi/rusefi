rem
rem auto-detects connected running rusEfi serial port and send text 'reboot' command
rem flashes DFU
rem

echo Sending rusEFI DFU request
java -jar ../java_console_binary/rusefi_console.jar reboot_dfu
echo Now sleeping before DFU
sleep 5
echo Invoking DFU process
"../misc/install/STM32_Programmer_CLI/bin/STM32_Programmer_CLI.exe" -c port=usb1 -w deliver/rusefi.hex -v -s