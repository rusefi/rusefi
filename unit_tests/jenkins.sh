#!/bin/bash
    echo "The PATH is ${PATH}"
    cd "${WORKSPACE}"
    echo "start in workspace ${PWD}"
    
    cd firmware
    echo "CD to ${PWD}"

    rm -fR .dep
    rm -fR build
    make

    if [ ! -f build/fome.hex ]; then
        echo "Firmware compilation failed"
        exit -1
    fi

    cd "${WORKSPACE}/win32_algo_tests"
    echo "CD to ${PWD}"

    rm -fR .dep
    rm -fR build
    make
    if [ ! -f build/fome_test ]; then
        echo "test compilation failed"
        exit -1
    fi

# we want to terminate if test fails
set -e

    # invoke the tests - hopefully error code would be propagated?
    build/fome_test

cd "${WORKSPACE}/java_console"
echo "CD to ${PWD}"

#JAVA_HOME=/usr/lib/jvm/java-6-openjdk-amd64
#ant

