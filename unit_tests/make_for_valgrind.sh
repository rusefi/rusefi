#!/bin/bash

# Valgrind is a memory analysis tool that is incompatible with AddressSanitizer (SANITIZE=yes).
# To run Valgrind, we must rebuild the unit tests without sanitizers.
#
# AddressSanitizer ASan is another runtime memory error detector
#
# see also run_with_valgrind.sh

set -e

make -j$(nproc) SANITIZE=no
