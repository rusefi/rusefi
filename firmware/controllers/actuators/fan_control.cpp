#include "fan_control.h"

#include "bench_test.h"
#include "efi_gpio.h"
#include "sensor.h"

static void fanControl(OutputPin& pin, int8_t fanOnTemp, int8_t fanOffTemp) {
	auto [cltValid, clt] = Sensor::get(SensorType::Clt);

	if (!cltValid) {
		// If CLT is broken, turn the fan on
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

EXTERN_ENGINE;

void updateFans(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
		return; // let's not mess with bench testing
	}
#endif

	fanControl(enginePins.fanRelay, CONFIG(fanOnTemperature), CONFIG(fanOffTemperature));
	//fanControl(enginePins.fanRelay2, CONFIG(fan2OnTemperature), CONFIG(fan2OffTemperature));
}
