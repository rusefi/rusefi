# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_H743ZI/board.c

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/nucleo_h743 $(PROJECT_DIR)/config/stm32h7ems
CONFDIR = config/stm32h7ems

LDSCRIPT  = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/ld/STM32H743xI.ld

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
