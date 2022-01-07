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
	$(UTILSRC) \
	$(CONTROLLERS_ALGO_SRC) \
	$(CONTROLLERS_CORE_SRC) \
	$(CONTROLLERS_MATH_SRC) \
	$(CONTROLLERS_SENSORS_SRC) \
	$(ENGINES_SRC) \
	$(HW_LAYER_DRIVERS_CORE) \
	$(TEST_SRC_C)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC += $(ALLCPPSRC) \
	$(UTILSRC_CPP) \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(TRIGGER_DECODERS_SRC_CPP) \
	$(ENGINES_SRC_CPP) \
	$(CONTROLLERS_SRC_CPP) \
	$(HW_LAYER_DRIVERS_CORE_CPP) \
	$(CONTROLLERS_CORE_SRC_CPP) \
	$(CONTROLLERS_MATH_SRC_CPP) \
	$(CONTROLLERS_SENSORS_SRC_CPP) \
	$(FRAMEWORK_SRC_CPP) \
	$(TESTS_SRC_CPP) \
	$(SYSTEMSRC_CPP) \
	$(HW_LAYER_EMS_CPP) \
	$(TUNERSTUDIO_SRC_CPP) \
	$(HW_SENSORS_CPP) \
	$(TRIGGER_SRC_CPP) \
	$(INIT_SRC_CPP) \
	$(PROJECT_DIR)/../unit_tests/logicdata.cpp \
	$(DEVELOPMENT_DIR)/engine_sniffer.cpp \
	$(PROJECT_DIR)/../unit_tests/main.cpp \
	$(PROJECT_DIR)/../unit_tests/global_mocks.cpp \
	$(PROJECT_DIR)/console/binary/tooth_logger.cpp \
	$(PROJECT_DIR)/console/binary_log/log_field.cpp \
	$(PROJECT_DIR)/../unit_tests/mocks.cpp \

INCDIR += \
	$(PCH_DIR) \
	$(UNIT_TESTS_DIR) \
	$(ALLINC) \
	$(HW_SENSORS_INC) \
	$(UNIT_TESTS_DIR)/test_data_structures \
	$(UNIT_TESTS_DIR)/chibios-mock \
	$(UNIT_TESTS_DIR)/tests \
	$(UNIT_TESTS_DIR)/tests/sensor \
	$(UNIT_TESTS_DIR)/test_basic_math
	

include $(UNIT_TESTS_DIR)/unit_test_rules.mk
