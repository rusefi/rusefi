
CONTROLLERSSRC = \
	controllers/ignition_central.c \
	$(PROJECT_DIR)/controllers/malfunction_indicator.c \
	$(PROJECT_DIR)/controllers/error_handling.c
	
CONTROLLERS_SRC_CPP = $(PROJECT_DIR)/controllers/settings.cpp \
	controllers/electronic_throttle.cpp \
	controllers/map_averaging.cpp \
	controllers/map_multiplier_thread.cpp \
	controllers/flash_main.cpp \
	controllers/injector_central.cpp \
	controllers/idle_thread.cpp \
	controllers/PwmTester.cpp \
	$(PROJECT_DIR)/controllers/alternatorController.cpp \
	$(PROJECT_DIR)/controllers/lcd_controller.cpp \
 	$(PROJECT_DIR)/controllers/engine_controller.cpp
