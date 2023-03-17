#!/bin/bash

echo "java version"
java -version

echo "Generating Java (Antlr)"
cd java_tools
./gradlew :config_definition:generateGrammarSource
cd ..

echo "Building java console"
pwd
cd java_console
ant clean clean_out_folder jar
cd ..

[ -e java_console_binary/fome_console.jar ] || { echo "CONSOLE COMPILATION FAILED"; exit 1; }
echo "java console looks good"
