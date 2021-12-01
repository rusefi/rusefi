#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "functional_sensor.h"
#include "identity_func.h"

// These aux sensors just read voltage - so the converter function has nothing to do

static FunctionalSensor auxSensors[] = {
	{ SensorType::Aux1, MS2NT(50) },
	{ SensorType::Aux2, MS2NT(50) },
	{ SensorType::Aux3, MS2NT(50) },
	{ SensorType::Aux4, MS2NT(50) },
	{ SensorType::Aux5, MS2NT(50) },
	{ SensorType::Aux6, MS2NT(50) },
	{ SensorType::Aux7, MS2NT(50) },
	{ SensorType::Aux8, MS2NT(50) },
};

static_assert(efi::size(auxSensors) == AUX_ANALOG_INPUT_COUNT);

void initAuxSensors() {
	for (size_t i = 0; i < efi::size(engineConfiguration->auxAnalogInputs); i++) {
		auto channel = engineConfiguration->auxAnalogInputs[i];

		// Skip unconfigured channels
		if (!isAdcChannelValid(channel)) {
			continue;
		}

		auto& sensor = auxSensors[i];
		sensor.setFunction(identityFunction);
		sensor.Register();

		AdcSubscription::SubscribeSensor(sensor, channel, 10);
	}
}
