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
	efiSetPadMode(msg, pin, PAL_MODE_INPUT_ANALOG);
#endif
}

void deInitIfValid(const char* msg, adc_channel_e channel) {
	if (!isAdcChannelValid(channel)) {
		return;
	}

#if EFI_PROD_CODE
	brain_pin_e pin = getAdcChannelBrainPin(msg, channel);
	efiSetPadUnused(pin);
#endif
}

static void initOldAnalogInputs() {
	initIfValid("AFR", engineConfiguration->afr.hwChannel);
	initIfValid("Baro", engineConfiguration->baroSensor.hwChannel);
	initIfValid("AUXF#1", engineConfiguration->auxFastSensor1_adcChannel);
	initIfValid("CJ125 UR", engineConfiguration->cj125ur);
	initIfValid("CJ125 UA", engineConfiguration->cj125ua);
}

static void deInitOldAnalogInputs() {
	deInitIfValid("AFR", activeConfiguration.afr.hwChannel);
	deInitIfValid("Baro", activeConfiguration.baroSensor.hwChannel);
	deInitIfValid("AUXF#1", activeConfiguration.auxFastSensor1_adcChannel);
	deInitIfValid("CJ125 UR", activeConfiguration.cj125ur);
	deInitIfValid("CJ125 UA", activeConfiguration.cj125ua);
}

void initNewSensors() {
#if EFI_CAN_SUPPORT
	initCanSensors();
#endif

	initVbatt();
	initMap();
	initTps();
	initOilPressure();
	initThermistors();
	initLambda();
	initFlexSensor();
	initBaro();
	initAuxSensors();
	initVehicleSpeedSensor();
	initTurbochargerSpeedSensor();

	#if !EFI_UNIT_TEST
		initFuelLevel();
		initMaf();
	#endif

	initOldAnalogInputs();

	// Init CLI functionality for sensors (mocking)
	initSensorCli();
}

void stopSensors() {
	deInitOldAnalogInputs();

	deinitTps();
	deinitOilPressure();
	deinitVbatt();
	deinitThermistors();
	deInitFlexSensor();
	deInitVehicleSpeedSensor();
	deinitTurbochargerSpeedSensor();
	deinitMap();
}

void reconfigureSensors() {
	initMap();
	initTps();
	initOilPressure();
	initVbatt();
	initThermistors();
	initFlexSensor();
	initVehicleSpeedSensor();
	initTurbochargerSpeedSensor();

	initOldAnalogInputs();
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
