/**
 * @file init_sensorss.cpp
 */

#include "cli_registry.h"
#include "init.h"
#include "sensor.h"

static void initSensorCli();

// Sensor init/config
void initTps();
void initOilPressure();

void initSensors0() {
	initTps();
	initOilPressure();

	// Init CLI functionality for sensors (mocking)
	initSensorCli();
}

// Sensor reconfiguration
void reconfigureTps();
void reconfigureOilPressure();

void reconfigureSensors() {
	reconfigureTps();
	reconfigureOilPressure();
}

// Mocking/testing helpers
static void initSensorCli() {
	addConsoleActionIF("set_sensor_mock", Sensor::setMockValue);
	addConsoleAction("reset_sensor_mocks", Sensor::resetAllMocks);
}
