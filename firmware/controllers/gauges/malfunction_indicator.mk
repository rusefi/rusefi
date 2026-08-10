ifeq ($(MODULE_MIL),yes)
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/gauges/malfunction_indicator.cpp
DDEFS += -DMODULE_MIL
endif
