#!/bin/bash

echo "TIMESTAMP $(date "+%D %T.%2N") Building rusefi simulator"
pwd
cd simulator
gcc -v
make -v
mkdir -p out
rm -rf build
rm -rf .dep
bash compile.sh
ls build
cd ..
[ -e simulator/build/rusefi_simulator.exe ] || [ -e simulator/build/rusefi_simulator ] || { echo "SIMULATOR COMPILATION FAILED"; exit 1; }
