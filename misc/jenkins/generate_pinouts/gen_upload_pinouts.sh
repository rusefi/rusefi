#!/bin/bash

I_AM="gen_upload_pinouts.sh:"

if [ -z $1 ] ; then
 echo "Specific yaml parameter not specified, searching for */connectors/*.yaml"
 CONNECTORS=$(find -path "./firmware/config/boards/*/connectors/*.yaml")
else
 CONNECTORS=$1
 echo "Processing $CONNECTORS"
fi
# yq 3 and 4 have incompatible syntax. We use yq 4.
echo ${I_AM} invoking yq version
yq -V
for c in $CONNECTORS; do
  echo "${I_AM} processing "$c
  DIR="pinouts/"$(echo $c | tr '/' '\n' | tail -n +5 | head -n -2 | tr '\n' '/')
  echo "${I_AM} DIR "$DIR
  NAME=$(basename $c .yaml)
  echo "${I_AM} NAME "$NAME
  mkdir -p $DIR
  if [ -f $DIR/index.html ]; then
    bash misc/pinout-gen/append.sh "$(yq -j e $c)" $DIR/index.html
  else
    bash misc/pinout-gen/gen.sh "$(yq -j e $c)" $DIR/index.html
  fi
  if [ $? -ne 0 ]; then
    echo "${I_AM} something is broken"
    exit 1;
  fi
  file $DIR/index.html
  IMG=$(yq e '.info.image.file' $c)
  if [ $? -ne 0 ]; then
    exit 1;
  fi
  echo "IMG "$IMG
  if [ "$IMG" != "null" ]; then
    cp $(dirname $c)/$IMG $DIR
  fi
  ls $DIR
done

if [ -n "$RUSEFI_FTP_SERVER" ]; then
  echo "Uploading Pinouts..."
  ncftpput -r 999 -m -R -z -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" / pinouts
  echo "Uploaded!"
fi
[ $? -eq 0 ] || { echo "upload FAILED"; exit 1; }

