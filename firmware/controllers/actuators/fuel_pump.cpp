
#include "pch.h"

#include "fuel_pump.h"

bool FuelPumpController::getPumpState() {
	auto uptime = getTimeNowSeconds();

	isPrime = uptime >= 0 && uptime < CONFIG(startUpFuelPumpDuration);
	engineTurnedRecently = engine->triggerCentral.getTimeSinceTriggerEvent(getTimeNowNt()) < 1;

	isPumpOn = isPrime || engineTurnedRecently;

	return isPumpOn;
}

void FuelPumpController::onSlowCallback() {
	bool state = getPumpState();

	enginePins.fuelPumpRelay.setValue(state);

#if EFI_TUNER_STUDIO
	tsOutputChannels.isFuelPumpOn = state;
#endif
}
