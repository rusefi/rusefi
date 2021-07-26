#include "pch.h"

#include "init.h"
#include "flex_sensor.h"

static FlexFuelSensor flexSensor;

// https://rusefi.com/forum/viewtopic.php?p=37452&sid=829804c90d5b2e1fecd1b900cf1b1811#p37452

void initFlexSensor(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	auto pin = CONFIG(flexSensorPin);

	// Nothing to do if no sensor configured
	if (!isBrainPinValid(pin)) {
		return;
	}

	flexSensor.init(pin);
	flexSensor.Register();
}
