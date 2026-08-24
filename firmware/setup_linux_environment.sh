#!/usr/bin/env bash

#
# it's totally possible to develop on Windows. Also WSL is amazing!
# TL,DR: use same gcc version as GitHub Actions from .github/workflows
#
# by the way to uninstall package with old compiler version you might use
# sudo apt remove binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi
#

# This script will download and install all dependencies require to develop rusEFI on Linux.
# After running this script, executing `make` in the firmware folder is expected to produce a functional firmware binary.

# Resolve paths relative to this script, not to the current working directory,
# so the script works when invoked from either the repo root or firmware/
FIRMWARE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(dirname "${FIRMWARE_DIR}")"

# Ensure submodules got cloned
git -C "${REPO_ROOT}" submodule update --init

# Update package lists
sudo apt-get update

# install dependencies
sudo bash "${REPO_ROOT}/misc/actions/ubuntu-install-tools.sh"
sudo apt-get install -y build-essential gcc gdb gcc-multilib make openjdk-11-jdk-headless xxd

# delete any old tools, create a new folder, and go there
rm -rf ~/.rusefi-tools
mkdir ~/.rusefi-tools
cd ~/.rusefi-tools

# provide GCC arm-none-eabi toolchain
# (runs with ~/.rusefi-tools as cwd - that is where it creates the gcc-arm-none-eabi symlink)
"${FIRMWARE_DIR}/provide_gcc.sh"

# Add the compiler to your path
echo 'export PATH=$PATH:$HOME/.rusefi-tools/gcc-arm-none-eabi/bin' >> ~/.profile

# Allow the current user to use serial ports
sudo usermod -a -G dialout $USER
