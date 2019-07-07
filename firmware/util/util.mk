UTIL_DIR=$(PROJECT_DIR)/util

UTILSRC = \
	$(UTIL_DIR)/containers/data_buffer.c \
	$(UTIL_DIR)/math/crc.c \
	$(UTIL_DIR)/os_util.c \
	$(UTIL_DIR)/histogram.c

UTILSRC_CPP = \
    $(UTIL_DIR)/containers/cyclic_buffer.cpp \
	$(UTIL_DIR)/containers/listener_array.cpp \
	$(UTIL_DIR)/containers/counter64.cpp \
	$(UTIL_DIR)/containers/local_version_holder.cpp \
	$(UTIL_DIR)/containers/table_helper.cpp \
	$(UTIL_DIR)/math/pid.cpp \
	$(UTIL_DIR)/math/avg_values.cpp \
	$(UTIL_DIR)/math/interpolation.cpp \
	$(UTIL_DIR)/math/biquad.cpp \
	$(PROJECT_DIR)/util/datalogging.cpp \
	$(PROJECT_DIR)/util/loggingcentral.cpp \
	$(PROJECT_DIR)/util/cli_registry.cpp \
	$(PROJECT_DIR)/util/efilib.cpp \
	
	
UTIL_INC = \
	$(UTIL_DIR) \
	$(UTIL_DIR)/containers \
	$(UTIL_DIR)/math \
	
