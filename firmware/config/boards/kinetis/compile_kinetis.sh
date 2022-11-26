#!/bin/bash

echo "Compiling for Kinetis MKE16F512"

# set BUILDDIR=build_kinetis
export BUILDDIR=build

# rm -f ${BUILDDIR}/rusefi_kinetis.*

# make -r -j4

bash ../common_make.sh kinetis kinetis
