#!/usr/bin/env bash

HW_SCRIPT=$1

set -e

# for instance
#
# .github/workflows/hw-ci/ ../.github/workflows/openocd_ci_f4_discovery.cfg
# .github/workflows/hw-ci/ ../.github/workflows/openocd_ci_proteus.cfg

openocd -f "$HW_SCRIPT" -c init -c targets -c "reset halt" -c "flash erase_sector 0 0 11" -c "flash write_image "deliver/rusefi.bin" 0x08000000" -c "reset run" -c "shutdown"