#!/bin/bash

DEFAULT_ENGINE_TYPE=-DDEFAULT_ENGINE_TYPE=FRANKENSO_MIATA_NA6_VAF

export EXTRA_PARAMS="-DDUMMY \
 -DSHORT_BOARD_NAME=frankenso_na6 \
 \
 \
 \
 \
 -DFIRMWARE_ID=\\\"frankensoNA6\\\""

bash ../common_make.sh

