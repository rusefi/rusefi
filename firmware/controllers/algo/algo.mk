
CONTROLLERS_ALGO_SRC = $(PROJECT_DIR)/controllers/algo/map_adjuster.c \
	$(PROJECT_DIR)/controllers/algo/signal_executor.c \
	$(PROJECT_DIR)/controllers/algo/malfunction_central.c \
	$(PROJECT_DIR)/controllers/algo/nmea.c
	
CONTROLLERS_ALGO_SRC_CPP = $(PROJECT_DIR)/controllers/algo/OutputSignalArray.cpp \
	$(PROJECT_DIR)/controllers/algo/advance_map.cpp \
	$(PROJECT_DIR)/controllers/algo/idle_controller.cpp \
	$(PROJECT_DIR)/controllers/algo/auto_generated_enums.cpp \
	$(PROJECT_DIR)/controllers/algo/fuel_math.cpp \
	$(PROJECT_DIR)/controllers/algo/accel_enrichment.cpp \
	$(PROJECT_DIR)/controllers/algo/engine_configuration.cpp \
	$(PROJECT_DIR)/controllers/algo/engine.cpp \
	$(PROJECT_DIR)/controllers/algo/event_registry.cpp \
	$(PROJECT_DIR)/controllers/algo/algo.cpp \
	$(PROJECT_DIR)/controllers/algo/wave_chart.cpp \
	$(PROJECT_DIR)/controllers/algo/enum_strings.cpp
	
