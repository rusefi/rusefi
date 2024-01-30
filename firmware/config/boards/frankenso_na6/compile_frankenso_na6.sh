#!/usr/bin/env bash

DEFAULT_ENGINE_TYPE=-DDEFAULT_ENGINE_TYPE=FRANKENSO_MIATA_NA6_VAF
FW_ID_ENV=frankensoNA6


#
# sad technical debt: HW CI uses default bundle and it needs logic analyzer
# while humans want to use second cam input asap https://rusefi.com/forum/viewtopic.php?f=4&t=2343
#

export PROJECT_BOARD="frankenso_na6"
export PROJECT_CPU="ARCH_STM32F4"
export SHORT_BOARD_NAME=frankenso_na6
