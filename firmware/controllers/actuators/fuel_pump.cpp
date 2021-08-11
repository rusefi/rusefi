#include "pch.h"

#include "fuel_pump.h"

auto timeSinceBoot(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_MAIN_RELAY_CONTROL
	// in main relay control mode, we return the number of seconds since the ignition is turned on
	// (or negative if the ignition key is switched off)
	return engine->getTimeIgnitionSeconds();
#else
	return getTimeNowSeconds();
#endif /* EFI_MAIN_RELAY_CONTROL */
}

static bool getFuelPumpState(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	auto uptime = timeSinceBoot(PASS_ENGINE_PARAMETER_SIGNATURE);

	bool isPrime = uptime >= 0 && uptime < CONFIG(startUpFuelPumpDuration);
	bool engineTurnedRecently = engine->triggerCentral.getTimeSinceTriggerEvent(getTimeNowNt()) < 1;

	return isPrime || engineTurnedRecently;
}

void updateFuelPump(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	bool state = getFuelPumpState(PASS_ENGINE_PARAMETER_SIGNATURE);

	enginePins.fuelPumpRelay.setValue(state);

#if EFI_TUNER_STUDIO
	tsOutputChannels.isFuelPumpOn = state;
#endif // EFI_TUNER_STUDIO
}
