#!/bin/bash

echo "java version"
java -version

echo "Generating Java (Antlr)"
cd java_tools/configuration_definition
ant antlr
cd ../..

echo "Building java console"
pwd
cd java_console
ant clean clean_out_folder jar
cd ..

echo "Building TS plugin"
cd java_tools/ts_plugin_launcher
ant clean jar
cd ../..

[ -e java_console_binary/rusefi_console.jar ] || { echo "CONSOLE COMPILATION FAILED"; exit 1; }
echo "java console looks good"
