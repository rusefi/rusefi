# List of all the board related files.
BOARDSRC = $(BOARD_DIR)/../board_extra.c

BOARDCPPSRC = $(BOARD_DIR)/../board_configuration.cpp

BOARDINC += $(BOARD_DIR)/..

# Required include directories
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
