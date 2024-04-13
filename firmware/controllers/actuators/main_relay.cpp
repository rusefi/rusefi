#include "pch.h"

#include "main_relay.h"
#include "ignition_controller.h"

void MainRelayController::onSlowCallback() {
#if EFI_MAIN_RELAY_CONTROL
#if defined(IGN_KEY_DIVIDER)
    if (isAdcChannelValid(engineConfiguration->ignKeyAdcChannel)) {
      hasIgnitionVoltage = isIgnVoltage();
    } else
#endif // IGN_KEY_DIVIDER
    if (engineConfiguration->ignitionKeyDigitalPin != Gpio::Unassigned) {
        // separate digital input pin just for main relay logic since it's preferred to read voltage from main relay
        // key-on is usually a bit smaller voltage than main relay but sometimes even 1v off!
        hasIgnitionVoltage = efiReadPin(engineConfiguration->ignitionKeyDigitalPin);
    } else {
	    hasIgnitionVoltage = Sensor::getOrZero(SensorType::BatteryVoltage) > 5;
	}

	if (hasIgnitionVoltage) {
		m_lastIgnitionTime.reset();
	}

	// Query whether any engine modules want to keep the lights on
	delayedShutoffRequested = engine->engineModules.aggregate([](auto& m, bool prev) { return m.needsDelayedShutoff() | prev; }, false);

	// TODO: delayed shutoff timeout?

	mainRelayState = hasIgnitionVoltage | delayedShutoffRequested;
#else // not EFI_MAIN_RELAY_CONTROL
	mainRelayState = true;
#endif

	enginePins.mainRelay.setValue("mr", mainRelayState);
}

bool MainRelayController::needsDelayedShutoff() {
	// Prevent main relay from turning off if we had ignition voltage in the past 1 second
	// This avoids accidentally killing the car during a transient, for example
	// right when the starter is engaged.
	return !m_lastIgnitionTime.hasElapsedSec(1);
}
