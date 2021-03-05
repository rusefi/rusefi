#!/bin/bash

git submodule update --init
bash misc/jenkins/functional_test_and_build_bundle/build_current_bundle.sh
[ $? -eq 0 ] || { echo "build ERROR DETECTED"; exit 1; }

# bash misc/jenkins/functional_test_and_build_bundle/simulator_test.sh
# [ $? -eq 0 ] || { echo "simulator test ERROR DETECTED"; exit 1; }

# Here we use last version of firmware produced by 'clean_compile_two_versions.bat'

if [ ! "$RUSEFI_SKIP_HW" ] || [ "$RUSEFI_SKIP_HW" = "false" ]; then
 sh misc/jenkins/functional_test_and_build_bundle/hw_test.sh
 [ $? -eq 0 ] || { echo "real hardware test ERROR DETECTED"; exit 1; }
fi
