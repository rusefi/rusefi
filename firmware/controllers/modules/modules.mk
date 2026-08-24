# auto-gen files
MODULES_INC += $(PROJECT_DIR)/controllers/modules/generated

# board module stubs - shared between prod firmware, simulator and unit_tests
MODULES_INC += $(PROJECT_DIR)/controllers/modules/stubs

MODULE_CHECK_ENGINE_LIGHT ?= yes
MODULE_MIL ?= yes
MODULE_DTC_MANAGER ?= yes

ifeq ($(MODULE_MIL),yes)
ifneq ($(MODULE_CHECK_ENGINE_LIGHT),yes)
$(error MODULE_MIL requires MODULE_CHECK_ENGINE_LIGHT=yes)
endif
endif

# user defined modules
include $(PROJECT_DIR)/controllers/modules/tachometer/tachometer.mk
include $(PROJECT_DIR)/controllers/modules/fuel_pump/fuel_pump.mk
include $(PROJECT_DIR)/controllers/modules/gear_detector/gear_detector.mk
include $(PROJECT_DIR)/controllers/modules/trip_odometer/trip_odometer.mk
include $(PROJECT_DIR)/controllers/modules/fan_control/fan_control.mk
include $(PROJECT_DIR)/controllers/modules/map_averaging/map_averaging.mk
include $(PROJECT_DIR)/controllers/modules/ethernet_console/ethernet_console.mk
include $(PROJECT_DIR)/controllers/modules/example_module/example_module.mk
include $(PROJECT_DIR)/controllers/modules/vvl_controller/vvl_controller.mk
include $(PROJECT_DIR)/controllers/modules/check_engine_light/check_engine_light.mk
include $(PROJECT_DIR)/controllers/modules/malfunction_indicator/malfunction_indicator.mk
include $(PROJECT_DIR)/controllers/modules/sd_log_trigger/sd_log_trigger.mk
include $(PROJECT_DIR)/controllers/modules/configuration_wizard/configuration_wizard.mk
include $(PROJECT_DIR)/controllers/modules/dtc_manager/dtc_manager.mk
include $(PROJECT_DIR)/controllers/modules/injector_deadtime_autotune/injector_deadtime_autotune.mk
