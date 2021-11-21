#include "pch.h"

#include "fan_control.h"

#include "bench_test.h"

static void fanControl(bool acActive, OutputPin& pin, int8_t fanOnTemp, int8_t fanOffTemp, bool enableWithAc, bool disableWhenStopped) {
	auto [cltValid, clt] = Sensor::get(SensorType::Clt);

	bool isCranking = engine->rpmCalculator.isCranking();
	bool isRunning = engine->rpmCalculator.isRunning();

	if (isCranking) {
		// Inhibit while cranking
		pin.setValue(false);
	} else if (disableWhenStopped && !isRunning) {
		// Inhibit while not running (if so configured)
		pin.setValue(false);
	} else if (!cltValid) {
		// If CLT is broken, turn the fan on
		pin.setValue(true);
	} else if (enableWithAc && acActive) {
		pin.setValue(true);
	} else if (clt > fanOnTemp) {
		// If hot, turn the fan on
		pin.setValue(true);
	} else if (clt < fanOffTemp) {
		// If cold, turn the fan off
		pin.setValue(false);
	} else {
		// no condition met, maintain previous state
	}
}

void updateFans(bool acActive) {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
		return; // let's not mess with bench testing
	}
#endif

	fanControl(acActive, enginePins.fanRelay, engineConfiguration->fanOnTemperature, engineConfiguration->fanOffTemperature, engineConfiguration->enableFan1WithAc, engineConfiguration->disableFan1WhenStopped);
	fanControl(acActive, enginePins.fanRelay2, engineConfiguration->fan2OnTemperature, engineConfiguration->fan2OffTemperature, engineConfiguration->enableFan2WithAc, engineConfiguration->disableFan2WhenStopped);
}
