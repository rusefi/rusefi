
HW_MASS_STORAGE_SRC_C = $(PROJECT_DIR)/ChibiOS-Contrib/os/various/lib_scsi.c \
						ext/uzlib/src/tinflate.c \
						ext/uzlib/src/tinfgzip.c

ALLINC += $(PROJECT_DIR)/ext/uzlib/src

ALLCPPSRC += $(PROJECT_DIR)/hw_layer/mass_storage/null_device.cpp \
			 $(PROJECT_DIR)/hw_layer/mass_storage/compressed_block_device.cpp \
			 $(PROJECT_DIR)/hw_layer/mass_storage/mass_storage_device.cpp \
			 $(PROJECT_DIR)/hw_layer/mass_storage/mass_storage_init.cpp \
