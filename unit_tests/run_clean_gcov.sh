#!/bin/bash

make clean
make -j6
build/fome_test
bash ci_gcov.sh "$RUSEFI_DOXYGEN_FTP_USER" "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER"
