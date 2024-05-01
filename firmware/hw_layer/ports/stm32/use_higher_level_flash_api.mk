# this header is relevant both for modern driver to access internal flash, and for external flash persistence
# see also 'HAL_USE_EFL' in case of internal flash

# do not use legacy implementation to persist calibrations
DDEFS += -DEFI_STORAGE_INT_FLASH=FALSE
# use higher level API instead
DDEFS += -DEFI_STORAGE_MFS=TRUE
