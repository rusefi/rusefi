#include "pch.h"

#include "adc_subscription.h"

#include "biquad.h"

#if EFI_UNIT_TEST

/*static*/ void AdcSubscription::SubscribeSensor(FunctionalSensor&, adc_channel_e, float, float) {
}

/*static*/ void AdcSubscription::UnsubscribeSensor(FunctionalSensor&) {
}

#else

struct AdcSubscriptionEntry {
	FunctionalSensor *Sensor;
	float VoltsPerAdcVolt;
	adc_channel_e Channel;
	Biquad Filter;
	bool HasUpdated = false;
};

static AdcSubscriptionEntry s_entries[16];

static AdcSubscriptionEntry* findEntry(FunctionalSensor* sensor) {
	for (size_t i = 0; i < efi::size(s_entries); i++) {
		if (s_entries[i].Sensor == sensor) {
			return &s_entries[i];
		}
	}

	return nullptr;
}

static AdcSubscriptionEntry* findEntry() {
	// Find an entry with no sensor set
	return findEntry(nullptr);
}

/*static*/ void AdcSubscription::SubscribeSensor(FunctionalSensor &sensor,
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

	auto entry = findEntry();

	// Ensure that a free entry was found
	if (!entry) {
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
	entry->VoltsPerAdcVolt = voltsPerAdcVolt;
	entry->Channel = channel;
	entry->Filter.configureLowpass(SLOW_ADC_RATE, lowpassCutoff);
	entry->HasUpdated = false;

	// Set the sensor last - it's the field we use to determine whether this entry is in use
	entry->Sensor = &sensor;
}

/*static*/ void AdcSubscription::UnsubscribeSensor(FunctionalSensor& sensor) {
	auto entry = findEntry(&sensor);

	if (!entry) {
		// This sensor wasn't configured, skip it
		return;
	}

#if EFI_PROD_CODE
	// Release the pin
	efiSetPadUnused(getAdcChannelBrainPin("adc unsubscribe", entry->Channel));
#endif // EFI_PROD_CODE

	// clear the sensor first to mark this entry not in use
	entry->Sensor = nullptr;

	entry->VoltsPerAdcVolt = 0;
	entry->Channel = EFI_ADC_NONE;
}

void AdcSubscription::UpdateSubscribers(efitick_t nowNt) {
	ScopePerf perf(PE::AdcSubscriptionUpdateSubscribers);

	for (size_t i = 0; i < efi::size(s_entries); i++) {
		auto &entry = s_entries[i];

		if (!entry.Sensor) {
			// Skip unconfigured entries
			continue;
		}

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
