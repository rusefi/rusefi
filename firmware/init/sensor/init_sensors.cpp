/**
 * @file init_sensorss.cpp
 */

#include "pch.h"

#include "init.h"
#include "cli_registry.h"

static void initSensorCli();

void initIfValid(const char* msg, adc_channel_e channel) {
	if (!isAdcChannelValid(channel)) {
		return;
	}

#if EFI_PROD_CODE
	brain_pin_e pin = getAdcChannelBrainPin(msg, channel);
	efiSetPadMode(msg, pin, PAL_MODE_INPUT_ANALOG PASS_ENGINE_PARAMETER_SUFFIX);
#endif
}

void deInitIfValid(const char* msg, adc_channel_e channel) {
	if (!isAdcChannelValid(channel)) {
		return;
	}

#if EFI_PROD_CODE
	brain_pin_e pin = getAdcChannelBrainPin(msg, channel);
	efiSetPadUnused(pin PASS_ENGINE_PARAMETER_SUFFIX);
#endif
}

static void initOldAnalogInputs(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	initIfValid("AFR", engineConfiguration->afr.hwChannel);
	initIfValid("MAP", engineConfiguration->map.sensor.hwChannel);
	initIfValid("Baro", engineConfiguration->baroSensor.hwChannel);
	initIfValid("AUXF#1", engineConfiguration->auxFastSensor1_adcChannel);
	initIfValid("CJ125 UR", engineConfiguration->cj125ur);
	initIfValid("CJ125 UA", engineConfiguration->cj125ua);
}

static void deInitOldAnalogInputs(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	deInitIfValid("AFR", activeConfiguration.afr.hwChannel);
	deInitIfValid("MAP", activeConfiguration.map.sensor.hwChannel);
	deInitIfValid("Baro", activeConfiguration.baroSensor.hwChannel);
	deInitIfValid("AUXF#1", activeConfiguration.auxFastSensor1_adcChannel);
	deInitIfValid("CJ125 UR", activeConfiguration.cj125ur);
	deInitIfValid("CJ125 UA", activeConfiguration.cj125ua);
}

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
	initVehicleSpeedSensor(PASS_ENGINE_PARAMETER_SIGNATURE);

	#if !EFI_UNIT_TEST
		initFuelLevel(PASS_CONFIG_PARAMETER_SIGNATURE);
		initMaf(PASS_CONFIG_PARAMETER_SIGNATURE);
	#endif

	initOldAnalogInputs(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Init CLI functionality for sensors (mocking)
	initSensorCli();
}

void reconfigureSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	deInitOldAnalogInputs(PASS_ENGINE_PARAMETER_SIGNATURE);

	deinitTps();
	deinitVbatt();
	deinitThermistors();
	deInitFlexSensor();
	reconfigureOilPressure(PASS_CONFIG_PARAMETER_SIGNATURE);

	initTps(PASS_CONFIG_PARAMETER_SIGNATURE);
	initVbatt(PASS_CONFIG_PARAMETER_SIGNATURE);
	initThermistors(PASS_CONFIG_PARAMETER_SIGNATURE);
	initFlexSensor(PASS_CONFIG_PARAMETER_SIGNATURE);

	initOldAnalogInputs(PASS_ENGINE_PARAMETER_SIGNATURE);
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
