#!/bin/bash

# fail on error
set -e

rm -f rusefi.zip ramdisk_image.h

# copy *count*KB of zeroes
dd if=/dev/zero of=ramdisk.image bs=1024 count=112

# create a FAT filesystem inside, name it RUSEFI
mkfs.fat ramdisk.image
fatlabel ramdisk.image RUSEFI

# -j option dumps all files in the root of the zip (instead of inside directories)
zip -j rusefi.ini.zip $1

# Put the zip inside the filesystem
mcopy -i ramdisk.image rusefi.ini.zip ::
# Put a readme text file in there too
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/README.txt ::
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/rusEFI\ Forum.url ::
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/rusEFI\ Wiki.url ::

# write out as a C array, with "static const" tacked on the front
xxd -i ramdisk.image \
    | cat <(echo -n "static const ") - \
    > $2

rm ramdisk.image rusefi.ini.zip
