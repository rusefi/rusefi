ifeq ($(MODULE_CHECK_ENGINE_LIGHT),yes)
MODULES_INC += $(PROJECT_DIR)/controllers/modules/check_engine_light
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/check_engine_light/check_engine_light.cpp
MODULES_INCLUDE += \#include "check_engine_light.h"\n
MODULES_LIST += CheckEngineLight,
DDEFS += -DMODULE_CHECK_ENGINE_LIGHT
endif
