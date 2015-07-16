
DEV_SRC = development/test/test.c \
	development/test/testbmk.c
	
DEV_SRC_CPP = development/trigger_emulator.cpp \
	development/hw_layer/poten.cpp \
	development/sensor_chart.cpp \
	development/rfi_perftest.cpp \
	development/engine_emulator.cpp \
	$(PROJECT_DIR)/development/engine_sniffer.cpp \
	development/wave_analyzer.cpp
	
DEV_SIMULATOR_SRC_CPP = $(PROJECT_DIR)/development/engine_sniffer.cpp