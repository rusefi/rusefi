ifeq ($(MODULE_DTC_MANAGER),yes)
MODULES_INC += $(PROJECT_DIR)/controllers/modules/dtc_manager
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/dtc_manager/dtc_manager.cpp
MODULES_INCLUDE += \#include "dtc_manager.h"\n
MODULES_LIST += DtcManagerModule,
DDEFS += -DMODULE_DTC_MANAGER=TRUE
else
DDEFS += -DMODULE_DTC_MANAGER=FALSE
endif
