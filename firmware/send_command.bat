rem
rem auto-detects connected running rusEfi serial port and send text 'reboot' command
rem 

set command=%1
echo "Command: [%command%]"

java -jar ../java_console_binary/rusefi_console.jar send_command %command%
