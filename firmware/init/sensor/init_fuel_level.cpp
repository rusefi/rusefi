#include "pch.h"

#include "adc_subscription.h"
#include "fuel_level_sensor.h"

static FuelLevelSensor fuelSensor(SensorType::FuelLevel, /* timeout = */ MS2NT(500));

void initFuelLevel() {
	adc_channel_e channel = engineConfiguration->fuelLevelSensor;

	if (!isAdcChannelValid(channel)) {
		return;
	}

	// noisy data from fuel tank slosh is handled on an higher level
	AdcSubscription::SubscribeSensor(fuelSensor, channel, /*lowpassCutoff =*/ 100);
	fuelSensor.Register();
}
