# List of all the board related files.
BOARDCPPSRC = $(PROJECT_DIR)/config/boards/nucleo_f767/board_configuration.cpp

# reducing flash consumption for EFI_ETHERNET to fit
DDEFS += -DEFI_FILE_LOGGING=FALSE -DEFI_ALTERNATOR_CONTROL=FALSE -DEFI_LOGIC_ANALYZER=FALSE -DEFI_ENABLE_ASSERTS=FALSE

LWIP = yes
DDEFS += -DEFI_ETHERNET=TRUE

DDEFS += -DSTM32F767xx

