CONTROLLERS_DIR=$(PROJECT_DIR)/controllers

CONTROLLERSSRC =
	
CONTROLLERS_SRC_CPP = \
	$(CONTROLLERS_DIR)/actuators/electronic_throttle.cpp \
	$(CONTROLLERS_DIR)/actuators/alternator_controller.cpp \
	$(CONTROLLERS_DIR)/actuators/malfunction_indicator.cpp \
	$(CONTROLLERS_DIR)/actuators/tachometer.cpp \
	$(CONTROLLERS_DIR)/actuators/idle_thread.cpp \
	$(CONTROLLERS_DIR)/actuators/pwm_tester.cpp \
	$(CONTROLLERS_DIR)/actuators/algo/aux_pid.cpp \
	$(CONTROLLERS_DIR)/actuators/lcd_controller.cpp \
	$(CONTROLLERS_DIR)/scheduling/signal_executor_sleep.cpp \
	$(CONTROLLERS_DIR)/scheduling/signal_executor.cpp \
	$(CONTROLLERS_DIR)/scheduling/single_timer_executor.cpp \
	$(CONTROLLERS_DIR)/scheduling/pwm_generator_logic.cpp \
	$(CONTROLLERS_DIR)/scheduling/event_queue.cpp \
	$(PROJECT_DIR)/controllers/settings.cpp \
	$(PROJECT_DIR)/controllers/core/error_handling.cpp \
	$(PROJECT_DIR)/controllers/map_averaging.cpp \
	$(PROJECT_DIR)/controllers/flash_main.cpp \
	$(PROJECT_DIR)/controllers/injector_central.cpp \
	$(PROJECT_DIR)/controllers/obd2.cpp \
 	$(PROJECT_DIR)/controllers/engine_controller.cpp \
	$(PROJECT_DIR)/controllers/persistent_store.cpp \
	

CONTROLLERS_INC=\
	$(CONTROLLERS_DIR) \
	$(CONTROLLERS_DIR)/scheduling \
	$(CONTROLLERS_DIR)/actuators \

