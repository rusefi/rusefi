CONTROLLERS_DIR=$(PROJECT_DIR)/controllers

CONTROLLERSSRC =
	
CONTROLLERS_SRC_CPP = \
	$(CONTROLLERS_DIR)/actuators/electronic_throttle.cpp \
	$(CONTROLLERS_DIR)/actuators/alternator_controller.cpp \
	$(CONTROLLERS_DIR)/actuators/malfunction_indicator.cpp \
	$(CONTROLLERS_DIR)/actuators/tachometer.cpp \
	$(PROJECT_DIR)/controllers/settings.cpp \
	$(PROJECT_DIR)/controllers/error_handling.cpp \
	$(PROJECT_DIR)/controllers/map_averaging.cpp \
	$(PROJECT_DIR)/controllers/flash_main.cpp \
	$(PROJECT_DIR)/controllers/injector_central.cpp \
	$(PROJECT_DIR)/controllers/idle_thread.cpp \
	$(PROJECT_DIR)/controllers/pwm_tester.cpp \
	$(PROJECT_DIR)/controllers/obd2.cpp \
	$(PROJECT_DIR)/controllers/lcd_controller.cpp \
 	$(PROJECT_DIR)/controllers/engine_controller.cpp \
	$(PROJECT_DIR)/controllers/persistent_store.cpp

CONTROLLERS_INC=\
	$(CONTROLLERS_DIR) \
	$(CONTROLLERS_DIR)/actuators \

