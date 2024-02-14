#!/bin/bash

export EXTRA_PARAMS="-DVR_HW_CHECK_MODE=TRUE -DHW_CHECK_MODE=TRUE -DHW_CHECK_SD=TRUE -DHW_CHECK_ALWAYS_STIMULATE=TRUE"

export VAR_DEF_ENGINE_TYPE=-DDEFAULT_ENGINE_TYPE=engine_type_e::PROTEUS_QC_TEST_BOARD

export DEBUG_LEVEL_OPT="-Os -ggdb -g"

bash ../common_make.sh config/boards/proteus/meta-info-proteus_f7_hardware_QC_special_build.env
