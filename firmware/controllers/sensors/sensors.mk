
CONTROLLERS_SENSORS_SRC_CPP = \
	$(PROJECT_DIR)/controllers/sensors/core/functional_sensor.cpp \
	$(PROJECT_DIR)/controllers/sensors/core/sensor.cpp \
 	$(PROJECT_DIR)/controllers/sensors/thermistors.cpp \
	$(PROJECT_DIR)/controllers/sensors/allsensors.cpp \
	$(PROJECT_DIR)/controllers/sensors/auto_generated_sensor.cpp \
	$(PROJECT_DIR)/controllers/sensors/impl/AemXSeriesLambda.cpp \
	$(PROJECT_DIR)/controllers/sensors/impl/map.cpp \
	$(PROJECT_DIR)/controllers/sensors/impl/maf.cpp \
	$(PROJECT_DIR)/controllers/sensors/impl/Lps25Sensor.cpp \
	$(PROJECT_DIR)/controllers/sensors/tps.cpp \
	$(PROJECT_DIR)/controllers/sensors/impl/ego.cpp \
	$(PROJECT_DIR)/controllers/sensors/sensor_info_printing.cpp \
	$(PROJECT_DIR)/controllers/sensors/sensor_checker.cpp \
	$(PROJECT_DIR)/controllers/sensors/redundant_sensor.cpp \
	$(PROJECT_DIR)/controllers/sensors/redundant_ford_tps.cpp \
	$(PROJECT_DIR)/controllers/sensors/frequency_sensor.cpp \
	$(PROJECT_DIR)/controllers/sensors/hella_oil_level.cpp \
	$(PROJECT_DIR)/controllers/sensors/impl/software_knock.cpp \
	$(PROJECT_DIR)/controllers/sensors/converters/linear_func.cpp \
	$(PROJECT_DIR)/controllers/sensors/converters/resistance_func.cpp \
	$(PROJECT_DIR)/controllers/sensors/converters/thermistor_func.cpp \
	$(PROJECT_DIR)/controllers/sensors/converters/identity_func.cpp \
	$(PROJECT_DIR)/controllers/sensors/vr_pwm.cpp


CONTROLLERS_SENSORS_INC=\
    $(CONTROLLERS_DIR)/sensors \
    $(CONTROLLERS_DIR)/sensors/core \
    $(CONTROLLERS_DIR)/sensors/impl \
	$(CONTROLLERS_DIR)/sensors/converters
