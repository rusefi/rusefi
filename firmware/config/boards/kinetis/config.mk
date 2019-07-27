
#default=1536
USE_PROCESS_STACKSIZE = 850
#default=4096
USE_EXCEPTIONS_STACKSIZE = 2400

KINETIS_CONTRIB = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/OS

CPU_STARTUP_DIR = $(KINETIS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_ke1xf.mk
CPU_PLATFORM_DIR = $(KINETIS_CONTRIB)/os/hal/ports/KINETIS/KE1xF/platform.mk
CPU_HWLAYER = ports/kinetis

GENERATED_ENUMS_DIR = $(BOARD_DIR)/config/controllers/algo

EXTRA_PARAMS += -nodefaultlibs -L$(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/libc -lgcc -ltinyc
