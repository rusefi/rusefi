MODULES_INC += $(PROJECT_DIR)/controllers/modules/configuration_wizard
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/configuration_wizard/configuration_wizard.cpp
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/configuration_wizard/strategies/vin_strategy.cpp

# not used until we are inside EngineModule
#MODULES_INCLUDE += \#include "configuration_wizard.h"\n
#MODULES_LIST += ConfigurationWizard,

DDEFS += -DMODULE_CONFIGURATION_WIZARD