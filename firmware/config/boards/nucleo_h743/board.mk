# List of all the board related files.

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/stm32h7ems
CONFDIR = config/stm32h7ems

# Shared variables
ALLCPPSRC += $(PROJECT_DIR)/config/boards/nucleo_h743/board_configuration.cpp
ALLINC    += $(BOARDINC)
