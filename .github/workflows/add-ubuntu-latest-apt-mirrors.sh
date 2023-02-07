#!/usr/bin/env bash

# run as root

touch /etc/apt/sources.list.d/ubuntu-latest-mirrors.list
for mirror in \
    'http://ubuntu.osuosl.org/ubuntu/' \
    'http://mirrors.ocf.berkeley.edu/ubuntu/' \
    ; do
    cat /etc/apt/sources.list >> /etc/apt/sources.list.d/ubuntu-latest-mirrors.list
    sed "s#http://azure.archive.ubuntu.com/ubuntu/#$mirror#" -i /etc/apt/sources.list.d/ubuntu-latest-mirrors.list
done

apt-get -o Acquire::Retries=3 update
