#!/bin/bash

echo "Compiling for Cypress FM4 S6E2CxAH"

export BUILDDIR="build"
export DEBUG_LEVEL_OPT="-O2"

bash ../common_make.sh cypress cypress
