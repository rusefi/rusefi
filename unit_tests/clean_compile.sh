#!/bin/bash

echo "Compiling unit tests"
make clean
# todo: how comes .dep is not being cleaned by 'make clean'?
rm -rf .dep/
make -j$(nproc)
