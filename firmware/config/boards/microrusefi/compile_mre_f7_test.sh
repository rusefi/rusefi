#!/bin/bash

export VAR_DEF_ENGINE_TYPE="-DDEFAULT_ENGINE_TYPE=engine_type_e::MRE_BOARD_TEST"

bash ../common_make.sh config/boards/microrusefi/meta-info_f7.env
