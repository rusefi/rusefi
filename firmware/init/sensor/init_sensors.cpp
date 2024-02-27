/**
 * @file init_sensorss.cpp
 */

#include "pch.h"

#include "init.h"
#include "cli_registry.h"
#include "io_pins.h"
#include "lua_hooks.h"

static void initSensorCli();

void initIfValid(const char* msg, adc_channel_e channel) {
	if (!isAdcChannelValid(channel)) {
		return;
	}

#if EFI_PROD_CODE && HAL_USE_ADC
/**
TODO: this code is similar to AdcSubscription::SubscribeSensor, what is the plan? shall we extract helper method or else?
 */

	brain_pin_e pin = getAdcChannelBrainPin(msg, channel);
	if (pin == Gpio::Invalid) {
	// todo: external muxes for internal ADC #3350
	    return;
	}
	efiSetPadMode(msg, pin, PAL_MODE_INPUT_ANALOG);
#endif // EFI_PROD_CODE && HAL_USE_ADC
}

void deInitIfValid(const char* msg, adc_channel_e channel) {
	if (!isAdcChannelValid(channel)) {
		return;
	}

#if EFI_PROD_CODE && HAL_USE_ADC
	brain_pin_e pin = getAdcChannelBrainPin(msg, channel);
	efiSetPadUnused(pin);
#endif // EFI_PROD_CODE && HAL_USE_ADC
}

static void initOldAnalogInputs() {
    if (isAdcChannelValid(engineConfiguration->afr.hwChannel) && engineConfiguration->enableAemXSeries) {
        criticalError("Please pick either analog AFR or CAN AFR input not both.");
    }
	initIfValid("AFR", engineConfiguration->afr.hwChannel);
	initIfValid("AUXF#1", engineConfiguration->auxFastSensor1_adcChannel);
}

static void deInitOldAnalogInputs() {
	deInitIfValid("AFR", activeConfiguration.afr.hwChannel);
	deInitIfValid("AUXF#1", activeConfiguration.auxFastSensor1_adcChannel);
}

static void initAuxDigital() {
#if EFI_PROD_CODE
	for (size_t i = 0;i<efi::size(engineConfiguration->luaDigitalInputPins);i++) {
		efiSetPadMode("Lua Digital", engineConfiguration->luaDigitalInputPins[i], engineConfiguration->luaDigitalInputPinModes[i]);
	}
#endif // EFI_PROD_CODE
}

void pokeAuxDigital() {
#if EFI_PROD_CODE
	for (size_t i = 0;i<efi::size(engineConfiguration->luaDigitalInputPins);i++) {
	  engine->luaDigitalInputState[i].state.update(getAuxDigital(i));
	}
#endif // EFI_PROD_CODE
}

static void deInitAuxDigital() {
	for (size_t i = 0;i<efi::size(activeConfiguration.luaDigitalInputPins);i++) {
		brain_pin_markUnused(activeConfiguration.luaDigitalInputPins[i]);
	}
}

// one-time start-up
// see also 'reconfigureSensors'
void initNewSensors() {
#if EFI_PROD_CODE && EFI_CAN_SUPPORT
	initCanSensors();
#endif

	initVbatt();
	initMap();
	initTps();
	initFluidPressure();
	initThermistors();
	initLambda();
	initFlexSensor(true);
	initBaro();
	initAuxSensors();
	initVehicleSpeedSensor();
	initTurbochargerSpeedSensor();
	initAuxSpeedSensors();
	initInputShaftSpeedSensor();

	#if !EFI_UNIT_TEST
		initFuelLevel();
		initMaf();
	#endif

	initOldAnalogInputs();
	initAuxDigital();

	// Init CLI functionality for sensors (mocking)
	initSensorCli();

#if defined(HARDWARE_CI) && !defined(HW_PROTEUS)
	chThdSleepMilliseconds(100);

	if (Sensor::getOrZero(SensorType::BatteryVoltage) < 8) {
		// Fake that we have battery voltage, some tests rely on it
		Sensor::setMockValue(SensorType::BatteryVoltage, 10);
	}
#endif

#if EFI_SIMULATOR
	// Simulator gets battery voltage so it detects ignition-on
	Sensor::setMockValue(SensorType::BatteryVoltage, 14);
#endif // EFI_SIMULATOR
}

void stopSensors() {
	deInitAuxDigital();
	deInitOldAnalogInputs();

	deinitTps();
	deinitFluidPressure();
	deinitVbatt();
	deinitThermistors();
	deInitFlexSensor();
	deInitVehicleSpeedSensor();
	deinitTurbochargerSpeedSensor();
	deinitAuxSpeedSensors();
	deinitMap();
	deinitInputShaftSpeedSensor();
}

void reconfigureSensors() {
	initMap();
	initTps();
	initFluidPressure();
	initVbatt();
	initThermistors();
	initLambda();
	initFlexSensor(false);
	initVehicleSpeedSensor();
	initTurbochargerSpeedSensor();
	initInputShaftSpeedSensor();

	initOldAnalogInputs();
}

// Mocking/testing helpers
static void initSensorCli() {
	addConsoleActionIF(CMD_SET_SENSOR_MOCK, Sensor::setMockValue);
	addConsoleAction(CMD_RESET_SENSOR_MOCKS, Sensor::resetAllMocks);
	addConsoleAction("show_sensors", Sensor::showAllSensorInfo);
	addConsoleActionI("show_sensor",
		[](int idx) {
			Sensor::showInfo(static_cast<SensorType>(idx));
		});
}
