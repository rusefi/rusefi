MODULES_INC += $(PROJECT_DIR)/controllers/modules/gear_detector
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/gear_detector/gear_detector.cpp
MODULES_INCLUDE += \#include "gear_detector.h"\n
MODULES_LIST += GearDetector,

DDEFS += -DMODULE_GEAR_DETECT
