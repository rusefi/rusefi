# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/skeleton/board_configuration.cpp

# Target processor details
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
  BOARDINC  = $(BOARDS_DIR)/skeleton
endif

DDEFS += -DDEFAULT_ENGINE_TYPE=DEFAULT_FRANKENSO
# Add them all together
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::E3 -DFIRMWARE_ID=\"skeleton\"
