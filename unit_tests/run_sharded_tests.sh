#!/bin/bash

# This script runs every test in its own process (own invocation of rusefi_test executable)
# This allows us to test for accidental cross-test leakage that fixes/breaks something

set -e

export GTEST_TOTAL_SHARDS=500
for IDX in {0..499}
do
	export GTEST_SHARD_INDEX=$IDX
	build/rusefi_test
done

unset GTEST_TOTAL_SHARDS
unset GTEST_SHARD_INDEX
