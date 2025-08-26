MODULES_INC += $(PROJECT_DIR)/controllers/modules/vvl_controller
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/vvl_controller/vvl_controller.cpp
MODULES_INCLUDE += \#include "vvl_controller.h"\n
MODULES_LIST += VvlController,

# this define needs to be used in any file where the module is used (ie to not generate a compile error when we deactivate this module)
DDEFS += -DMODULE_VVL_CONTROLLER
