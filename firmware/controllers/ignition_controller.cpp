#include "pch.h"

bool isIgnVoltage() {
#if EFI_PROD_CODE
	// Digital pin has priority over analog inputs
	if (isBrainPinValid(engineConfiguration->ignitionKeyDigitalPin)) {
		return efiReadPin(engineConfiguration->ignitionKeyDigitalPin, engineConfiguration->ignitionKeyDigitalPinMode);
	}
#endif

	float ignVoltage = 0;
	if (Sensor::hasSensor(SensorType::IgnKeyVoltage)) {
		// If we have separate ignition sensing input
		ignVoltage = Sensor::getOrZero(SensorType::IgnKeyVoltage);
	} else {
		// No, we assume that ignition is ON while we are powered with reasonable voltage
		ignVoltage = Sensor::getOrZero(SensorType::BatteryVoltage);
	}

	return (ignVoltage > 6.0f);
}

void IgnitionController::onSlowCallback() {
	// default to 0 if failed sensor to prevent accidental ign-on if battery
	// input misconfigured (or the ADC hasn't started yet)
	auto hasIgnVoltage = isIgnVoltage();

	if (hasIgnVoltage) {
		m_timeSinceIgnVoltage.reset();
	}

	if (hasIgnVoltage == m_lastState) {
		// nothing to do, states match
		return;
	}

	// Ignore low voltage transients - we may see this at the start of cranking
	// and we don't want to
	if (!hasIgnVoltage && secondsSinceIgnVoltage() < 0.2f) {
		return;
	}

	// Store state and notify other modules of the change
	m_lastState = hasIgnVoltage;
	engine->engineModules.apply_all([&](auto& m) { m.onIgnitionStateChanged(hasIgnVoltage); });
}
