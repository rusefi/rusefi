#include "pch.h"
#include "flash_main.h"

bool isIgnVoltage() {
#if defined(IGN_KEY_DIVIDER)
  return Sensor::getOrZero(SensorType::IgnKeyVoltage) > 5;
#endif
  return Sensor::getOrZero(SensorType::BatteryVoltage) > 5;
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
	} else if (!hasIgnVoltage && secondsSinceIgnVoltage() >= 0.2f) {
		pendingSleep = 1;
	}

	// Store state and notify other modules of the change
	m_lastState = hasIgnVoltage;
	engine->engineModules.apply_all([&](auto& m) { m.onIgnitionStateChanged(hasIgnVoltage); });

	if(pendingSleep) {
		writeToFlashNow();
	}
}
