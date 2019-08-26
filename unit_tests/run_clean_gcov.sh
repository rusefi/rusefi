#!/bin/bash

make clean
make -j4
build/rusefi_test
./run_gcov.sh