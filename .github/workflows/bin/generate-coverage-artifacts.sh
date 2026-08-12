#!/bin/bash

# This script generates coverage artifacts for rusEFI unit tests.
# It expects to be run from the unit_tests directory.

set -e

rm -rf gcov_working_area
mkdir -p gcov_working_area/gcov
cd gcov_working_area

# Generate HTML report
gcovr --exclude '.*/googletest/.*' --exclude '.*/unit_tests/.*' -j4 -r ../.. --html-nested --output gcov/index.html

# Generate XML report
gcovr --exclude '.*/googletest/.*' --exclude '.*/unit_tests/.*' -j4 -r ../.. --xml-pretty --output gcov/coverage.xml
