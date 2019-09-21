#include "cli_registry.h"
#include "init.h"
#include "sensor.h"

void initOilPressure();

void initSensorCli();

void initSensors() {
	// aux sensors
	initOilPressure();

	// Init CLI functionality for sensors (mocking)
	initSensorCli();
}

void initSensorCli() {
	addConsoleActionIF("set_sensor_mock", Sensor::setMockValue);
	addConsoleAction("reset_sensor_mocks", Sensor::resetAllMocks);
}
