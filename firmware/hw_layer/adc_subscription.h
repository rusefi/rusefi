/**
 * @file adc_subscription.h
 */

#pragma once

#include "global.h"
#include "functional_sensor.h"

class AdcSubscription {
public:
	static void SubscribeSensor(FunctionalSensorBase& sensor, adc_channel_e channel, float voltsPerAdcVolt = 0.0f);
	static void UpdateSubscribers();
};
