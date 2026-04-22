#!/bin/bash

#
# ci_gcov.sh - Generate code coverage reports for rusEFI unit tests
#
# This script is invoked by GitHub Actions (see .github/workflows/build-unit-tests.yaml)
# after unit tests have been compiled and executed with gcov instrumentation enabled.
#
# It uses gcovr to process the coverage data produced by gcc/g++ and generates
# an HTML coverage report.
#
# Prerequisites:
#   - Unit tests must have been built and run with --coverage flags prior to invoking this script.
#   - gcovr must be installed (pip install gcovr).
#
# Output:
#   - gcov_working_area/gcov/index.html — nested HTML coverage report.
#
# Usage:
#   ./ci_gcov.sh
#

# Clean up any previous coverage working directory
rm -rf gcov_working_area

# Create a fresh working area and output directory
mkdir gcov_working_area
cd gcov_working_area
mkdir gcov

echo -e "\nGenerating rusEFI unit test coverage"

source ../coverage_common.sh

# Run gcovr to collect coverage data and produce an HTML report.
#   --exclude-throw-branches   : exclude branches caused by throw/catch from branch coverage
#   --exclude-unreachable-branches : exclude compiler-generated unreachable branches
#   --decisions                : enable decision (MC/DC-like) coverage analysis
#   --merge-mode-functions=separate : keep separate entries for same-named functions
#   --exclude '/.*/googletest/': skip Google Test framework sources
#   -j4                        : use 4 parallel threads for processing
#   -r ../..                   : repository root (two levels up from gcov_working_area)
#   --html-nested              : generate a nested HTML report with per-directory pages
#
# For debug use --html-details --html-single-page --verbose to generate a single html
gcovr --exclude-throw-branches --exclude-unreachable-branches --decisions --merge-mode-functions=separate \
      $GCOVR_EXCLUDES \
      -j4  -r ../.. --html-nested gcov/index.html
