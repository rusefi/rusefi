/**
 * @file init_sensorss.cpp
 */

#include "init.h"
#include "cli_registry.h"

static void initSensorCli();

void initNewSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_CAN_SUPPORT
	initCanSensors();
#endif

	initVbatt(PASS_CONFIG_PARAMETER_SIGNATURE);
	initMap(PASS_ENGINE_PARAMETER_SIGNATURE);
	initTps(PASS_CONFIG_PARAMETER_SIGNATURE);
	initOilPressure(PASS_CONFIG_PARAMETER_SIGNATURE);
	initThermistors(PASS_CONFIG_PARAMETER_SIGNATURE);
	initLambda(PASS_ENGINE_PARAMETER_SIGNATURE);
	initFlexSensor(PASS_CONFIG_PARAMETER_SIGNATURE);
	initBaro(PASS_CONFIG_PARAMETER_SIGNATURE);
	initAuxSensors(PASS_CONFIG_PARAMETER_SIGNATURE);

	#if !EFI_UNIT_TEST
		initFuelLevel(PASS_CONFIG_PARAMETER_SIGNATURE);
		initMaf(PASS_CONFIG_PARAMETER_SIGNATURE);
	#endif

	// Init CLI functionality for sensors (mocking)
	initSensorCli();
}

void reconfigureSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	reconfigureVbatt(PASS_CONFIG_PARAMETER_SIGNATURE);
	reconfigureTps(PASS_CONFIG_PARAMETER_SIGNATURE);
	reconfigureOilPressure(PASS_CONFIG_PARAMETER_SIGNATURE);
	reconfigureThermistors(PASS_CONFIG_PARAMETER_SIGNATURE);
}

// Mocking/testing helpers
static void initSensorCli() {
	addConsoleActionIF("set_sensor_mock", Sensor::setMockValue);
	addConsoleAction("reset_sensor_mocks", Sensor::resetAllMocks);
	addConsoleAction("show_sensors", Sensor::showAllSensorInfo);
	addConsoleActionI("show_sensor",
		[](int idx) {
			Sensor::showInfo(static_cast<SensorType>(idx));
		});
}
