#!/usr/bin/env bash

# this script installs tools needed to compile ECU firmware, simulator and unit tests
#
# out of scope: JDK, ARM GCC compiler

apt-get install make
apt-get install gcc-multilib g++-multilib g++-mingw-w64 gcc-mingw-w64 sshpass mtools zip dosfstools colordiff
