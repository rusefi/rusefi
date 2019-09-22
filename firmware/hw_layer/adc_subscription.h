/**
 * @file adc_subscription.h
 */

#include "global.h"
#include "converter_sensor.h"

class AdcSubscription {
public:
	static void SubscribeSensor(ConvertedSensor& sensor, adc_channel_e channel, float voltsPerAdcVolt = 0.0f);
	static void UpdateSubscribers();
};
