# syntax=docker/dockerfile:1
# base image from ggrouv: https://github.com/ggurov/rusefibuildcontainer
FROM ubuntu:mantic
RUN apt-get -y update
RUN apt-get -y install make automake autoconf gcc-12-arm-linux-gnueabi gcc-12-arm-linux-gnueabi-base gcc-arm-none-eabi binutils-arm-none-eabi git nano openjdk-21-jdk-headless mtools dosfstools xxd
RUN apt-get -y install openssh-server cmake rsync gdb-multiarch build-essential g++-12 gcc-12-multilib g++-12-multilib zip zsh
# used for remote non-devcontainer use ex: (https://blog.jetbrains.com/clion/2020/01/using-docker-with-clion/) [also on vscode: https://code.visualstudio.com/remote/advancedcontainers/develop-remote-host]
RUN useradd -m rusefi && yes password | passwd rusefi
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 12 --slave /usr/bin/g++ g++ /usr/bin/g++-12 --slave /usr/bin/gcov gcov /usr/bin/gcov-12
CMD service ssh start && /bin/bash