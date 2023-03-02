DEVELOPMENT_DIR=$(PROJECT_DIR)/development

DEV_SRC_CPP = $(DEVELOPMENT_DIR)/sensor_chart.cpp \
	$(DEVELOPMENT_DIR)/engine_emulator.cpp \
	$(DEVELOPMENT_DIR)/engine_sniffer.cpp \
	$(DEVELOPMENT_DIR)/logic_analyzer.cpp \
	$(DEVELOPMENT_DIR)/development/perf_trace.cpp
	
DEV_SIMULATOR_SRC_CPP = $(DEVELOPMENT_DIR)/engine_sniffer.cpp