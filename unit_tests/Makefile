##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

PROJECT_DIR = ../firmware
UNIT_TESTS_DIR=$(PROJECT_DIR)/../unit_tests

# Imported source files and paths
include $(PROJECT_DIR)/console/console.mk
include $(PROJECT_DIR)/console/binary/tunerstudio.mk
include $(UNIT_TESTS_DIR)/test.mk
include $(UNIT_TESTS_DIR)/tests/tests.mk
include $(PROJECT_DIR)/../unit_tests/tests/util/test_util.mk
include $(PROJECT_DIR)/common.mk

RUSEFI_LIB = $(PROJECT_DIR)/libfirmware
include $(RUSEFI_LIB)/util/util.mk
include $(RUSEFI_LIB)/pt2001/pt2001.mk

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC += $(ALLCSRC) \
	$(RUSEFI_LIB_C) \
	$(HW_LAYER_DRIVERS_CORE) \
	$(TEST_SRC_C)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC += $(ALLCPPSRC) \
	$(HW_LAYER_DRIVERS_CORE_CPP) \
	$(FRAMEWORK_SRC_CPP) \
	$(TESTS_SRC_CPP) \
	$(DEVELOPMENT_DIR)/engine_sniffer.cpp \
	$(CONSOLE_COMMON_SRC_CPP) \
	$(PROJECT_DIR)/config/boards/hellen/hellen_board_id.cpp \
	$(PROJECT_DIR)/hw_layer/drivers/can/can_hw.cpp \
	$(PROJECT_DIR)/../unit_tests/logicdata.cpp \
	$(PROJECT_DIR)/../unit_tests/native/native_impl.cpp \
	$(PROJECT_DIR)/../unit_tests/main.cpp \
	$(PROJECT_DIR)/../unit_tests/global_mocks.cpp \
	$(PROJECT_DIR)/../unit_tests/mocks.cpp \
	$(RUSEFI_LIB_CPP) \
	$(RUSEFI_LIB_CPP_TEST) \

INCDIR += \
	$(PCH_DIR) \
	$(UNIT_TESTS_DIR) \
	$(ALLINC) \
	$(PROJECT_DIR)/config/boards/hellen \
	$(UNIT_TESTS_DIR)/test_data_structures \
	$(UNIT_TESTS_DIR)/chibios-mock \
	$(UNIT_TESTS_DIR)/native \
	$(UNIT_TESTS_DIR)/tests \
	$(UNIT_TESTS_DIR)/tests/sensor \
	$(UNIT_TESTS_DIR)/test_basic_math \
	$(RUSEFI_LIB_INC) \


include $(UNIT_TESTS_DIR)/unit_test_rules.mk
