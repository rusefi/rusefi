#!/bin/bash

set -e

# macOS has no nproc and its default 'make' is BSD make; prefer GNU tools
# (brew install make coreutils)
if command -v gmake >/dev/null 2>&1; then
	MAKE=gmake
else
	MAKE=make
fi

if command -v nproc >/dev/null 2>&1; then
	JOBS=$(nproc)
elif command -v gnproc >/dev/null 2>&1; then
	JOBS=$(gnproc)
else
	JOBS=$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
fi

$MAKE -j$JOBS

TEST_NAME=$1

if [ -n "$TEST_NAME" ]; then
    build/rusefi_test --gtest_filter="*$TEST_NAME*"
else
    build/rusefi_test
fi
