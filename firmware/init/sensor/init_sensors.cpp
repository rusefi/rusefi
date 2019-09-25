/**
 * @file init_sensorss.cpp
 */

#include "cli_registry.h"
#include "init.h"
#include "sensor.h"

void initTempSensors();
void initOilPressure();

void initSensorCli();

void initSensors() {
	// temperature sensors
	initTempSensors();

	// aux sensors
	initOilPressure();

	// Init CLI functionality for sensors (mocking)
	initSensorCli();
}

void initSensorCli() {
	addConsoleActionIF("set_sensor_mock", Sensor::setMockValue);
	addConsoleAction("reset_sensor_mocks", Sensor::resetAllMocks);
}
