#!/usr/bin/env bash

#
# it's totally possible to develop on Windows. Also WSL is amazing!
# TL,DR: use gcc 9.3
#
# by the way to uninstall package with old compiler version you might use
# sudo apt remove binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi
#

# This script will download and install all dependencies require to develop rusEFI on Linux.
# After running this script, executing `make` in the firmware folder is expected to produce a functional firmware binary.

# Ensure submodules got cloned
git submodule update --init

# Update package lists
sudo apt-get update

# install dependencies
sudo apt-get install -y build-essential gcc gdb gcc-multilib g++-multilib make openjdk-8-jdk-headless mtools zip xxd libncurses5 libncursesw5

# delete any old tools, create a new folder, and go there
rm -rf ~/.rusefi-tools
mkdir ~/.rusefi-tools
dir=$(pwd)
cd ~/.rusefi-tools

# provide GCC arm-none-eabi toolchain
${dir}/provide_gcc.sh

# Add the compiler to your path
echo 'export PATH=$PATH:$HOME/.rusefi-tools/gcc-arm-none-eabi/bin' >> ~/.profile

# Allow the current user to use serial ports
sudo usermod -a -G dialout $USER
