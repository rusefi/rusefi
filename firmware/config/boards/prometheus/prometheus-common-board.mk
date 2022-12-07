# List of all the board related files.
BOARDSRC = $(PROJECT_DIR)/config/boards/prometheus/board_extra.c

BOARDCPPSRC = $(PROJECT_DIR)/config/boards/Prometheus/board_configuration.cpp

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/prometheus

# Default to a release build - clear EXTRA_PARAMS from cmdline to build debug
ifeq ($(EXTRA_PARAMS),)
	EXTRA_PARAMS = -DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE
endif
ifeq ($(DEBUG_LEVEL_OPT),)
	DEBUG_LEVEL_OPT = -O2
endif

ifeq ($(PROMETHEUS_BOARD),405)
	DDEFS += -DFIRMWARE_ID=\"prometeus405\" -DSHORT_BOARD_NAME=prometheus_405
else
	DDEFS += -DFIRMWARE_ID=\"prometeus469\" -DSHORT_BOARD_NAME=prometheus_469
endif
