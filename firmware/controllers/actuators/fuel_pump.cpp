
#include "pch.h"

#include "fuel_pump.h"

void FuelPumpController::onSlowCallback() {
	auto uptime = getTimeNowSeconds();

	// If the ECU just started, turn on the fuel pump to prime
	isPrime = uptime >= 0 && uptime < engineConfiguration->startUpFuelPumpDuration;

	// If there was a trigger event recently, turn on the pump, the engine is running!
	auto timeSinceTrigger = engine->triggerCentral.getTimeSinceTriggerEvent(getTimeNowNt());
	engineTurnedRecently = timeSinceTrigger < 1;

	isPumpOn = isPrime || engineTurnedRecently;

	enginePins.fuelPumpRelay.setValue(isPumpOn);

#if EFI_TUNER_STUDIO
	tsOutputChannels.isFuelPumpOn = isPumpOn;
#endif
}
