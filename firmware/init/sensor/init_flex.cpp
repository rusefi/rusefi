#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "flex_sensor.h"

static FrequencySensor flexSensor(
	SensorType::FuelEthanolPercent, 
	MS2NT(500),
	CONFIG(flexSensorPin));
static FlexConverter converter;

// https://rusefi.com/forum/viewtopic.php?p=37452&sid=829804c90d5b2e1fecd1b900cf1b1811#p37452

void initFlexSensor(DECLARE_CONFIG_PARAMETER_SIGNATURE) {

	// Nothing to do if no sensor configured
	if (!isBrainPinValid(flexSensor.getPin())) {
		return;
	}

	flexSensor.setFunction(converter);
	flexSensor.init("flex");
	flexSensor.Register();
}

void deInitFlexSensor() {
	flexSensor.deInit();
}
