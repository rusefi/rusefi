#!/bin/sh

sh misc/jenkins/functional_test_and_build_bundle/build_current_bundle.sh
[ $? -eq 0 ] || { echo "build ERROR DETECTED"; exit 1; }

# sh misc/jenkins/functional_test_and_build_bundle/simulator_test.sh
# [ $? -eq 0 ] || { echo "simulator test ERROR DETECTED"; exit 1; }

# Here we use last version of firmware produced by 'clean_compile_two_versions.bat'

sh misc/jenkins/functional_test_and_build_bundle/hw_test.sh
[ $? -eq 0 ] || { echo "real hardware test ERROR DETECTED"; exit 1; }
