#include "pch.h"
#include "flash_main.h"

void sleepEnter() {
	writeToFlashNow();
}

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

	if (hasIgnVoltage == m_lastState) {
		if(hasIgnVoltage) {
			m_timeSinceIgnVoltage.reset();
		}
		return;
	} else {
		m_lastState = hasIgnVoltage;
		engine->engineModules.apply_all([&](auto& m) { m.onIgnitionStateChanged(hasIgnVoltage); });
		if(!hasIgnVoltage) {
			if(secondsSinceIgnVoltage() < float(engineConfiguration->standbyTimeout)){
				return;
			} else {
				m_timeSinceIgnVoltage.reset();
				m_pendingSleep = true;
				sleepEnter();
				return;
			}
		}
	}

}
