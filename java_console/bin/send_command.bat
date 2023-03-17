echo off
set arg1=%1
cd ../console
java -jar fome_console.jar send_command %arg1%
