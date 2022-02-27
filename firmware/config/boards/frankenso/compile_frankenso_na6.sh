#!/bin/bash

DEFAULT_ENGINE_TYPE=-DDEFAULT_ENGINE_TYPE=FRANKENSO_MIATA_NA6_VAF
FW_ID_ENV=frankensoNA6

export EXTRA_PARAMS="-DDUMMY \
 -DSHORT_BOARD_NAME=frankenso_na6 \
 \
 \
 \
 \
 "

bash ../common_make.sh

