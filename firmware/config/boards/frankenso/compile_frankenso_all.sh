#!/bin/bash

cd ../../..

export EXTRA_PARAMS="-DDUMMY \
 -DDEFAULT_ENGINE_TYPE=MIATA_NA6_VAF \
 \
 \
 \
 \
 \
 -DFIRMWARE_ID=\\\"frankensoNA6\\\""

bash config/boards/common_make.sh

