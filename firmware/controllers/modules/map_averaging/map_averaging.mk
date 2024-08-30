MODULES_INC += $(PROJECT_DIR)/controllers/modules/map_averaging
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/map_averaging/map_averaging.cpp
MODULES_INCLUDE += \#include "map_averaging.h"\n
MODULES_LIST += MapAveragingModule,

DDEFS += -DMODULE_MAP_AVERAGING
