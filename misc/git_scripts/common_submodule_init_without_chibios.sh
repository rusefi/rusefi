#!/bin/bash

        git submodule update --init --depth=1 .github/workflows/hw-ci/gethla
        git submodule update --init --depth=1 firmware/libfirmware
        git submodule update --init --depth=1 firmware/ext/lua
        git submodule update --init --depth=1 firmware/ext/uzlib
        git submodule update --init --depth=1 firmware/ext/openblt
        git submodule update --init --depth=1 firmware/controllers/lua/luaaa
        git submodule update --init --depth=1 firmware/controllers/can/wideband_firmware
        git submodule update --init --depth=1 java_console/luaformatter
        git submodule update --init --depth=1 java_console/peak-can-basic
        git submodule update --init --depth=1 unit_tests/googletest
