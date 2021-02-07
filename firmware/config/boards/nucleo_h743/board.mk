# List of all the board related files.

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/nucleo_h743 $(PROJECT_DIR)/config/stm32h7ems
CONFDIR = config/stm32h7ems

LDSCRIPT  = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/ld/STM32H743xI.ld

# Shared variables
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
