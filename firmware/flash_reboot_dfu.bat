rem
rem auto-detects connected running rusEfi serial port and send text 'reboot' command
rem flashes DFU
rem

echo Sending rusEFI DFU request
java -jar ../java_console_binary/rusefi_console.jar reboot_dfu
echo Now sleeping before DFU
sleep 10
echo Invoking DFU process
..\misc\install\DfuSe\DfuSeCommand.exe -c -d --fn deliver/rusefi.dfu