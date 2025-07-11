#!/bin/bash

#
# this script is used by github actions
#
cd gcov_working_area
echo -e "\nUploading HTML"
tar -czf - -C gcov . | sshpass -p "$RUSEFI_SSH_PASS" ssh -o StrictHostKeyChecking=no "$RUSEFI_SSH_USER"@"$RUSEFI_SSH_SERVER" "tar -xzf - -C docs/unit_tests_coverage"
echo -e "\nHappy End."