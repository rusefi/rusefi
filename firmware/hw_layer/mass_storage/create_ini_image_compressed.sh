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

echo "create_ini_image: ini $FULL_INI to $H_OUTPUT size $FS_SIZE for $SHORT_BOARDNAME [$BOARD_SPECIFIC_URL]"

rm -f rusefi.zip ramdisk_image.h

# copy *count*KB of zeroes
dd if=/dev/zero of=ramdisk.image bs=1024 count=$FS_SIZE

# create a FAT filesystem inside, name it RUSEFI
mkfs.fat ramdisk.image
# labels can be no longer than 11 characters
fatlabel ramdisk.image RUSEFI




cp hw_layer/mass_storage/filesystem_contents/rusEFI_Wiki_template.url hw_layer/mass_storage/wiki.temp
echo "URL=${BOARD_SPECIFIC_URL}" >>  hw_layer/mass_storage/wiki.temp

# Put the zip inside the filesystem
mcopy -i ramdisk.image $FULL_INI ::
# Put a readme text file in there too
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/README.nozip.txt ::README.txt
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/rusEFI\ Forum.url ::
mcopy -i ramdisk.image hw_layer/mass_storage/wiki.temp ::rusEFI\ ${SHORT_BOARDNAME}\ Wiki.url

# Compress the image as DEFLATE with gzip
gzip ramdisk.image

# write out as a C array, with "static const" tacked on the front
xxd -i ramdisk.image.gz \
    | cat <(echo -n "static const ") - \
    > $H_OUTPUT

rm ramdisk.image.gz
