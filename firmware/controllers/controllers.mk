include $(PROJECT_DIR)/controllers/algo/algo.mk
include $(PROJECT_DIR)/controllers/core/core.mk
include $(PROJECT_DIR)/controllers/math/math.mk
include $(PROJECT_DIR)/controllers/trigger/trigger.mk
include $(PROJECT_DIR)/controllers/sensors/sensors.mk
include $(PROJECT_DIR)/controllers/system/system.mk
#include $(PROJECT_DIR)/controllers/gauges/gauges.mk

CONTROLLERS_DIR=$(PROJECT_DIR)/controllers

CONTROLLERSSRC =
	
CONTROLLERS_SRC_CPP = \
	$(CONTROLLERS_DIR)/actuators/electronic_throttle.cpp \
	$(CONTROLLERS_DIR)/actuators/alternator_controller.cpp \
	$(CONTROLLERS_DIR)/actuators/idle_thread.cpp \
	$(CONTROLLERS_DIR)/actuators/pwm_tester.cpp \
	$(CONTROLLERS_DIR)/actuators/algo/aux_pid.cpp \
	$(CONTROLLERS_DIR)/gauges/tachometer.cpp \
	$(CONTROLLERS_DIR)/gauges/malfunction_indicator.cpp \
	$(CONTROLLERS_DIR)/gauges/lcd_controller.cpp \
	$(CONTROLLERS_DIR)/system/timer/signal_executor_sleep.cpp \
	$(CONTROLLERS_DIR)/system/timer/single_timer_executor.cpp \
	$(CONTROLLERS_DIR)/system/timer/pwm_generator_logic.cpp \
	$(CONTROLLERS_DIR)/system/timer/event_queue.cpp \
	$(CONTROLLERS_DIR)/settings.cpp \
	$(CONTROLLERS_DIR)/core/error_handling.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/map_averaging.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/rpm_calculator.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/spark_logic.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/main_trigger_callback.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/aux_valves.cpp \
	$(CONTROLLERS_DIR)/flash_main.cpp \
	$(CONTROLLERS_DIR)/injector_central.cpp \
	$(CONTROLLERS_DIR)/obd2.cpp \
 	$(CONTROLLERS_DIR)/engine_controller.cpp \
	$(CONTROLLERS_DIR)/persistent_store.cpp \
	

CONTROLLERS_INC=\
	$(CONTROLLERS_DIR) \
	$(CONTROLLERS_DIR)/system \
	$(CONTROLLERS_DIR)/system/timer \
	$(CONTROLLERS_DIR)/algo \
	$(CONTROLLERS_DIR)/engine_cycle \
	$(CONTROLLERS_DIR)/trigger/decoders \
	$(CONTROLLERS_DIR)/trigger \
    $(CONTROLLERS_DIR)/sensors \
	$(CONTROLLERS_DIR)/sensors/converters \
	$(CONTROLLERS_DIR)/core \
	$(CONTROLLERS_DIR)/gauges \
	$(CONTROLLERS_DIR)/math \
	$(CONTROLLERS_DIR)/generated \
	$(CONTROLLERS_DIR)/actuators \

