#!/bin/bash

# This script runs unit tests with coverage and generates HTML and JSON reports
# It uses a local virtual environment for gcovr as suggested in misc/actions/install-gcovr.sh

set -e

# Navigate to unit_tests directory
cd "$(dirname "$0")"

# 1. Setup virtual environment for gcovr if it doesn't exist
if [ ! -d "venv" ]; then
    echo "Creating virtual environment..."
    python3 -m venv venv
fi

# 2. Install gcovr if not present
if [ ! -f "venv/bin/gcovr" ]; then
    echo "Installing gcovr..."
    ./venv/bin/pip install gcovr
fi

# 3. Clean and Build unit tests with coverage enabled
JOBS=$(nproc)
echo "Building unit tests with coverage using $JOBS cores..."
# make clean
make -j$JOBS COVERAGE=yes

# 4. Run unit tests
echo "Running unit tests..."
./build/rusefi_test

# 5. Generate coverage reports
echo "Generating coverage reports..."
mkdir -p coverage_reports

source ./coverage_common.sh

# HTML report
./venv/bin/gcovr -r ../ --html-details -o coverage_reports/coverage.html $GCOVR_EXCLUDES

# JSON report
./venv/bin/gcovr -r ../ --json -o coverage_reports/coverage.json $GCOVR_EXCLUDES

echo "Coverage reports generated in unit_tests/coverage_reports/"
echo "HTML: unit_tests/coverage_reports/coverage.html"
echo "JSON: unit_tests/coverage_reports/coverage.json"
