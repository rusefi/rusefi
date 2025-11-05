#!/bin/bash

# Usage:
# create_image.sh <output_header_file_name> <file_sistem_size_in_kilobytes> <image_compression_flag> <file_to_copy_on_image_1> <file_to_copy_on_image_2> ...

# fail on error
set -e

H_OUTPUT=$1
echo "H_OUTPUT=$H_OUTPUT"
FS_SIZE=$2
echo "FS_SIZE=$FS_SIZE"
COMPRESS_IMAGE=$3
echo "COMPRESS_IMAGE=$COMPRESS_IMAGE"
shift 3

IMAGE=ramdisk.image

# mkfs.fat and fatlabel are privileged on some systems
PATH="$PATH:/usr/sbin"

rm -f $IMAGE $IMAGE.gz

# copy *FS_SIZE*KB of zeroes
dd if=/dev/zero of=$IMAGE bs=1024 count=$FS_SIZE

# create a FAT filesystem inside, name it RUSEFI
mkfs.fat -v -r 64 $IMAGE
# labels can be no longer than 11 characters
fatlabel $IMAGE RUSEFI

for file in "$@"
do
  mcopy -i $IMAGE "$file" ::
  echo "File '$file' is added to the image."
done

# macOS bash 3.x compatible version.
if [ "$(printf '%s' "$COMPRESS_IMAGE" | tr '[:upper:]' '[:lower:]')" = "true" ]; then
  # Compress the image as DEFLATE with gzip
  gzip $IMAGE
  IMAGE_TO_OUTPUT=$IMAGE.gz
else
  IMAGE_TO_OUTPUT=$IMAGE
fi

# write out as a C array, with "static const" tacked on the front
xxd -i $IMAGE_TO_OUTPUT \
    | cat <(echo -n "static const ") - \
    > $H_OUTPUT

rm $IMAGE_TO_OUTPUT
exit 0
