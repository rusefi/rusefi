MASS_STORAGE_DIR=$(HW_LAYER_DIR)/mass_storage

HW_MASS_STORAGE_INC = \
	$(PROJECT_DIR)/ext/uzlib/src

HW_MASS_STORAGE_SRC = \
	$(PROJECT_DIR)/ChibiOS-Contrib/os/various/lib_scsi.c \
	$(PROJECT_DIR)/ext/uzlib/src/tinflate.c \
	$(PROJECT_DIR)/ext/uzlib/src/tinfgzip.c

HW_MASS_STORAGE_CPP = \
	$(MASS_STORAGE_DIR)/null_device.cpp \
	$(MASS_STORAGE_DIR)/compressed_block_device.cpp \
	$(MASS_STORAGE_DIR)/mass_storage_device.cpp \
	$(MASS_STORAGE_DIR)/mass_storage_init.cpp
