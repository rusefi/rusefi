#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "functional_sensor.h"

// These aux sensors just read voltage - so the converter function has nothing to do
struct IdentityFunction : public SensorConverter {
	SensorResult convert(float raw) const {
		return raw;
	}
};

static IdentityFunction func;

static FunctionalSensor auxSensors[] = {
	{ SensorType::Aux1, MS2NT(50) },
	{ SensorType::Aux2, MS2NT(50) },
	{ SensorType::Aux3, MS2NT(50) },
	{ SensorType::Aux4, MS2NT(50) },
};

static_assert(efi::size(auxSensors) == FSIO_ANALOG_INPUT_COUNT);

void initAuxSensors(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	for (size_t i = 0; i < efi::size(CONFIG(fsioAdc)); i++) {
		auto channel = CONFIG(fsioAdc)[i];

		// Skip unconfigured channels
		if (!isAdcChannelValid(channel)) {
			continue;
		}

		auto& sensor = auxSensors[i];
		sensor.setFunction(func);
		sensor.Register();

		AdcSubscription::SubscribeSensor(sensor, channel, 10);
	}
}
