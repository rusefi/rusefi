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
	$(CONTROLLERS_DIR)/actuators/ac_control.cpp \
	$(CONTROLLERS_DIR)/actuators/alternator_controller.cpp \
	$(CONTROLLERS_DIR)/actuators/boost_control.cpp \
	$(CONTROLLERS_DIR)/actuators/dc_motors.cpp \
	$(CONTROLLERS_DIR)/actuators/fan_control.cpp \
	$(CONTROLLERS_DIR)/actuators/fuel_pump.cpp \
	$(CONTROLLERS_DIR)/actuators/idle_thread_io.cpp \
	$(CONTROLLERS_DIR)/actuators/idle_hardware.cpp \
	$(CONTROLLERS_DIR)/actuators/idle_thread.cpp \
	$(CONTROLLERS_DIR)/actuators/pwm_tester.cpp \
	$(CONTROLLERS_DIR)/actuators/vvt.cpp \
	$(CONTROLLERS_DIR)/actuators/gppwm/gppwm_channel.cpp \
	$(CONTROLLERS_DIR)/actuators/gppwm/gppwm.cpp \
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
	$(CONTROLLERS_DIR)/engine_cycle/high_pressure_fuel_pump.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/rpm_calculator.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/spark_logic.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/knock_logic.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/main_trigger_callback.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/aux_valves.cpp \
	$(CONTROLLERS_DIR)/engine_cycle/fuel_schedule.cpp \
	$(CONTROLLERS_DIR)/flash_main.cpp \
	$(CONTROLLERS_DIR)/bench_test.cpp \
	$(CONTROLLERS_DIR)/can/obd2.cpp \
	$(CONTROLLERS_DIR)/can/can_verbose.cpp \
	$(CONTROLLERS_DIR)/can/can_rx.cpp \
	$(CONTORLLERS_DIR)/can/rusefi_wideband.cpp \
	$(CONTROLLERS_DIR)/can/can_tx.cpp \
	$(CONTROLLERS_DIR)/can/can_dash.cpp \
	$(CONTROLLERS_DIR)/can/can_vss.cpp \
 	$(CONTROLLERS_DIR)/engine_controller.cpp \
 	$(CONTROLLERS_DIR)/engine_controller_misc.cpp \
	$(CONTROLLERS_DIR)/persistent_store.cpp \
	$(CONTROLLERS_DIR)/serial/serial_rx.cpp \
	$(CONTROLLERS_DIR)/serial/serial_sensor.cpp \
	$(CONTROLLERS_DIR)/buttonshift.cpp \
	$(CONTROLLERS_DIR)/tcu.cpp \
	$(CONTROLLERS_DIR)/gear_controller.cpp \
	$(CONTROLLERS_DIR)/start_stop.cpp \
	$(CONTROLLERS_DIR)/simple_tcu.cpp \
	$(CONTROLLERS_DIR)/limp_manager.cpp \

CONTROLLERS_INC=\
	$(CONTROLLERS_DIR) \
	$(CONTROLLERS_DIR)/system \
	$(CONTROLLERS_DIR)/system/timer \
	$(CONTROLLERS_DIR)/algo \
	$(CONTROLLERS_DIR)/algo/airmass \
	$(CONTROLLERS_DIR)/algo/defaults \
	$(CONTROLLERS_DIR)/algo/fuel \
	$(CONTROLLERS_DIR)/engine_cycle \
	$(CONTROLLERS_DIR)/trigger/decoders \
	$(CONTROLLERS_DIR)/trigger \
    $(CONTROLLERS_DIR)/sensors \
	$(CONTROLLERS_DIR)/sensors/converters \
	$(CONTROLLERS_DIR)/can \
	$(CONTROLLERS_DIR)/core \
	$(CONTROLLERS_DIR)/gauges \
	$(CONTROLLERS_DIR)/math \
	$(CONTROLLERS_DIR)/generated \
	$(CONTROLLERS_DIR)/actuators \
	$(CONTROLLERS_DIR)/actuators/gppwm \
	$(CONTROLLERS_DIR)/serial \

