#!/bin/bash

pwd
cd firmware

doxygen || { echo "doxygen run FAILED"; exit 1; }

# actually Cygwin http://rusefi.com/wiki/index.php?title=Internal:Software:Build_Server
cd ../doxygen
if [ -n "$RUSEFI_SSH_SERVER" ]; then
  echo "Uploading Doxygen"
  tar -czf - html | sshpass -p "$RUSEFI_DOXYGEN_SSH_PASS" ssh -o StrictHostKeyChecking=no "$RUSEFI_DOXYGEN_SSH_USER"@"$RUSEFI_SSH_SERVER" "tar -xzf -"
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }
