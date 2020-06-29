#!/bin/sh

echo "TIMESTAMP $(date "+%D %T.%2N") Building rusefi simulator"
pwd
cd simulator
gcc -v
make -v
mkdir out
rm -rf build
rm -rf .dep
sh compile.sh
cd ..
[ -e simulator/build/rusefi_simulator.exe ] || { echo "SIMULATOR COMPILATION FAILED"; exit -1; }
