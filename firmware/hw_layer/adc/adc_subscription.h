/**
 * @file adc_subscription.h
 */

#pragma once

#include "functional_sensor.h"
#include "global.h"

class AdcSubscription {
public:
	static void SubscribeSensor(FunctionalSensor &sensor, adc_channel_e channel, float lowpassCutoff, float voltsPerAdcVolt = 0.0f);
	static void UnsubscribeSensor(FunctionalSensor& sensor);
	static void UpdateSubscribers(efitick_t nowNt);
};
