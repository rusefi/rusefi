#!/bin/bash

echo "java version"
java -version

echo "Generating Java (Antlr)"
cd android
./gradlew :config_definition:generateGrammarSource
cd ..

echo "Building java console"
pwd
cd java_console
ant clean clean_out_folder jar
cd ..

[ -e java_console_binary/rusefi_console.jar ] || { echo "CONSOLE COMPILATION FAILED"; exit 1; }
echo "java console looks good"

echo "Building TS plugin"
cd android
./gradlew :ts_plugin_launcher:shadowJar
cd ..

[ -e java_tools/ts_plugin_launcher/build/jar/rusefi_ts_plugin_launcher.jar ] || { echo "PLUGIN COMPILATION FAILED"; exit 1; }
echo "TS plugin looks good"
