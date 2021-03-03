#!/bin/bash

rm -f rusefi.zip ramdisk_image.h

# copy 100KB of zeroes
dd if=/dev/zero of=ramdisk.image bs=1024 count=100

# create a FAT filesystem inside, name it RUSEFI
mkfs.fat ramdisk.image
fatlabel ramdisk.image RUSEFI

# -j option dumps all files in the root of the zip (instead of inside directories)
zip -j rusefi.ini.zip $1

# Put the zip inside the filesystem
mcopy -i ramdisk.image rusefi.ini.zip ::

# write out as a C array, with "static const" tacked on the front
xxd -i ramdisk.image \
    | cat <(echo -n "static const ") - \
    > $2

rm ramdisk.image rusefi.ini.zip
