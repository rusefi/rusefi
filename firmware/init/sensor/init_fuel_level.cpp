#include "pch.h"

#include "functional_sensor.h"
#include "adc_subscription.h"
#include "fuel_level_func.h"

static FunctionalSensor fuelSensor(SensorType::FuelLevel, /* timeout = */ MS2NT(500));

static FuelLevelFunc fuelCurve;

void initFuelLevel() {
	adc_channel_e channel = engineConfiguration->fuelLevelSensor;

	if (!isAdcChannelValid(channel)) {
		return;
	}

	fuelSensor.setFunction(fuelCurve);

	// Filtering with such a small bandwidth helps prevent noisy data from fuel tank slosh
	AdcSubscription::SubscribeSensor(fuelSensor, channel, /*lowpassCutoff =*/ 0.05f);
	fuelSensor.Register();
}
