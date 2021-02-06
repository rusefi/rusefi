#!/bin/bash

echo "Should be executed from project root folder. Will try to upload to $RUSEFI_FTP_SERVER"
pwd
# ibom is part of Doxygen job simply in order to reduce workspace HDD usage on my tiny build server
bash misc/jenkins/InteractiveHtmlBom/run.sh

if [ -n "$RUSEFI_FTP_SERVER" ]; then
  echo "Uploading IBOMs"
  tar -cvzf - hardware/ibom |\
    sshpass -p $RUSEFI_DOXYGEN_FTP_PASS ssh -o StrictHostKeyChecking=no $RUSEFI_DOXYGEN_FTP_USER@$RUSEFI_FTP_SERVER \
      "(tar -xzvf - -C ~)"
fi

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

cd ..

CONNECTORS=$(find -path "./firmware/config/boards/*/connectors/*.yaml")
for c in $CONNECTORS; do
  echo "processing "$c
  DIR="pinouts/"$(echo $c | tr '/' '\n' | tail -n +5 | head -n -2 | tr '\n' '/')
  echo "DIR "$DIR
  NAME=$(basename $c .yaml)
  echo "NAME "$NAME
  mkdir -p $DIR
  bash misc/pinout-gen/gen.sh $c > $DIR/$NAME.html
  file $DIR/$NAME.html
  IMG=$(yq r $c 'info.image.file')
  echo "IMG "$IMG
  if [ $IMG ]; then
    cp $(dirname $c)/$IMG $DIR
  fi
  ls $DIR
  cp misc/pinout-gen/yaml.min.js $DIR
  ls $DIR
done

if [ -n "$RUSEFI_FTP_SERVER" ]; then
  echo "Uploading Pinouts"
  tar -cvzf - pinouts |\
    sshpass -p $RUSEFI_DOXYGEN_FTP_PASS ssh -o StrictHostKeyChecking=no $RUSEFI_DOXYGEN_FTP_USER@$RUSEFI_FTP_SERVER \
      "(tar -xzvf - -C ~)"
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }
