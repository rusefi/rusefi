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

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC += $(ALLCSRC) \
	$(HW_LAYER_DRIVERS_CORE) \
	$(TEST_SRC_C)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC += $(ALLCPPSRC) \
	$(HW_LAYER_DRIVERS_CORE_CPP) \
	$(FRAMEWORK_SRC_CPP) \
	$(TESTS_SRC_CPP) \
	$(DEVELOPMENT_DIR)/engine_sniffer.cpp \
	$(PROJECT_DIR)/console/binary/tooth_logger.cpp \
	$(PROJECT_DIR)/console/binary_log/log_field.cpp \
	$(PROJECT_DIR)/config/boards/hellen/hellen_board_id.cpp \
	$(PROJECT_DIR)/../unit_tests/logicdata.cpp \
	$(PROJECT_DIR)/../unit_tests/native/native_impl.cpp \
	$(PROJECT_DIR)/../unit_tests/main.cpp \
	$(PROJECT_DIR)/../unit_tests/global_mocks.cpp \
	$(PROJECT_DIR)/../unit_tests/mocks.cpp \

INCDIR += \
	$(PCH_DIR) \
	$(UNIT_TESTS_DIR) \
	$(ALLINC) \
	$(PROJECT_DIR)/config/boards/hellen \
	$(UNIT_TESTS_DIR)/test_data_structures \
	$(UNIT_TESTS_DIR)/chibios-mock \
	$(UNIT_TESTS_DIR)/tests \
	$(UNIT_TESTS_DIR)/tests/sensor \
	$(UNIT_TESTS_DIR)/test_basic_math
	

include $(UNIT_TESTS_DIR)/unit_test_rules.mk
