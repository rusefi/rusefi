# List of all the board related files.
BOARDSRC = $(PROJECT_DIR)/config/boards/prometheus/board_extra.c

BOARDCPPSRC = $(PROJECT_DIR)/config/boards/Prometheus/board_configuration.cpp

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/prometheus

# This board uses bootloader
USE_BOOTLOADER = yes

# include Prometheus bootloader code
BOOTLOADERINC = $(PROJECT_DIR)/bootloader/prometheus/$(PROMETHEUS_BOARD)

# Default to a release build - clear EXTRA_PARAMS from cmdline to build debug
ifeq ($(EXTRA_PARAMS),)
	EXTRA_PARAMS = -DSHORT_BOARD_NAME=pth -DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE
endif
ifeq ($(DEBUG_LEVEL_OPT),)
	DEBUG_LEVEL_OPT = -O2
endif

ifeq ($(PROMETHEUS_BOARD),405)
	DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS -DFIRMWARE_ID=\"prometeus405\"
else
	# Override DEFAULT_ENGINE_TYPE
	DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS -DFIRMWARE_ID=\"prometeus469\"
endif

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)