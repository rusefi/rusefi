
CONTROLLERSSRC = \
	controllers/electronic_throttle.c \
	controllers/idle_thread.c \
	controllers/injector_central.c \
	controllers/ignition_central.c \
	controllers/flash_main.c \
	$(PROJECT_DIR)/controllers/malfunction_indicator.c \
	$(PROJECT_DIR)/controllers/error_handling.c \
	controllers/map_averaging.c \
	controllers/map_multiplier_thread.c \
	$(PROJECT_DIR)/controllers/alternatorController.c
	
CONTROLLERS_SRC_CPP = $(PROJECT_DIR)/controllers/settings.cpp \
 	$(PROJECT_DIR)/controllers/engine_controller.cpp
