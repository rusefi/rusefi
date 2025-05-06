#!/bin/bash

# see also run_with_valgrind.sh

set -e

make -j$(nproc) SANITIZE=no
