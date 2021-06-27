#include "ac_control.h"

#include "efi_gpio.h"
#include "sensor.h"

EXTERN_ENGINE;

static bool getAcState(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	auto rpm = Sensor::get(SensorType::Rpm).value_or(0);

	// Engine too slow, disable
	if (rpm < 500) {
		return false;
	}

	// Engine too fast, disable
	// if (rpm > CONFIG(maxAcRpm)) {
	// 	return false;
	// }

	// Engine too hot, disable
	// if (Sensor::get(SensorType::Clt).value_or(FLOAT_MAX) > CONFIG(maxAcClt)) {
	// 	return false;
	// }

	// All conditions OK, simply pass thru switch
	return engine->acSwitchState;
}

bool updateAc(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	bool isEnabled = getAcState(PASS_ENGINE_PARAMETER_SIGNATURE);

	enginePins.acRelay.setValue(isEnabled);

	return isEnabled;
}
