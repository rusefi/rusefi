#!/bin/bash

# This script will download and install all dependencies require to develop rusEFI on Linux.
# After running this script, executing `make` in the firmware folder is expected to produce a functional firmware binary.

# Update package lists
sudo apt-get update

# install dependencies
sudo apt-get install -y build-essential gcc make openjdk-8-jdk-headless ant mtools

# delete any old tools, create a new folder, and go there
rm -rf ~/.rusefi-tools
mkdir ~/.rusefi-tools
cd ~/.rusefi-tools

# Download and extract GCC compiler
curl -L -o arm-none-eabi-gcc.tar.bz2 https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2
tar -xjvf arm-none-eabi-gcc.tar.bz2

# Delete downloaded image
rm arm-none-eabi-gcc.tar.bz2

# delete old symlinks
sudo rm -f /usr/bin/arm-none-eabi-gcc
sudo rm -f /usr/bin/arm-none-eabi-g++
sudo rm -f /usr/bin/arm-none-eabi-gdb
sudo rm -f /usr/bin/arm-none-eabi-size
sudo rm -f /usr/bin/arm-none-eabi-objdump
sudo rm -f /usr/bin/arm-none-eabi-objcopy

# Add symlinks to compiler - compilation doesn't use all the tools, just these 5, so that's all we need to link
sudo ln -s `pwd`/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc
sudo ln -s `pwd`/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++
sudo ln -s `pwd`/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb
sudo ln -s `pwd`/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size
sudo ln -s `pwd`/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-objdump /usr/bin/arm-none-eabi-objdump
sudo ln -s `pwd`/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy
