/**
 * @file adc_subscription.h
 */

#pragma once

#include "functional_sensor.h"
#include "global.h"

class AdcSubscription {
public:
	static void SubscribeSensor(FunctionalSensor &sensor, adc_channel_e channel, float voltsPerAdcVolt = 0.0f);
	static void UpdateSubscribers();
};
