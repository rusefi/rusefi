#!/bin/bash

echo "I am simulator_test.sh"
pwd

cd java_console

echo "Running simulator functional test"
which java
which javac
ant simulator_test
[ $? -eq 0 ] || { echo "ERROR DETECTED"; exit 1; }

echo "TIMESTAMP $(date "+%a %D %T.%2S")"
echo "simulator_test.sh: done"
