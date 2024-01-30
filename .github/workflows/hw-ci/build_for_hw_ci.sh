#!/usr/bin/env bash

export BOARD_DIR=${1:-$BOARD_DIR}
export SHORT_BOARD_NAME=${2:-$SHORT_BOARD_NAME}

# for instance
# .github/workflows/hw-ci/build_for_hw_ci.sh config/boards/f407-discovery f407-discovery
# .github/workflows/hw-ci/build_for_hw_ci.sh config/boards/proteus        proteus_f4
# .github/workflows/hw-ci/build_for_hw_ci.sh config/boards/proteus        proteus_f7_debug

set -e
echo "HW CI build [$BOARD_DIR][$SHORT_BOARD_NAME]"

cd firmware

./gen_live_documentation.sh
./gen_config_board.sh $BOARD_DIR $SHORT_BOARD_NAME

echo "We aren't guaranteed a clean machine every time, so manually clean the output."
make clean
cd ..

export EXTRA_2_PARAMS=-DHARDWARE_CI

echo Build Firmware
make -j$(nproc) -r
