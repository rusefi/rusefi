DEVELOPMENT_DIR=$(PROJECT_DIR)/development

DEV_SRC = $(DEVELOPMENT_DIR)/test/test.c \
	$(DEVELOPMENT_DIR)/test/testbmk.c

DEV_SRC_CPP = $(DEVELOPMENT_DIR)/hw_layer/poten.cpp \
	$(DEVELOPMENT_DIR)/sensor_chart.cpp \
	$(DEVELOPMENT_DIR)/rfi_perftest.cpp \
	$(DEVELOPMENT_DIR)/engine_emulator.cpp \
	$(DEVELOPMENT_DIR)/engine_sniffer.cpp \
	$(DEVELOPMENT_DIR)/logic_analyzer.cpp \
	$(DEVELOPMENT_DIR)/development/perf_trace.cpp
