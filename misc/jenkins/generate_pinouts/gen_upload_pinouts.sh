#!/bin/bash

misc/actions/pinouts-create.sh

if [ -n "$RUSEFI_FTP_SERVER" ]; then
  echo "Uploading Pinouts..."
  ncftpput -r 999 -m -R -z -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" / pinouts
  echo "Uploaded!"
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }

