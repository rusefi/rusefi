#!/bin/bash

# from firmware folder:
# hw_layer/mass_storage/create_ini_image_compressed.sh tunerstudio/generated/rusefi.ini hw_layer/mass_storage/ramdisk_image.h  1024 test https://rusefi.com/s/test

# fail on error
set -e

if [ "$(expr substr $(uname -s) 1 6)" == "CYGWIN" ] || [ "$(expr substr $(uname -s) 1 5)" == "MINGW" ]; then
  echo No image on $(uname -s)
  exit 0
fi

FULL_INI=$1
H_OUTPUT=$2
FS_SIZE=$3
SHORT_BOARD_NAME=$4
BOARD_SPECIFIC_URL=$5

IMAGE=ramdisk.image

# mkfs.fat and fatlabel are privileged on some systems
PATH="$PATH:/usr/sbin"

echo "create_ini_image_compressed: ini $FULL_INI to $H_OUTPUT size $FS_SIZE for $SHORT_BOARD_NAME [$BOARD_SPECIFIC_URL]"

rm -f $IMAGE $IMAGE.gz

# copy *FS_SIZE*KB of zeroes
dd if=/dev/zero of=$IMAGE bs=1024 count=$FS_SIZE

# create a FAT filesystem inside, name it RUSEFI
mkfs.fat $IMAGE
# labels can be no longer than 11 characters
fatlabel $IMAGE RUSEFI




if [[ $OSTYPE == 'darwin'* ]]; then
  # Mac OS comes with Bash version 3 which is quite limiting and lack key features
  current_date='huh-MAC'
else
  bash --version
  # bash >=4.3 magic
  printf -v current_date '%(%Y-%m-%d)T' -1
  echo "create_ini_image_compressed.sh says [${current_date}]"
fi

README_FILE_PATH=hw_layer/mass_storage/README-${current_date}.txt
WIKI_FILE_PATH=hw_layer/mass_storage/rusEFI\ ${SHORT_BOARD_NAME}\ Wiki.url

cp hw_layer/mass_storage/filesystem_contents/rusEFI_Wiki_template.url "${WIKI_FILE_PATH}"
echo "URL=${BOARD_SPECIFIC_URL}" >> "${WIKI_FILE_PATH}"
cp hw_layer/mass_storage/filesystem_contents/README.template.txt "${README_FILE_PATH}"
echo ${BOARD_SPECIFIC_URL}       >> "${README_FILE_PATH}"

# Put the zip inside the filesystem
mcopy -i $IMAGE $FULL_INI ::
# Put a readme text file in there too
mcopy -i $IMAGE "${README_FILE_PATH}" ::
mcopy -i $IMAGE hw_layer/mass_storage/filesystem_contents/rusEFI\ Forum.url ::
mcopy -i $IMAGE hw_layer/mass_storage/filesystem_contents/rusEFI\ Quick\ Start.url ::
mcopy -i $IMAGE "${WIKI_FILE_PATH}" ::

# Compress the image as DEFLATE with gzip
gzip $IMAGE

# write out as a C array, with "static const" tacked on the front
xxd -i $IMAGE.gz \
    | cat <(echo -n "static const ") - \
    > $H_OUTPUT

rm $IMAGE.gz "${README_FILE_PATH}" "${WIKI_FILE_PATH}"
exit 0
