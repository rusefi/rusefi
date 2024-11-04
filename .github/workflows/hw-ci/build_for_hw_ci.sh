#!/usr/bin/env bash

HW_FOLDER=$1
HW_TARGET=$2

# for instance
# .github/workflows/hw-ci/build_for_hw_ci.sh config/boards/f407-discovery f407-discovery
# .github/workflows/hw-ci/build_for_hw_ci.sh config/boards/proteus        proteus_f4
# .github/workflows/hw-ci/build_for_hw_ci.sh config/boards/proteus        proteus_f7
# .github/workflows/hw-ci/build_for_hw_ci.sh config/boards/proteus        proteus_f7_debug
# .github/workflows/hw-ci/build_for_hw_ci.sh config/boards/nucleo_f767 nucleo_f767

set -e
echo "[build_for_hw_ci.sh] enter [$HW_FOLDER][$HW_TARGET]"

cd firmware

export BOARD_META_PATH=$(bash bin/find_meta_info.sh ${HW_FOLDER} ${HW_TARGET})
source config/boards/common_script_read_meta_env.inc "${BOARD_META_PATH}"

echo "[build_for_hw_ci.sh] We aren't guaranteed a clean machine every time, so manually clean the output."
make clean

export EXTRA_2_PARAMS=-DHARDWARE_CI

echo "[build_for_hw_ci.sh] Building Firmware"
make -j$(nproc) -r deliver/rusefi.bin

echo "[build_for_hw_ci.sh] Done!"
