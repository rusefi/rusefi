#!/bin/bash

# fail on error!
set -e

cd firmware
bash clean.sh

echo "Sourcing $COMPILE_SCRIPT"
source $COMPILE_SCRIPT
bash config/boards/common_script.sh
