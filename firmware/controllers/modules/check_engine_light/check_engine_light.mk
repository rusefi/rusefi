ifeq ($(MODULE_CHECK_ENGINE_LIGHT),yes)
MODULES_INC += $(PROJECT_DIR)/controllers/modules/check_engine_light
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/check_engine_light/check_engine_light.cpp
DDEFS += -DMODULE_CHECK_ENGINE_LIGHT
endif
