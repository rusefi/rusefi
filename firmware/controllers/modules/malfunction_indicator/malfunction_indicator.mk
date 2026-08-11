ifeq ($(MODULE_MIL),yes)
MODULES_INC += $(PROJECT_DIR)/controllers/modules/malfunction_indicator
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/malfunction_indicator/malfunction_indicator.cpp
MODULES_INCLUDE += \#include "malfunction_indicator.h"\n
MODULES_LIST += MILController,
DDEFS += -DMODULE_MIL
endif
