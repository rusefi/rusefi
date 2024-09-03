include $(PROJECT_DIR)/controllers/modules/fan/fan.mk
include $(PROJECT_DIR)/controllers/modules/fuel_pump/fuel_pump.mk
include $(PROJECT_DIR)/controllers/modules/gear_detector/gear_detector.mk
include $(PROJECT_DIR)/controllers/modules/tachometer/tachometer.mk
include $(PROJECT_DIR)/controllers/modules/trip_odometer/trip_odometer.mk

ifeq ($(NO_MAP_AVERAGING),)
  include $(PROJECT_DIR)/controllers/modules/map_averaging/map_averaging.mk
endif
