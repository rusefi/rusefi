#!/bin/bash

# see also make_for_valgrind.sh

set -e

valgrind --error-exitcode=1 --exit-on-first-error=yes --leak-check=no --show-error-list=yes build/rusefi_test
