#!/bin/sh

echo "Compiling unit tests"
rm -rf .dep/
rm -rf build/
make -j4
