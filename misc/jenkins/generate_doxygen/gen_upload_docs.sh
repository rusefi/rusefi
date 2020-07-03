#!/bin/sh

echo "Should be executed from project root folder. Will try to upload to $RUSEFI_FTP_SERVER"

# ibom is part of Doxygen job simply in order to reduce workspace HDD usage on my tiny build server
sh misc/jenkins/InteractiveHtmlBom/run.sh
echo "Uploading IBOMs"
ncftpput -R -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" /ibom hardware/ibom/*

pwd
cd firmware

doxygen || { echo "doxygen run FAILED"; exit 1; }

# http://www.ncftp.com/download/
# actually Cygwin http://rusefi.com/wiki/index.php?title=Internal:Software:Build_Server
cd ../doxygen
echo "Uploading Doxygen"
ncftpput -R -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" /html html/*
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }
