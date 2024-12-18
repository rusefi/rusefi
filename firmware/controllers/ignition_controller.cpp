#include "pch.h"
#include "rusefi.h"
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

	if (hasIgnVoltage && (hasIgnVoltage == m_lastState)) {
		m_timeSinceIgnVoltage.reset();
	} else if (hasIgnVoltage && (hasIgnVoltage != m_lastState)) {
		pendingSleep = 0;
		pendingSleepInner = 0;
		restartFromSleep = 1;
		m_timeSinceNotIgnVoltage.reset();
		rebootNow();
	}

	if(hasIgnVoltage && secondsSinceNotIgnVoltage() > 0.2f && restartFromSleep) {
		restartFromSleep = 0;
	}

	if (hasIgnVoltage == m_lastState && !pendingSleepInner && !pendingSleep) {
		// nothing to do, states match
		return;
	}

	// Ignore low voltage transients - we may see this at the start of cranking
	// and we don't want to
	if (!hasIgnVoltage && secondsSinceIgnVoltage() < 0.2f) {
		return;
	} else if (!hasIgnVoltage && secondsSinceIgnVoltage() >= 0.2f) {
		m_timeSinceNotIgnVoltage.reset();
		pendingSleepInner = 1;
		restartFromSleep = 0;
	}

	// Store state and notify other modules of the change
	if(hasIgnVoltage != m_lastState) {
		m_lastState = hasIgnVoltage;
		engine->engineModules.apply_all([&](auto& m) { m.onIgnitionStateChanged(hasIgnVoltage); });
	}	

	if(pendingSleepInner && secondsSinceIgnVoltage() >= float(engineConfiguration->standbyTimeout)) {
		pendingSleep = 1;
		pendingSleepInner = 0;
		writeToFlashNow();
	}
}
