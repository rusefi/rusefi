UTIL_DIR=$(PROJECT_DIR)/util

UTILSRC = $(UTIL_DIR)/crc.c \
	$(UTIL_DIR)/containers/data_buffer.c \
	$(UTIL_DIR)/rfiutil.c \
	$(UTIL_DIR)/histogram.c

UTILSRC_CPP = $(UTIL_DIR)/containers/cyclic_buffer.cpp \
	$(PROJECT_DIR)/util/datalogging.cpp \
	$(PROJECT_DIR)/util/loggingcentral.cpp \
	$(UTIL_DIR)/containers/listener_array.cpp \
	$(PROJECT_DIR)/util/cli_registry.cpp \
	$(PROJECT_DIR)/util/pid.cpp \
	$(PROJECT_DIR)/util/biquad.cpp \
	$(PROJECT_DIR)/util/efilib.cpp \
	$(PROJECT_DIR)/util/efilib2.cpp \
	$(UTIL_DIR)/containers/local_version_holder.cpp
	
UTIL_INC = \
	$(UTIL_DIR) \
	$(UTIL_DIR)/containers \
	
