#!/bin/bash

# from firmware folder:
# hw_layer/mass_storage/create_ini_image.sh tunerstudio/generated/rusefi.ini hw_layer/mass_storage/ramdisk_image.h  112 test https://rusefi.com/s/test

# fail on error
set -euo pipefail

if [ "$(expr substr $(uname -s) 1 6)" == "CYGWIN" ] || [ "$(expr substr $(uname -s) 1 5)" == "MINGW" ]; then
  echo No image on $(uname -s)
  exit 0
fi

FULL_INI=$1
H_OUTPUT=$2
FS_SIZE=$3
SHORT_BOARDNAME=$4
BOARD_SPECIFIC_URL=$5

IMAGE=ramdisk.image
ZIP=fome.ini.zip

# mkfs.fat and fatlabel are privileged on some systems
PATH="$PATH:/usr/sbin"

echo "create_ini_image: ini $FULL_INI to $H_OUTPUT size $FS_SIZE for $SHORT_BOARDNAME [$BOARD_SPECIFIC_URL]"

rm -f $ZIP $IMAGE

# copy *FS_SIZE*KB of zeroes
dd if=/dev/zero of=$IMAGE bs=1024 count=$FS_SIZE

# create a FAT filesystem inside, name it "FOME EFI"
mkfs.fat -v -r 64 $IMAGE
# labels can be no longer than 11 characters
fatlabel $IMAGE "FOME EFI"

# -j option dumps all files in the root of the zip (instead of inside directories)
zip -j $ZIP $FULL_INI

cp hw_layer/mass_storage/filesystem_contents/README.template.txt hw_layer/mass_storage/readme.temp
echo ${BOARD_SPECIFIC_URL}       >> hw_layer/mass_storage/readme.temp

if [[ $OSTYPE == 'darwin'* ]]; then
  # Mac OS comes with Bash version 3 which is quite limiting and lack key features
  current_date='huh-MAC'
else
  bash --version
  # bash >=4.3 magic
  printf -v current_date '%(%Y-%m-%d)T' -1
  echo "create_ini_image.sh says [${current_date}]"
fi

# Put the zip inside the filesystem
mcopy -i $IMAGE $ZIP ::
# Put a readme text file in there too
mcopy -i $IMAGE hw_layer/mass_storage/readme.temp ::README-${current_date}.txt
mcopy -i $IMAGE hw_layer/mass_storage/filesystem_contents/FOME\ Wiki.url ::
mcopy -i $IMAGE hw_layer/mass_storage/filesystem_contents/FOME\ Discord.url ::
mcopy -i $IMAGE hw_layer/mass_storage/filesystem_contents/FOME\ Releases.url ::




# write out as a C array, with "static const" tacked on the front
xxd -i $IMAGE \
    | cat <(echo -n "static const ") - \
    > $H_OUTPUT

rm $ZIP $IMAGE
exit 0
