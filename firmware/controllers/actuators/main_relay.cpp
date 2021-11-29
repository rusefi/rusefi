#include "pch.h"

#include "main_relay.h"

void MainRelayController::onSlowCallback() {
	isBenchTest = engine->isInMainRelayBench();

#if EFI_MAIN_RELAY_CONTROL
	mainRelayState = isBenchTest | hasIgnitionVoltage;
#else // not EFI_MAIN_RELAY_CONTROL
	mainRelayState = !isBenchTest;
#endif

	enginePins.mainRelay.setValue(mainRelayState);
}

void MainRelayController::onIgnitionStateChanged(bool ignitionOn) {
	hasIgnitionVoltage = ignitionOn;
}
