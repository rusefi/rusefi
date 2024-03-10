HW_SENSORS_DIR=$(PROJECT_DIR)/hw_layer/sensors

HW_SENSORS_INC=$(HW_SENSORS_DIR)

HW_SENSORS_CPP = \
	$(HW_SENSORS_DIR)/yaw_rate_sensor.cpp \
	$(HW_SENSORS_DIR)/accelerometer.cpp \
	$(HW_SENSORS_DIR)/lps25.cpp \
	$(HW_SENSORS_DIR)/max3185x.cpp \
	$(HW_SENSORS_DIR)/gps_uart.cpp
