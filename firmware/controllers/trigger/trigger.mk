TRIGGER_DECODERS_SRC =

TRIGGER_DECODERS_SRC_CPP = \
	$(CONTROLLERS_DIR)/trigger/decoders/trigger_structure.cpp \
	$(CONTROLLERS_DIR)/trigger/decoders/instant_rpm_calculator.cpp \
	$(CONTROLLERS_DIR)/trigger/decoders/auto_generated_sync_edge.cpp \
	$(CONTROLLERS_DIR)/trigger/trigger_decoder.cpp \
	$(CONTROLLERS_DIR)/trigger/trigger_simulator.cpp \
	$(CONTROLLERS_DIR)/trigger/decoders/trigger_universal.cpp \
	$(CONTROLLERS_DIR)/trigger/decoders/generated_trigger_init.cpp

TRIGGER_SRC_CPP = \
	$(CONTROLLERS_DIR)/trigger/trigger_emulator_algo.cpp \
	$(CONTROLLERS_DIR)/trigger/trigger_central.cpp
