rem
rem this takes about 12 minutes to run unfortunatelly, current implementation is pretty inefficient :( See comments around 'nextChart()' method
rem pa
rem

set port=%1
echo "Port (optional): %port%"
java -jar ../java_console_binary/rusefi_console.jar functional_test %port%
