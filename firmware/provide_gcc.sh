#!/usr/bin/env bash

# Download and extract GCC arm-none-eabi toolchain

set -e

# URL to download original toolchain from
URL="https://github.com/rusefi/build_support/raw/master/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz"
# This is the md5sum of the /bin/arm-none-eabi-ld executable within the archive, used for verifying we have the proper version.
# If you change the above URL, you will need to update this checksum as well.
MANIFEST_SUM="74515a27fb04b300fdefa666bd03255b"
# colloquial directory name, to afford re-use of script
COLLOQUIAL="gcc-arm-none-eabi"
# persistent working directory
TMP_DIR="${HOME}/.rusefi-tools/rusefi-provide_gcc14"

archive="${URL##*/}"

SWD="$PWD"

# If the checksum file doesn't exist, or if its checksum doesn't match, then download and install the archive.
if [ ! -f "${TMP_DIR}"/*/bin/arm-none-eabi-ld ] ||\
   [ "$MANIFEST_SUM" != "$(md5sum ${TMP_DIR}/*/bin/arm-none-eabi-ld | cut -d ' ' -f 1)" ]; then
	rm -rf "${TMP_DIR}"
	# Download and extract archive
	echo Downloading and extracting ${archive}
	mkdir -p "${TMP_DIR}"
	cd "${TMP_DIR}"
	curl -L -o "${archive}" "${URL}"
	tar -xaf "${archive}"
	echo "Cleaning ${archive}"
	rm "${archive}"
else
	echo "Toolkit already present"
fi

# Create colloquially named link
cd "$SWD"
echo "Linking ${TMP_DIR} ${COLLOQUIAL}"
ln -sf "${TMP_DIR}"/* "${COLLOQUIAL}"
