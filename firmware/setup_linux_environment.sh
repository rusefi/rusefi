#!/usr/bin/env bash

#
# This script configures your Linux machine (probably just Debian/Ubuntu-based machine, really)
# for FOME firmware development and compilation.
#
# This script will also work on a Windows Subsystem for Linux installation on top of a Windows PC.
#
# Purposes for dependencies:
#  - `gcc`, `gdb`, `gcc-multilib`, `g++-multilib`: Native compilers for your PC, used for unit tests and simulator.
#  - `make`: GNU Make, build tool that orchestrates the correct build steps.
#  - `openjdk-8-jdk-headless`: Java compiler for FOME console and code generation tools.
#  - `mtools`, `zip`, `xxd`: Used to generate the on-ECU filesystem that stores the matching ini file.
#

# This script will download and install all dependencies require to develop rusEFI on Linux.
# After running this script, executing `make` in the firmware folder is expected to produce a functional firmware binary.

# Ensure submodules got cloned
git submodule update --init

# Update package lists
sudo apt-get update

# install dependencies
sudo apt-get install -y build-essential gcc gdb gcc-multilib g++-multilib make openjdk-8-jdk-headless mtools zip xxd libncurses5 libncursesw5

# Allow the current user to use serial ports
sudo usermod -a -G dialout $USER

# For gdb to work, unpack it.
bash ./ext/build-tools/inflate.sh
