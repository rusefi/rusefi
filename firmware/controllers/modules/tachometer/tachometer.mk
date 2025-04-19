MODULES_INC += $(PROJECT_DIR)/controllers/modules/tachometer
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/tachometer/tachometer.cpp
MODULES_INCLUDE += \#include "tachometer.h"\n
MODULES_LIST += TachometerModule,

# this define needs to be used in any file where the module is used (ie to not generate a compile error when we deactivate this module)
DDEFS += -DMODULE_TACHOMETER
