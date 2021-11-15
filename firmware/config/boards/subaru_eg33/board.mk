BOARD_DIR = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)

# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# Required include directories
BOARDINC = $(BOARD_DIR)
BOARDINC += $(BOARD_DIR)/config/controllers/algo

# Override LD script
ifeq ($(USE_BOOTLOADER),yes)
  # include Prometheus bootloader code
  BOOTLOADERINC = $(PROJECT_DIR)/bootloader/subaru_eg33
endif

#LED
DDEFS +=  -DLED_CRITICAL_ERROR_BRAIN_PIN=GPIOG_7

# We are running on Subaru EG33 hardware!
DDEFS += -DHW_SUBARU_EG33=1

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DDEFAULT_ENGINE_TYPE=SUBARUEG33_DEFAULTS

#Some options override
DDEFS += -DHAL_USE_UART=FALSE
DDEFS += -DUART_USE_WAIT=FALSE

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)

#Serial flash support
include $(PROJECT_DIR)/hw_layer/drivers/flash/sst26f_jedec.mk
