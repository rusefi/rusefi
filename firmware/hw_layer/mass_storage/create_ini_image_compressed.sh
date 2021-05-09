#!/bin/bash

# fail on error
set -e

rm -f rusefi.zip ramdisk_image.h

# copy 1MB of zeroes
dd if=/dev/zero of=ramdisk.image bs=1024 count=1024

# create a FAT filesystem inside, name it RUSEFI
mkfs.fat ramdisk.image
fatlabel ramdisk.image RUSEFI

# Put the zip inside the filesystem
mcopy -i ramdisk.image $1 ::
# Put a readme text file in there too
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/README.nozip.txt ::README.txt
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/rusEFI\ Forum.url ::
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/rusEFI\ Wiki.url ::

# Compress the image as DEFLATE with gzip
gzip ramdisk.image

# write out as a C array, with "static const" tacked on the front
xxd -i ramdisk.image.gz \
    | cat <(echo -n "static const ") - \
    > $2

rm ramdisk.image.gz
