#include "pch.h"

#include "main_relay.h"

void MainRelayController::onSlowCallback() {
	isBenchTest = engine->isInMainRelayBench();

#if EFI_MAIN_RELAY_CONTROL
	hasIgnitionVoltage = Sensor::getOrZero(SensorType::BatteryVoltage) > 5;

	if (hasIgnitionVoltage) {
		m_lastIgnitionTime.reset();
	}

	// Query whether any engine modules want to keep the lights on
	delayedShutoffRequested = engine->engineModules.aggregate([](auto& m, bool prev) { return m.needsDelayedShutoff() | prev; }, false);

	// TODO: delayed shutoff timeout?

	mainRelayState = isBenchTest | hasIgnitionVoltage | delayedShutoffRequested;
#else // not EFI_MAIN_RELAY_CONTROL
	mainRelayState = !isBenchTest;
#endif

	enginePins.mainRelay.setValue(mainRelayState);
}

bool MainRelayController::needsDelayedShutoff() {
	// Prevent main relay from turning off if we had igniton voltage in the past 1 second
	// This avoids accidentally killing the car during a transient, for example
	// right when the starter is engaged.
	return !m_lastIgnitionTime.hasElapsedSec(1);
}
