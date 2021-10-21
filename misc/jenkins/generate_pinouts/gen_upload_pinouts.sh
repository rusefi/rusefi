#!/bin/bash

misc/actions/pinouts-create.sh

if [ -n "$RUSEFI_SSH_SERVER" ]; then
  echo "Uploading Pinouts..."
  tar -czf - pinouts | sshpass -p "$RUSEFI_DOXYGEN_SSH_PASS" ssh -o StrictHostKeyChecking=no "$RUSEFI_DOXYGEN_SSH_USER"@"$RUSEFI_SSH_SERVER" "tar -xzf -"
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }

