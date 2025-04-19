MODULES_INC += $(PROJECT_DIR)/controllers/modules/fuel_pump
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/fuel_pump/fuel_pump.cpp
MODULES_INCLUDE += \#include "fuel_pump.h"\n
MODULES_LIST += FuelPumpController,

# this define needs to be used in any file where the module is used (ie to not generate a compile error when we deactivate this module)
DDEFS += -DMODULE_FUEL_PUMP
