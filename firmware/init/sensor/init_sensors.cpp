/**
 * @file init_sensorss.cpp
 */

#include "cli_registry.h"
#include "init.h"
#include "sensor.h"

void initTps();
void initOilPressure();

void initSensorCli();

void initSensors() {
	// TPS
	initTps();

	// aux sensors
	initOilPressure();

	// Init CLI functionality for sensors (mocking)
	initSensorCli();
}

void initSensorCli() {
	addConsoleActionIF("set_sensor_mock", Sensor::setMockValue);
	addConsoleAction("reset_sensor_mocks", Sensor::resetAllMocks);
}
