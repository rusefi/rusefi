echo off
set arg1=%1
cd ../console
java -jar rusefi_console.jar send_command %arg1%
