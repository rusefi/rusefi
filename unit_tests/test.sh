#!/bin/bash

set -e

make -j$(nproc)

TEST_NAME=$1

if [ -n "$TEST_NAME" ]; then
    build/rusefi_test --gtest_filter="*$TEST_NAME*"
else
    build/rusefi_test
fi
