#!/bin/bash

        sudo apt-get update
        sudo apt-get install ncftp
        wget https://github.com/mikefarah/yq/releases/download/v4.9.6/yq_linux_amd64
        chmod a+x yq_linux_amd64
        mkdir yqdir
        mv yq_linux_amd64 yqdir/yq
