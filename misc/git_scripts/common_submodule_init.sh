#!/bin/bash

misc/git_scripts/common_submodule_init_without_chibios.sh

        git submodule update --init --depth=1 firmware/ChibiOS
        git submodule update --init --depth=1 firmware/ChibiOS-Contrib
