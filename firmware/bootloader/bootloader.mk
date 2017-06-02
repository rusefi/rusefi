# Added if USE_BOOTLOADER=yes in Makefile
BOOTLOADERSRC= $(PROJECT_DIR)/bootloader/bootloader_storage.c

ifeq ($(BOOTLOADERINC),)
	BOOTLOADERINC=$(PROJECT_DIR)/bootloader
endif

ifeq ($(LDSCRIPT),)
	LDSCRIPT= config/stm32f4ems/STM32F407xG_CCM_bootloader.ld
endif

# Add bootloader code to the firmware
DDEFS += -DEFI_BOOTLOADER_INCLUDE_CODE=TRUE

# Relocate interrupt vectors for the new application start address
DDEFS += -DCORTEX_VTOR_INIT=0x8000
