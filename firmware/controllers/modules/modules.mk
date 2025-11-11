# auto-gen files
MODULES_INC += $(PROJECT_DIR)/controllers/modules/generated

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
include $(PROJECT_DIR)/controllers/modules/configuration_wizard/configuration_wizard.mk