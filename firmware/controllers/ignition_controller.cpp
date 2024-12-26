#include "pch.h"
#include "flash_main.h"

void sleepEnter() {
	writeToFlashNow();
}

bool isUsbVoltage() {
	return (Sensor::getOrZero(SensorType::BatteryVoltage) > 4 && Sensor::getOrZero(SensorType::BatteryVoltage) < 6);
}

bool isIgnVoltage() {
#if defined(IGN_KEY_DIVIDER)
  return Sensor::getOrZero(SensorType::IgnKeyVoltage) > 6;
#endif
  return Sensor::getOrZero(SensorType::BatteryVoltage) > 6;
}

IgnitionController::IgnitionController() {
	m_timeSinceIgnVoltage.reset();
}

void IgnitionController::onSlowCallback() {
	// default to 0 if failed sensor to prevent accidental ign-on if battery
	// input misconfigured (or the ADC hasn't started yet)
	auto hasIgnVoltage = isIgnVoltage();

	if (hasIgnVoltage == m_lastState) {
		if(hasIgnVoltage) {
			m_timeSinceIgnVoltage.reset();
		} else {
			if(secondsSinceIgnVoltage() >= float(engineConfiguration->standbyTimeout) && float(engineConfiguration->standbyTimeout) > 180 && !isUsbVoltage()){
				m_timeSinceIgnVoltage.reset();
				m_pendingSleep = true;
				sleepEnter();
			}
		}
		return;
	} else {
		m_lastState = hasIgnVoltage;
		engine->engineModules.apply_all([&](auto& m) { m.onIgnitionStateChanged(hasIgnVoltage); });
		if(hasIgnVoltage) {
			m_timeSinceIgnVoltage.reset();
		}
		return;
	}

}
