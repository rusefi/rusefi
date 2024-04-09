#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "functional_sensor.h"
#include "identity_func.h"

// These aux sensors just read voltage - so the converter function has nothing to do

static FunctionalSensor auxSensors[] = {
	{ SensorType::AuxAnalog1, MS2NT(50) },
	{ SensorType::AuxAnalog2, MS2NT(50) },
	{ SensorType::AuxAnalog3, MS2NT(50) },
	{ SensorType::AuxAnalog4, MS2NT(50) },
	{ SensorType::AuxAnalog5, MS2NT(50) },
	{ SensorType::AuxAnalog6, MS2NT(50) },
	{ SensorType::AuxAnalog7, MS2NT(50) },
	{ SensorType::AuxAnalog8, MS2NT(50) },
};

StoredValueSensor luaGauges[] = {
		{ SensorType::LuaGauge1, MS2NT(5000) },
		{ SensorType::LuaGauge2, MS2NT(5000) },
};


static_assert(efi::size(auxSensors) == LUA_ANALOG_INPUT_COUNT);
static_assert(efi::size(luaGauges) == LUA_GAUGE_COUNT);

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

	for (size_t i = 0; i < efi::size(luaGauges); i++) {
		auto& sensor = luaGauges[i];
		sensor.Register();
	}
}

void deinitAuxSensors() {
	for (size_t i = 0; i < efi::size(engineConfiguration->auxAnalogInputs); i++) {
		AdcSubscription::UnsubscribeSensor(auxSensors[i]);
		auxSensors[i].unregister();
	}
}
