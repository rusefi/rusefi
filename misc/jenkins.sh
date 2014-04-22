#!/bin/bash
    echo "The PATH is ${PATH}"
    cd "${WORKSPACE}"
    echo "start in workspace ${PWD}"
    
    cd firmware
    echo "CD to ${PWD}"

    rm -fR .dep
    rm -fR build
    make

    if [ ! -f build/rusefi.hex ]; then
        echo "Firmware compilation failed"
        exit -1
    fi

    cd "${WORKSPACE}/win32_algo_tests"
    echo "CD to ${PWD}"

    rm -fR .dep
    rm -fR build
    make
    if [ ! -f build/rusefi_test ]; then
        echo "test compilation failed"
        exit -1
    fi

# we want to terminate if test fails
set -e

    # invoke the tests - hopefully error code would be propagated?
    build/rusefi_test

cd "${WORKSPACE}/java_console"
echo "CD to ${PWD}"

ant

cd "${WORKSPACE}"
echo "CD to ${PWD}"

zip -j firmware/build/rusefi_firmware.zip firmware/build/rusefi.hex firmware/tunerstudio/rusefi.ini

echo user u71977750-build pass > ftp.cmd
echo put firmware/build/rusefi_firmware.zip rusefi_firmware.zip >> ftp.cmd
echo put java_console_binary/rusefi_console.jar rusefi_console.jar >> ftp.cmd
ftp -npiv rusefi.com < ftp.cmd
cat ftp.cmd
rm ftp.cmd 