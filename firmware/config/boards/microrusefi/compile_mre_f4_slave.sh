#!/bin/bash

export EXTRA_PARAMS=-DDUMMY -DEFI_CANBUS_SLAVE=TRUE

export VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::BMW_M73_MRE_SLAVE

bash ../common_make.sh config/boards/microrusefi/meta-info_f4.env
