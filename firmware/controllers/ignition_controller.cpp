#include "pch.h"

void IgnitionController::onSlowCallback() {
	auto hasIgnVoltage = Sensor::get(SensorType::BatteryVoltage).value_or(12) > 5;

	if (hasIgnVoltage == m_lastState) {
		// nothing to do, states match
		return;
	}

	// We just changed state, suppress another change
	if (!m_timeout.hasElapsedSec(2)) {
		return;
	}

	// Store state and notify other modules of the change
	m_lastState = hasIgnVoltage;
	engine->engineModules.apply_all([&](auto& m) { m.onIgnitionStateChanged(hasIgnVoltage); });
}
