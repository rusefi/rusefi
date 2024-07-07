/**
 * @file adc_subscription.h
 */

#pragma once

#include "functional_sensor.h"
#include "global.h"
#include "biquad.h"

struct AdcSubscriptionEntry {
	FunctionalSensor *Sensor;
	float VoltsPerAdcVolt;
	// raw voltage before we apply biquad filter
	float sensorVolts;
	Biquad Filter;
	adc_channel_e Channel;
	bool HasUpdated = false;
};

class AdcSubscription {
public:
	static AdcSubscriptionEntry *SubscribeSensor(FunctionalSensor &sensor, adc_channel_e channel, float lowpassCutoffHZ, float voltsPerAdcVolt = 0.0f);
	static void UnsubscribeSensor(FunctionalSensor& sensor);
	static void UnsubscribeSensor(FunctionalSensor& sensor, adc_channel_e newChannel);
	static void UpdateSubscribers(efitick_t nowNt);

	static void ResetFilters();

	static void PrintInfo();
};
