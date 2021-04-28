#!/bin/bash

if [ -z $1 ] ; then
 echo "Specific yaml parameter not specified, searching for */connectors/*.yaml"
 CONNECTORS=$(find -path "./firmware/config/boards/*/connectors/*.yaml")
else
 CONNECTORS=$1
 echo "Processing $CONNECTORS"
fi
for c in $CONNECTORS; do
  echo "processing "$c
  DIR="pinouts/"$(echo $c | tr '/' '\n' | tail -n +5 | head -n -2 | tr '\n' '/')
  echo "DIR "$DIR
  NAME=$(basename $c .yaml)
  echo "NAME "$NAME
  mkdir -p $DIR
  if [ -f $DIR/index.html ]; then
    bash misc/pinout-gen/append.sh $c $DIR/index.html
  else
    bash misc/pinout-gen/gen.sh $c $DIR/index.html
  fi
  file $DIR/index.html
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
  echo "Uploading Pinouts..."
  ncftpput -R -z -m -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" / pinouts
  echo "Uploaded!"
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }
