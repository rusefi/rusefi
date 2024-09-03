MODULES_INC += $(PROJECT_DIR)/controllers/modules/tachometer
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/tachometer/tachometer.cpp
MODULES_INCLUDE += \#include "tachometer.h"\n
MODULES_LIST += TachometerModule,

DDEFS += -DMODULE_TACHOMETER
