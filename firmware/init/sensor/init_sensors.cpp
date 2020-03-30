/**
 * @file init_sensorss.cpp
 */

#include "cli_registry.h"
#include "init.h"
#include "sensor.h"

static void initSensorCli(Logging* logger);

// Sensor init/config
void initTps();
void initOilPressure();

void initNewSensors(Logging* logger) {
	initTps();
	initOilPressure();

	// Init CLI functionality for sensors (mocking)
	initSensorCli(logger);
}

// Sensor reconfiguration
void reconfigureTps();
void reconfigureOilPressure();

void reconfigureSensors() {
	reconfigureTps();
	reconfigureOilPressure();
}

static Logging* s_logger;

// Mocking/testing helpers
static void initSensorCli(Logging* logger) {
	s_logger = logger;

	addConsoleActionIF("set_sensor_mock", Sensor::setMockValue);
	addConsoleAction("reset_sensor_mocks", Sensor::resetAllMocks);
	addConsoleAction("show_sensors", []() { Sensor::showAllSensorInfo(s_logger); });
	addConsoleActionI("show_sensor", 
		[](int idx) {
			if (auto s = Sensor::getSensorOfType(static_cast<SensorType>(idx))) {
				s->showAllSensorInfo(s_logger);
			}
		});
}
