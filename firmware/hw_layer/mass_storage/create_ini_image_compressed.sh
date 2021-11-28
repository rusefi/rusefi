#!/bin/bash

# from firmware folder:
# hw_layer/mass_storage/create_ini_image_compressed.sh tunerstudio/generated/rusefi.ini hw_layer/mass_storage/ramdisk_image.h  1024 test https://rusefi.com/s/test

# fail on error
set -e

FULL_INI=$1
H_OUTPUT=$2
FS_SIZE=$3
SHORT_BOARDNAME=$4
BOARD_SPECIFIC_URL=$5

IMAGE=ramdisk.image

echo "create_ini_image_compressed: ini $FULL_INI to $H_OUTPUT size $FS_SIZE for $SHORT_BOARDNAME [$BOARD_SPECIFIC_URL]"

rm -f $IMAGE $IMAGE.gz

# copy *count*KB of zeroes
dd if=/dev/zero of=$IMAGE bs=1024 count=$FS_SIZE

# create a FAT filesystem inside, name it RUSEFI
mkfs.fat $IMAGE
# labels can be no longer than 11 characters
fatlabel $IMAGE RUSEFI




cp hw_layer/mass_storage/filesystem_contents/rusEFI_Wiki_template.url hw_layer/mass_storage/wiki.temp
echo "URL=${BOARD_SPECIFIC_URL}" >> hw_layer/mass_storage/wiki.temp
cp hw_layer/mass_storage/filesystem_contents/README.nozip.template.txt hw_layer/mass_storage/readme.temp
echo ${BOARD_SPECIFIC_URL}       >> hw_layer/mass_storage/readme.temp

# Put the zip inside the filesystem
mcopy -i $IMAGE $FULL_INI ::
# Put a readme text file in there too
mcopy -i $IMAGE hw_layer/mass_storage/readme.temp ::README.txt
mcopy -i $IMAGE hw_layer/mass_storage/filesystem_contents/rusEFI\ Forum.url ::
mcopy -i $IMAGE hw_layer/mass_storage/filesystem_contents/rusEFI\ Quick\ Start.url ::
mcopy -i $IMAGE hw_layer/mass_storage/wiki.temp ::rusEFI\ ${SHORT_BOARDNAME}\ Wiki.url

# Compress the image as DEFLATE with gzip
gzip $IMAGE

# write out as a C array, with "static const" tacked on the front
xxd -i $IMAGE.gz \
    | cat <(echo -n "static const ") - \
    > $H_OUTPUT

rm $IMAGE.gz
