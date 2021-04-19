#!/bin/bash

pwd
cd firmware

doxygen || { echo "doxygen run FAILED"; exit 1; }

# http://www.ncftp.com/download/
# actually Cygwin http://rusefi.com/wiki/index.php?title=Internal:Software:Build_Server
cd ../doxygen
if [ -n "$RUSEFI_FTP_SERVER" ]; then
  echo "Uploading Doxygen"
  ncftpput -R -z -m -V -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" / html
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }
