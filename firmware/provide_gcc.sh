#!/usr/bin/env bash

# Download and extract GCC arm-none-eabi toolchain

set -e

URL="https://github.com/rusefi/build_support/raw/master/rusefi-arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi.tar.xz"
ARCHIVE="${URL##*/}"
DIR="gcc-arm-none-eabi"

# Delete existing archive
rm -rf ${ARCHIVE}

# Download and extract archive
curl -L -o ${ARCHIVE} ${URL}
tar -xavf ${ARCHIVE}

# Create colloquially named link
ARCHIVE_DIR=$(tar --exclude="*/*" -tf ${ARCHIVE})
ln -s ${ARCHIVE_DIR%/} ${DIR}

# Delete downloaded archive
rm ${ARCHIVE}
