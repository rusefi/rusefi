#!/usr/bin/env bash

# for instance
# .github/workflows/hw-ci/run_hw_ci.sh com.rusefi.HwCiF4Proteus

HW_SUITE=$1

set -euo pipefail

cd java_tools
./gradlew :autotest:shadowJar
cd ..

java -cp java_console/autotest/build/libs/autotest-all.jar $HW_SUITE