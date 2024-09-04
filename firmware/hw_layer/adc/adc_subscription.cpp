#include "pch.h"

#include "adc_subscription.h"

#if EFI_UNIT_TEST

/*static*/ AdcSubscriptionEntry * AdcSubscription::SubscribeSensor(FunctionalSensor&, adc_channel_e, float, float) {
  return nullptr;
}

/*static*/ void AdcSubscription::UnsubscribeSensor(FunctionalSensor&) {
}

/*static*/ void AdcSubscription::UnsubscribeSensor(FunctionalSensor&, adc_channel_e) {
}

#else

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

/*static*/ AdcSubscriptionEntry* AdcSubscription::SubscribeSensor(FunctionalSensor &sensor,
									  adc_channel_e channel,
									  float lowpassCutoff,
									  float voltsPerAdcVolt /*= 0.0f*/) {
	// Don't subscribe null channels
	if (!isAdcChannelValid(channel)) {
		return nullptr;
	}

	// If you passed the same sensor again, resubscribe it with the new parameters
	AdcSubscriptionEntry* entry = findEntry(&sensor);

	if (entry) {
		// If the channel didn't change, we're already set
		if (entry->Channel == channel) {
			return entry;
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
		return nullptr;
	}

#if EFI_PROD_CODE && HAL_USE_ADC
	// Enable the input pin
/**
TODO: this code is similar to initIfValid, what is the plan? shall we extract helper method or else?
 */
	brain_pin_e pin = getAdcChannelBrainPin(name, channel);
	if (pin != Gpio::Invalid) {
		// todo: external muxes for internal ADC #3350
		/* reuqest pin only for parent */
		if (!adcIsMuxedInput(channel)) {
			efiSetPadMode(name, pin, PAL_MODE_INPUT_ANALOG);
		} else {
			efiSetPadModeWithoutOwnershipAcquisition(name, pin, PAL_MODE_INPUT_ANALOG);
		}
	}

	// if 0, default to the board's divider coefficient for given channel
	if (voltsPerAdcVolt == 0) {
		voltsPerAdcVolt = getAnalogInputDividerCoefficient(channel);
	}
#endif /* EFI_PROD_CODE && HAL_USE_ADC */
	// Populate the entry
	entry->VoltsPerAdcVolt = voltsPerAdcVolt;
	entry->Channel = channel;
	entry->Filter.configureLowpass(SLOW_ADC_RATE, lowpassCutoff);
	entry->HasUpdated = false;

	// Set the sensor last - it's the field we use to determine whether this entry is in use
	entry->Sensor = &sensor;
	return entry;
}

/*static*/ void AdcSubscription::UnsubscribeSensor(FunctionalSensor& sensor) {
	auto entry = findEntry(&sensor);

	if (!entry) {
		// This sensor wasn't configured, skip it
		return;
	}

#if EFI_PROD_CODE && HAL_USE_ADC
	// Release the pin
	efiSetPadUnused(getAdcChannelBrainPin("adc unsubscribe", entry->Channel));
#endif // EFI_PROD_CODE && HAL_USE_ADC

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

void AdcSubscription::ResetFilters() {
	for (size_t i = 0; i < efi::size(s_entries); i++) {
		auto &entry = s_entries[i];
		entry.HasUpdated = false;
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
		entry.sensorVolts = mcuVolts * entry.VoltsPerAdcVolt;

		// On the very first update, preload the filter as if we've been
		// seeing this value for a long time.  This prevents a slow ramp-up
		// towards the correct value just after startup
		if (!entry.HasUpdated) {
			entry.Filter.cookSteadyState(entry.sensorVolts);
			entry.HasUpdated = true;
		}

		float filtered = entry.Filter.filter(entry.sensorVolts);

		entry.Sensor->postRawValue(filtered, nowNt);
	}
}

#if EFI_PROD_CODE && HAL_USE_ADC
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
			"%s ADC%d m=%d %s adc=%.2f/input=%.2fv/divider=%.2f",
			name,
			channel,
			(int)getAdcMode(channel),
			getPinNameByAdcChannel(name, channel, pinNameBuffer, sizeof(pinNameBuffer)),
			mcuVolts, sensorVolts, entry.VoltsPerAdcVolt
		);
	}
}
#endif // EFI_PROD_CODE && HAL_USE_ADC

#endif // !EFI_UNIT_TEST
