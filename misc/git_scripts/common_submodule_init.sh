#!/bin/bash
# [tag:submodule_auth] in GitHub Actions call this through .github/actions/submodule-init (or custom-board-build's submodule_token), which supplies the credentials.

misc/git_scripts/common_submodule_init_without_chibios.sh

        git submodule update --init --depth=1 firmware/ChibiOS
        git submodule update --init --depth=1 firmware/ChibiOS-Contrib
