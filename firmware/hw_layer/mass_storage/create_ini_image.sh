#!/bin/bash

# fail on error
set -e

FULL_INI=$1
H_OUTPUT=$2
FS_SIZE=$3
SHORT_BOARDNAME=$4

echo "ini $FULL_INI to $H_OUTPUT size $FS_SIZE for $SHORT_BOARDNAME"

rm -f rusefi.zip ramdisk_image.h

# copy *count*KB of zeroes
dd if=/dev/zero of=ramdisk.image bs=1024 count=$FS_SIZE

# create a FAT filesystem inside, name it RUSEFI
mkfs.fat ramdisk.image
# labels can be no longer than 11 characters
fatlabel ramdisk.image RUSEFI

# -j option dumps all files in the root of the zip (instead of inside directories)
zip -j rusefi.ini.zip $FULL_INI

# Put the zip inside the filesystem
mcopy -i ramdisk.image rusefi.ini.zip ::
# Put a readme text file in there too
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/README.txt ::
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/rusEFI\ Forum.url ::
mcopy -i ramdisk.image hw_layer/mass_storage/filesystem_contents/rusEFI\ Wiki.url ::




# write out as a C array, with "static const" tacked on the front
xxd -i ramdisk.image \
    | cat <(echo -n "static const ") - \
    > $H_OUTPUT

rm ramdisk.image rusefi.ini.zip
