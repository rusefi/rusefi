#include "adc_subscription.h"

#include "adc_inputs.h"
#include "engine.h"
#include "perf_trace.h"
#include "biquad.h"

#if EFI_UNIT_TEST

void AdcSubscription::SubscribeSensor(FunctionalSensor &sensor,
									  adc_channel_e channel,
									  float lowpassCutoff,
									  float voltsPerAdcVolt /*= 0.0f*/)
{
}

#else

struct AdcSubscriptionEntry {
	FunctionalSensor *Sensor;
	float VoltsPerAdcVolt;
	adc_channel_e Channel;
	Biquad Filter;
	bool HasUpdated = false;
};

static size_t s_nextEntry = 0;
static AdcSubscriptionEntry s_entries[16];

void AdcSubscription::SubscribeSensor(FunctionalSensor &sensor,
									  adc_channel_e channel,
									  float lowpassCutoff,
									  float voltsPerAdcVolt /*= 0.0f*/) {
	// Don't subscribe null channels
	if (!isAdcChannelValid(channel)) {
		return;
	}

	const char* name = sensor.getSensorName();
	if (/*type-limited (int)setting < 0 || */(int)channel >= HW_MAX_ADC_INDEX) {
		firmwareError(CUSTOM_INVALID_ADC, "Invalid ADC setting %s", name);
		return;
	}

	// Ensure that enough entries are available
	if (s_nextEntry >= efi::size(s_entries)) {
		firmwareError(CUSTOM_INVALID_ADC, "too many ADC subscriptions");
		return;
	}

#if EFI_PROD_CODE
	// Enable the input pin
	efiSetPadMode(name, getAdcChannelBrainPin(name, channel), PAL_MODE_INPUT_ANALOG);
#endif /* EFI_PROD_CODE */

	// if 0, default to the board's divider coefficient
	if (voltsPerAdcVolt == 0) {
		voltsPerAdcVolt = engineConfiguration->analogInputDividerCoefficient;
	}

	// Populate the entry
	auto &entry = s_entries[s_nextEntry];
	entry.Sensor = &sensor;
	entry.VoltsPerAdcVolt = voltsPerAdcVolt;
	entry.Channel = channel;
	entry.Filter.configureLowpass(SLOW_ADC_RATE, lowpassCutoff);

	s_nextEntry++;
}

void AdcSubscription::UpdateSubscribers(efitick_t nowNt) {
	ScopePerf perf(PE::AdcSubscriptionUpdateSubscribers);

	for (size_t i = 0; i < s_nextEntry; i++) {
		auto &entry = s_entries[i];

		float mcuVolts = getVoltage("sensor", entry.Channel);
		float sensorVolts = mcuVolts * entry.VoltsPerAdcVolt;

		// On the very first update, preload the filter as if we've been
		// seeing this value for a long time.  This prevents a slow ramp-up
		// towards the correct value just after startup
		if (!entry.HasUpdated) {
			entry.Filter.cookSteadyState(sensorVolts);
			entry.HasUpdated = true;
		}

		float filtered = entry.Filter.filter(sensorVolts);

		entry.Sensor->postRawValue(filtered, nowNt);
	}
}

#endif // !EFI_UNIT_TEST
