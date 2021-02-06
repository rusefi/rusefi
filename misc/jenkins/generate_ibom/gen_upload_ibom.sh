#!/bin/bash

echo "Should be executed from project root folder. Will try to upload to $RUSEFI_FTP_SERVER"
pwd
# ibom is part of Doxygen job simply in order to reduce workspace HDD usage on my tiny build server
bash misc/jenkins/InteractiveHtmlBom/run.sh

if [ -n "$RUSEFI_FTP_SERVER" ]; then
  echo "Uploading IBOMs"
  tar -cvzf - -C hardware ibom |\
    sshpass -p $RUSEFI_DOXYGEN_FTP_PASS ssh -o StrictHostKeyChecking=no $RUSEFI_DOXYGEN_FTP_USER@$RUSEFI_FTP_SERVER \
      "(tar -xzvf - -C ~)"
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }
