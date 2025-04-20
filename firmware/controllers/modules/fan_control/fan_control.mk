MODULES_INC += $(PROJECT_DIR)/controllers/modules/fan_control
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/fan_control/fan_control.cpp
MODULES_INCLUDE += \#include "fan_control.h"\n
MODULES_LIST += FanControl1,FanControl2,

# this define needs to be used in any file where the module is used (ie to not generate a compile error when we deactivate this module)
DDEFS += -DMODULE_FAN_CONTROL
