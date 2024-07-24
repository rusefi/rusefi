MODULES_INC += $(PROJECT_DIR)/controllers/modules/trip_odometer
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/trip_odometer/trip_odometer.cpp
MODULES_INCLUDE += \#include "trip_odometer.h"\n
MODULES_LIST += TripOdometer,

DDEFS += -DMODULE_TRIP_ODO