FLASH_DIR=$(PROJECT_DIR)/hw_layer/drivers/flash

# List of all the Micron N25Q device files.
SNORSRC := $(CHIBIOS)/os/hal/lib/complex/serial_nor/hal_serial_nor.c \
           $(FLASH_DIR)/hal_flash_device.c

# Required include directories
SNORINC := $(CHIBIOS)/os/hal/lib/complex/serial_nor \
           $(FLASH_DIR)

# Shared variables
ALLCSRC += $(SNORSRC)
ALLINC  += $(SNORINC)
