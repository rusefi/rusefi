MODULES_INC += $(PROJECT_DIR)/controllers/modules/example_module
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/example_module/example_module.cpp
MODULES_INCLUDE += \#include "example_module.h"\n
MODULES_LIST += ModuleExample,

DDEFS += -DMODULE_EXAMPLE_LIVEDATA