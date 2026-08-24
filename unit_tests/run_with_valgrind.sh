#!/bin/bash

# Valgrind is a memory analysis tool that is incompatible with AddressSanitizer (SANITIZE=yes).
# The code must be compiled with SANITIZE=no (see make_for_valgrind.sh) before running this script.
# see also make_for_valgrind.sh

set -e

valgrind --error-exitcode=1 --exit-on-first-error=yes --leak-check=no --show-error-list=yes build/rusefi_test
