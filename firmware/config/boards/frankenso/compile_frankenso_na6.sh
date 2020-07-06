#!/bin/sh

cd ../../..

export EXTRA_PARAMS="-DDUMMY \
 -DDEFAULT_ENGINE_TYPE=MIATA_NA6_VAF \
 -DSHORT_BOARD_NAME=fra \
 -DFIRMWARE_ID=\\\"frankensoNA6\\\""

sh config/boards/common_make.sh

