# auto-gen files
MODULES_INC += $(PROJECT_DIR)/controllers/modules/generated

# user defined modules
include $(PROJECT_DIR)/controllers/modules/tachometer/tachometer.mk
include $(PROJECT_DIR)/controllers/modules/fuel_pump/fuel_pump.mk
include $(PROJECT_DIR)/controllers/modules/gear_detector/gear_detector.mk