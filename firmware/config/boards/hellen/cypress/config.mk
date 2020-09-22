
USE_PROCESS_STACKSIZE = 1536
USE_EXCEPTIONS_STACKSIZE = 4096

CYPRESS_CONTRIB = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/OS

CPU_STARTUP_DIR = $(CYPRESS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_S6E2CxAH.mk
CPU_PLATFORM_DIR = $(CYPRESS_CONTRIB)/os/hal/ports/Cypress/S6E2CxAH/platform.mk
CPU_HWLAYER = ports/cypress

GENERATED_ENUMS_DIR = $(BOARD_DIR)/config/controllers/algo

EXTRA_PARAMS += -DFIRMWARE_ID=\"cypress\"
# -nodefaultlibs -lc -lgcc -ltinyc

# used by USE_SMART_BUILD
CONFDIR = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)
