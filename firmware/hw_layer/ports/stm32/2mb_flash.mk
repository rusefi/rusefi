# Configure the linker script to use more flash for this board (2Mb assumed)
USE_OPT += -Wl,--defsym=EFI_FLASH_SIZE=1536k
# affects config placement
DDEFS += -DEFI_FLASH_USE_1500_OF_2MB
