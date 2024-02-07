#!/bin/bash

echo "java version"
java -version

echo "Building java console"
pwd
cd java_tools
./gradlew :ui:shadowJar
cd ..

[ -e java_console_binary/rusefi_console.jar ] || { echo "CONSOLE COMPILATION FAILED"; exit 1; }
echo "java console looks good"

cd java_tools
./gradlew :autoupdate:jar
cd ..

[ -e java_console_binary/rusefi_autoupdate.jar ] || { echo "AUTO-UPDATE COMPILATION FAILED"; exit 1; }
echo "auto-update looks good"

# this is a bit dead
#not really needed in 2024
#echo "Building TS plugin"
#cd java_tools
#./gradlew :ts_plugin_launcher:shadowJar
#cd ..

#[ -e java_tools/ts_plugin_launcher/build/jar/rusefi_ts_plugin_launcher.jar ] || { echo "PLUGIN COMPILATION FAILED"; exit 1; }
#echo "TS plugin looks good"
