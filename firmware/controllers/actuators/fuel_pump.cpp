
#include "pch.h"

#include "fuel_pump.h"

void FuelPumpController::onSlowCallback() {
	auto timeSinceIgn = m_ignOnTimer.getElapsedSeconds();

	// If the ignition just turned on, turn on the fuel pump to prime
	isPrime = timeSinceIgn >= 0 && timeSinceIgn < engineConfiguration->startUpFuelPumpDuration;

	// If there was a trigger event recently, turn on the pump, the engine is running!
	auto timeSinceTrigger = engine->triggerCentral.getTimeSinceTriggerEvent(getTimeNowNt());
	engineTurnedRecently = timeSinceTrigger < 1;

	isPumpOn = isPrime || engineTurnedRecently;

	enginePins.fuelPumpRelay.setValue(isPumpOn);

#if EFI_TUNER_STUDIO
	engine->outputChannels.isFuelPumpOn = isPumpOn;
#endif
}

void FuelPumpController::onIgnitionStateChanged(bool ignitionOnParam) {
	// live data parser convention is asking for a field
	ignitionOn = ignitionOnParam;
	if (ignitionOn) {
		m_ignOnTimer.reset();
	}
}
