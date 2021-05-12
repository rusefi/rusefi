rem
rem auto-detects connected running rusEfi serial port and send text 'reboot' command
rem flashes DFU
rem

java -jar ../java_console_binary/rusefi_console.jar reboot_dfu
sleep 10
..\misc\install\DfuSe\DfuSeCommand.exe -c -d --fn deliver/rusefi.dfu