#include "pch.h"

#include "adc_subscription.h"
#include "fuel_level_sensor.h"

static FuelLevelSensor fuelSensor(SensorType::FuelLevel, /* timeout = */ MS2NT(500));

void initFuelLevel() {
	adc_channel_e channel = engineConfiguration->fuelLevelSensor;

	if (!isAdcChannelValid(channel)) {
		return;
	}

	// Filtering with such a small bandwidth helps prevent noisy data from fuel tank slosh
	AdcSubscription::SubscribeSensor(fuelSensor, channel, /*lowpassCutoff =*/ 0.05f);
	fuelSensor.Register();
}
