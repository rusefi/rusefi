#!/bin/bash

# fail on error!
set -e

cd firmware
bash clean.sh

bash config/boards/common_script.sh "${BOARD_META_PATH}"
