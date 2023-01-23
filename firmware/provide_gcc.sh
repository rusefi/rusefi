#!/usr/bin/env bash

# Download and extract GCC arm-none-eabi toolchain

set -e

# URL to download original toolchain from
URL="https://github.com/rusefi/build_support/raw/master/rusefi-arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi.tar.xz"
MANIFEST_SUM="e4b25a623445c9f02303eba63b1d2806"
# colloquial directory name, to afford re-use of script
COLLOQUIAL="gcc-arm-none-eabi"
# temporary working directory
TMP_DIR="/tmp/rusefi-provide_gcc"

archive="${URL##*/}"

if [ -d "${TMP_DIR}" ]; then
	if [ "$(md5sum ${TMP_DIR}/*manifest.txt | cut -d ' ' -f 1)" = "$MANIFEST_SUM" ]; then
		exit 0
	fi
fi

# Download and extract archive
echo Downloading and extracting ${archive}
mkdir -p "${TMP_DIR}"
cd "${TMP_DIR}"
curl -L -o "${archive}" "${URL}"
tar -xaf "${archive}"
rm "${archive}"

# Create colloquially named link
archive_dir="$(echo *)"
cd - >/dev/null
mv "${TMP_DIR}/${archive_dir}" "$(pwd)"
ln -s "${archive_dir%/}" "${COLLOQUIAL}"
