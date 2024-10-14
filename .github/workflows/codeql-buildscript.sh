#!/usr/bin/env bash

# Reference: firmware/setup_linux_environment.sh

# Update package lists
sudo apt-get update -y

# install dependencies
sudo bash misc/actions/ubuntu-install-tools.sh
sudo apt-get install -y build-essential gcc gdb gcc-multilib make openjdk-11-jdk-headless xxd libncurses5 libncursesw5

# delete any old tools, create a new folder, and go there
rm -rf ~/.rusefi-tools
mkdir ~/.rusefi-tools
dir=$(realpath firmware)
cd ~/.rusefi-tools

cd ${dir}
make -j$(nproc)
