#
# do we have a defect below when we allow code to overflow above 1MB? would the fix be to set
# EFI_FLASH_SIZE to 1Mb minus bootloader size?
#

# Configure the linker script to use more flash for this board (2Mb assumed)
USE_OPT += -Wl,--defsym=EFI_FLASH_SIZE=1536k
# affects config placement
DDEFS += -DEFI_FLASH_USE_1500_OF_2MB
