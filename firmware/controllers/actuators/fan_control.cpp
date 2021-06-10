#include "fan_control.h"
#include "engine.h"

#include "bench_test.h"
#include "efi_gpio.h"
#include "sensor.h"

EXTERN_ENGINE;

static void fanControl(OutputPin& pin, int8_t fanOnTemp, int8_t fanOffTemp, bool enableWithAc DECLARE_ENGINE_PARAMETER_SUFFIX) {
	auto [cltValid, clt] = Sensor::get(SensorType::Clt);

	if (!cltValid) {
		// If CLT is broken, turn the fan on
		pin.setValue(true);
	} else if (enableWithAc && ENGINE(acSwitchState)) {
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

void updateFans(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
		return; // let's not mess with bench testing
	}
#endif

	fanControl(enginePins.fanRelay, CONFIG(fanOnTemperature), CONFIG(fanOffTemperature), CONFIG(enableFan1WithAc) PASS_ENGINE_PARAMETER_SUFFIX);
	fanControl(enginePins.fanRelay2, CONFIG(fan2OnTemperature), CONFIG(fan2OffTemperature), CONFIG(enableFan2WithAc) PASS_ENGINE_PARAMETER_SUFFIX);
}
