#!/bin/bash

echo "Should be executed from project root folder. Will try to upload to $RUSEFI_FTP_SERVER"
pwd
# ibom is part of Doxygen job simply in order to reduce workspace HDD usage on my tiny build server
bash misc/jenkins/InteractiveHtmlBom/run.sh

if [ -n "$RUSEFI_FTP_SERVER" ]; then
  echo "Uploading IBOMs"
  ncftpput -R -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" /ibom hardware/ibom/*
fi

pwd
cd firmware

doxygen || { echo "doxygen run FAILED"; exit 1; }

# http://www.ncftp.com/download/
# actually Cygwin http://rusefi.com/wiki/index.php?title=Internal:Software:Build_Server
cd ../doxygen
if [ -n "$RUSEFI_FTP_SERVER" ]; then
  echo "Uploading Doxygen"
  ncftpput -R -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" /html html/
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }

cd ..

CONNECTORS=$(find -path "./firmware/config/boards/*/connectors/*.yaml")
for c in $CONNECTORS; do
  DIR="pinouts/"$(echo $c | tr '/' '\n' | tail -n +5 | head -n -2 | tr '\n' '/')
  NAME=$(basename $c .yaml)
  mkdir -p $DIR
  bash misc/pinout-gen/gen.sh $c > $DIR/$NAME.html
  IMG=$(yq r $c 'info.image.file')
  if [ $IMG ]; then
    cp $(dirname $DIR)/$IMG $DIR
  fi
done

if [ -n "$RUSEFI_FTP_SERVER" ]; then
  echo "Uploading Pinouts"
  ncftpput -R -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" /pinouts pinouts/
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }
