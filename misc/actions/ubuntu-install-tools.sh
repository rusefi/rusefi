#!/usr/bin/env bash

# this script installs tools needed to compile ECU firmware, simulator and unit tests
#
# out of scope: JDK, ARM GCC compiler

set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

# Retry transient download failures, but fail setup if installation still fails.
apt-get -o Acquire::Retries=3 install -y \
    make gcc-multilib g++-multilib g++-mingw-w64 gcc-mingw-w64 \
    sshpass mtools zip 7zip dosfstools xxd colordiff

# Check the RAM disk tools now instead of failing late in the firmware build.
# Match the PATH used by firmware/hw_layer/mass_storage/create_image.sh.
export PATH="$PATH:/usr/sbin"
for tool in mcopy mkfs.fat fatlabel zip 7z xxd; do
    if ! command -v "$tool" > /dev/null; then
        echo "Required firmware image tool is missing after installation: $tool" >&2
        exit 1
    fi
done
