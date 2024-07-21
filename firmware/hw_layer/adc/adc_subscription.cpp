#include "pch.h"

#include "adc_subscription.h"

#include "biquad.h"

#if EFI_UNIT_TEST

/*static*/ void AdcSubscription::SubscribeSensor(FunctionalSensor&, adc_channel_e, float, float) {
}

/*static*/ void AdcSubscription::UnsubscribeSensor(FunctionalSensor&) {
}

/*static*/ void AdcSubscription::UnsubscribeSensor(FunctionalSensor&, adc_channel_e) {
}

#else

struct AdcSubscriptionEntry {
	FunctionalSensor *Sensor;
	float VoltsPerAdcVolt;
	Biquad Filter;
	adc_channel_e Channel;
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

	// If you passed the same sensor again, resubscribe it with the new parameters
	auto entry = findEntry(&sensor);

	if (entry) {
		// If the channel didn't change, we're already set
		if (entry->Channel == channel) {
			return;
		}

		// avoid updates to this while we're mucking with the configuration
		entry->Sensor = nullptr;
	} else {
		// If not already registered, get an empty (new) entry
		entry = findEntry();
	}

	const char* name = sensor.getSensorName();

	// Ensure that a free entry was found
	if (!entry) {
		firmwareError(ObdCode::CUSTOM_INVALID_ADC, "too many ADC subscriptions subscribing %s", name);
		return;
	}

#if EFI_PROD_CODE
	// Enable the input pin
/**
TODO: this code is similar to initIfValid, what is the plan? shall we extract helper method or else?
 */
	brain_pin_e pin = getAdcChannelBrainPin(name, channel);
	if (pin != Gpio::Invalid) {
	// todo: external muxes for internal ADC #3350
	    efiSetPadMode(name, pin, PAL_MODE_INPUT_ANALOG);
	}

	// if 0, default to the board's divider coefficient for given channel
	if (voltsPerAdcVolt == 0) {
		voltsPerAdcVolt = getAnalogInputDividerCoefficient(channel);
	}
#endif /* EFI_PROD_CODE */
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

	sensor.unregister();

	// clear the sensor first to mark this entry not in use
	entry->Sensor = nullptr;

	entry->VoltsPerAdcVolt = 0;
	entry->Channel = EFI_ADC_NONE;
}

/*static*/ void AdcSubscription::UnsubscribeSensor(FunctionalSensor& sensor, adc_channel_e channel) {
	// Find the old sensor
	auto entry = findEntry(&sensor);

	// if the channel changed, unsubscribe!
	if (entry && entry->Channel != channel) {
		AdcSubscription::UnsubscribeSensor(sensor);
	}
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

#if EFI_PROD_CODE
void AdcSubscription::PrintInfo() {
	for (size_t i = 0; i < efi::size(s_entries); i++) {
		auto& entry = s_entries[i];

		if (!entry.Sensor) {
			// Skip unconfigured entries
			continue;
		}

		const auto name = entry.Sensor->getSensorName();
		float mcuVolts = getVoltage("sensor", entry.Channel);
		float sensorVolts = mcuVolts * entry.VoltsPerAdcVolt;
		auto channel = entry.Channel;

		char pinNameBuffer[16];

		efiPrintf(
			"%s ADC%d %s adc=%.2f/input=%.2fv/divider=%.2f",
			name,
			channel,
			getPinNameByAdcChannel(name, channel, pinNameBuffer),
			mcuVolts, sensorVolts, entry.VoltsPerAdcVolt
		);
	}
}
#endif // EFI_PROD_CODE

#endif // !EFI_UNIT_TEST
