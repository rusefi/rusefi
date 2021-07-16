#include "fan_control.h"
#include "engine.h"

#include "bench_test.h"
#include "efi_gpio.h"
#include "sensor.h"

static void fanControl(bool acActive, OutputPin& pin, int8_t fanOnTemp, int8_t fanOffTemp, bool enableWithAc, bool disableWhenStopped DECLARE_ENGINE_PARAMETER_SUFFIX) {
	auto [cltValid, clt] = Sensor::get(SensorType::Clt);

	bool isCranking = ENGINE(rpmCalculator).isCranking();
	bool isRunning = ENGINE(rpmCalculator).isRunning();

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

void updateFans(bool acActive DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
		return; // let's not mess with bench testing
	}
#endif

	fanControl(acActive, enginePins.fanRelay, CONFIG(fanOnTemperature), CONFIG(fanOffTemperature), CONFIG(enableFan1WithAc), CONFIG(disableFan1WhenStopped) PASS_ENGINE_PARAMETER_SUFFIX);
	fanControl(acActive, enginePins.fanRelay2, CONFIG(fan2OnTemperature), CONFIG(fan2OffTemperature), CONFIG(enableFan2WithAc), CONFIG(disableFan2WhenStopped) PASS_ENGINE_PARAMETER_SUFFIX);
}
