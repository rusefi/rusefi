#!/bin/bash

# from firmware folder:
# hw_layer/mass_storage/create_ini_image.sh tunerstudio/generated/rusefi.ini hw_layer/mass_storage/ramdisk_image.h  112 test https://rusefi.com/s/test

# fail on error
set -e

FULL_INI=$1
H_OUTPUT=$2
FS_SIZE=$3
SHORT_BOARDNAME=$4
BOARD_SPECIFIC_URL=$5

IMAGE=ramdisk.image
ZIP=rusefi.ini.zip

echo "create_ini_image: ini $FULL_INI to $H_OUTPUT size $FS_SIZE for $SHORT_BOARDNAME [$BOARD_SPECIFIC_URL]"

rm -f $ZIP $IMAGE

# copy *count*KB of zeroes
dd if=/dev/zero of=$IMAGE bs=1024 count=$FS_SIZE

# create a FAT filesystem inside, name it RUSEFI
mkfs.fat -v -r 64 $IMAGE
# labels can be no longer than 11 characters
fatlabel $IMAGE RUSEFI

# -j option dumps all files in the root of the zip (instead of inside directories)
zip -j $ZIP $FULL_INI

cp hw_layer/mass_storage/filesystem_contents/rusEFI_Wiki_template.url hw_layer/mass_storage/wiki.temp
echo "URL=${BOARD_SPECIFIC_URL}" >> hw_layer/mass_storage/wiki.temp
cp hw_layer/mass_storage/filesystem_contents/README.template.txt hw_layer/mass_storage/readme.temp
echo ${BOARD_SPECIFIC_URL}       >> hw_layer/mass_storage/readme.temp

# Put the zip inside the filesystem
mcopy -i $IMAGE $ZIP ::
# Put a readme text file in there too
mcopy -i $IMAGE hw_layer/mass_storage/readme.temp ::README.txt
mcopy -i $IMAGE hw_layer/mass_storage/filesystem_contents/rusEFI\ Forum.url ::
mcopy -i $IMAGE hw_layer/mass_storage/filesystem_contents/rusEFI\ Quick\ Start.url ::
mcopy -i $IMAGE hw_layer/mass_storage/wiki.temp ::rusEFI\ ${SHORT_BOARDNAME}\ Wiki.url




# write out as a C array, with "static const" tacked on the front
xxd -i $IMAGE \
    | cat <(echo -n "static const ") - \
    > $H_OUTPUT

rm $ZIP $IMAGE
