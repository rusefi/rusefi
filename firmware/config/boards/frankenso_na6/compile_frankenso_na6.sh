#!/bin/bash

DEFAULT_ENGINE_TYPE=-DDEFAULT_ENGINE_TYPE=FRANKENSO_MIATA_NA6_VAF
FW_ID_ENV=frankensoNA6


#
# sad technical debt: HW CI uses default bundle and it needs logic analyzer
# while humans want to use second cam input asap https://rusefi.com/forum/viewtopic.php?f=4&t=2343
#

bash ../common_script.sh config/boards/frankenso_na6/meta-info.env
