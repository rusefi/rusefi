# Configure libfirmware Paths/Includes
RUSEFI_LIB = $(PROJECT_DIR)/libfirmware
include $(RUSEFI_LIB)/util/util.mk
include $(RUSEFI_LIB)/pt2001/pt2001.mk
include $(RUSEFI_LIB)/sent/sent.mk

INCDIR += $(RUSEFI_LIB)/sent/include
