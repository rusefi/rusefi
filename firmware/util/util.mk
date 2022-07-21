UTIL_DIR=$(PROJECT_DIR)/util

UTILSRC = \
	$(UTIL_DIR)/tinymt32.c \

UTILSRC_CPP = \
	$(UTIL_DIR)/histogram.cpp \
	$(UTIL_DIR)/containers/listener_array.cpp \
	$(UTIL_DIR)/containers/local_version_holder.cpp \
	$(UTIL_DIR)/math/biquad.cpp \
	$(UTIL_DIR)/math/error_accumulator.cpp \
	$(UTIL_DIR)/math/efi_pid.cpp \
	$(UTIL_DIR)/math/interpolation.cpp \
	$(PROJECT_DIR)/util/datalogging.cpp \
	$(PROJECT_DIR)/util/loggingcentral.cpp \
	$(PROJECT_DIR)/util/cli_registry.cpp \
	$(PROJECT_DIR)/util/efilib.cpp \
	$(PROJECT_DIR)/util/timer.cpp \
	$(UTIL_DIR)/os_util.cpp \
	
	
UTIL_INC = \
	$(UTIL_DIR) \
	$(UTIL_DIR)/containers \
	$(UTIL_DIR)/math \
	
