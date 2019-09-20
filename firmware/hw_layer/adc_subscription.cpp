#include "adc_subscription.h"
#include "adc_inputs.h"
#include "analog_input.h"
#include "engine.h"

#include <iterator>

EXTERN_ENGINE;

struct AdcSubscriptionEntry {
	ConvertedSensor* Sensor;
	float VoltsPerAdcVolt;
	adc_channel_e Channel;
};

static size_t s_nextEntry = 0;
static AdcSubscriptionEntry s_entries[8];

void AdcSubscription::SubscribeSensor(ConvertedSensor& sensor, adc_channel_e channel, float voltsPerAdcVolt /*= 0.0f*/) {
	// Don't subscribe null channels
	if (channel == EFI_ADC_NONE) {
		return;
	}
	
	// bounds check
	if (s_nextEntry >= std::size(s_entries)) {
		return;
	}

	// if 0, default to the board's divider coefficient
	if (voltsPerAdcVolt == 0) {
		voltsPerAdcVolt = engineConfiguration->analogInputDividerCoefficient;
	}

	// Populate the entry
	auto& entry = s_entries[s_nextEntry];
	entry.Sensor = &sensor;
	entry.VoltsPerAdcVolt = voltsPerAdcVolt;
	entry.Channel = channel;

	s_nextEntry++;
}

void AdcSubscription::UpdateSubscribers() {
	for (size_t i = 0; i < s_nextEntry; i++) {
		auto& entry = s_entries[i];

		float volts = getVoltage("sensor", entry.Channel);

		entry.Sensor->postRawValue(volts);
	}
}
