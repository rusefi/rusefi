#!/usr/bin/env bash

# run as root

[ ! -f /etc/apt/sources.list.bak ] && cp /etc/apt/sources.list /etc/apt/sources.list.bak
for mirror in \
    'http://azure.archive.ubuntu.com/ubuntu/' \
    'http://ubuntu.osuosl.org/ubuntu/' \
    'http://mirrors.ocf.berkeley.edu/ubuntu/' \
    ; do
    sed "s#http:.*?(?=\s)#$mirror#" /etc/apt/sources.list.bak > /etc/apt/sources.list
    if apt-get -o Acquire::Retries=3 update; then
        exit 0
    fi
done
exit 1

