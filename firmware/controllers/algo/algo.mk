
CONTROLLERS_ALGO_SRC = $(PROJECT_DIR)/controllers/algo/map_adjuster.c \
	$(PROJECT_DIR)/controllers/algo/advance_map.c \
	$(PROJECT_DIR)/controllers/algo/engine_configuration.c \
	$(PROJECT_DIR)/controllers/algo/signal_executor.c \
	$(PROJECT_DIR)/controllers/algo/interpolation.c \
	$(PROJECT_DIR)/controllers/algo/malfunction_central.c \
	$(PROJECT_DIR)/controllers/algo/event_registry.c \
	$(PROJECT_DIR)/controllers/algo/idle_controller.c \
	$(PROJECT_DIR)/controllers/algo/wave_chart.c \
	$(PROJECT_DIR)/controllers/algo/nmea.c \
	$(PROJECT_DIR)/controllers/algo/algo.c \
	$(PROJECT_DIR)/controllers/algo/signal_executor_single_timer_algo.c
	
CONTROLLERS_ALGO_SRC_CPP = $(PROJECT_DIR)/controllers/algo/OutputSignalArray.cpp \
	$(PROJECT_DIR)/controllers/algo/fuel_math.cpp \
	$(PROJECT_DIR)/controllers/algo/accel_enrichmemnt.cpp \
	$(PROJECT_DIR)/controllers/algo/event_queue.cpp
	
