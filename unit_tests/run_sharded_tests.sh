#!/bin/bash

# This script runs every test in its own process (own invocation of fome_test executable)
# This allows us to test for accidental cross-test leakage that fixes/breaks something

set -e

export GTEST_TOTAL_SHARDS=600
for IDX in {0..599}
do
	export GTEST_SHARD_INDEX=$IDX
	build/fome_test
done

unset GTEST_TOTAL_SHARDS
unset GTEST_SHARD_INDEX
