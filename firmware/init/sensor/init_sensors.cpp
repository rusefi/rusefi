#include "init.h"
#include "cli_registry.h"
#include "sensor.h"

void init_oil_pressure();

void init_sensor_cli();

void init_sensors() {
	// aux sensors
	init_oil_pressure();

	// Init CLI functionality for sensors (mocking)
	init_sensor_cli();
}

void init_sensor_cli() {
	addConsoleActionIF("set_sensor_mock", Sensor::SetMockValue);
	addConsoleAction("reset_sensor_mocks", Sensor::ResetAllMocks);
}
