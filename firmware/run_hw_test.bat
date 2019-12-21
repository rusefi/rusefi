set port=%1
echo "Port (optional): %port%"
java -jar ../java_console_binary/rusefi_console.jar functional_test %port%
