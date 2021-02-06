#!/bin/bash

pwd
cd firmware

doxygen || { echo "doxygen run FAILED"; exit 1; }

# http://www.ncftp.com/download/
# actually Cygwin http://rusefi.com/wiki/index.php?title=Internal:Software:Build_Server
cd ../doxygen
if [ -n "$RUSEFI_FTP_SERVER" ]; then
  echo "Uploading Doxygen"
  tar -cvzf - html |\
    sshpass -p $RUSEFI_DOXYGEN_FTP_PASS ssh -o StrictHostKeyChecking=no $RUSEFI_DOXYGEN_FTP_USER@$RUSEFI_FTP_SERVER \
      "(tar -xzvf - -C ~)"
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }
