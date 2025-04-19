MODULES_INC += $(PROJECT_DIR)/controllers/modules/trip_odometer
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/trip_odometer/trip_odometer.cpp
MODULES_INCLUDE += \#include "trip_odometer.h"\n
MODULES_LIST += TripOdometer,

# this define needs to be used in any file where the module is used (ie to not generate a compile error when we deactivate this module)
DDEFS += -DMODULE_ODOMETER
