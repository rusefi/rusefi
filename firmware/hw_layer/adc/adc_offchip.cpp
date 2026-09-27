#include "pch.h"
#include "adc_offchip.h"

namespace {
struct AdcChannel {
	AdcChip* chip = nullptr;
	size_t channel = 0;
	brain_pin_e pin = Gpio::Invalid;
};

AdcChannel channels[EFI_ADC_TOTAL_CHANNELS - EFI_ADC_OFFCHIP_FIRST];

AdcChannel* findChannel(adc_channel_e channel) {
	if (channel < EFI_ADC_OFFCHIP_FIRST || channel >= EFI_ADC_TOTAL_CHANNELS) {
		return nullptr;
	}
	return &channels[channel - EFI_ADC_OFFCHIP_FIRST];
}
}

bool adcchipCanRegister(adc_channel_e base, size_t count) {
	if (!findChannel(base) || count == 0 || count > static_cast<size_t>(EFI_ADC_TOTAL_CHANNELS - base)) {
		return false;
	}
	for (size_t i = 0; i < count; i++) {
		if (channels[base - EFI_ADC_OFFCHIP_FIRST + i].chip) {
			return false;
		}
	}
	return true;
}

bool adcchipRegister(adc_channel_e base, AdcChip& chip, brain_pin_e pinBase, size_t count) {
	if (!adcchipCanRegister(base, count)) {
		return false;
	}
	for (size_t i = 0; i < count; i++) {
		channels[base - EFI_ADC_OFFCHIP_FIRST + i] = {
			&chip, i, static_cast<brain_pin_e>(static_cast<int>(pinBase) + i)
		};
	}
	return true;
}

expected<AdcSample> adcOffchipRead(adc_channel_e channel) {
	auto entry = findChannel(channel);
	if (!entry || !entry->chip) {
		return unexpected;
	}
	return entry->chip->readAdc(entry->channel);
}

brain_pin_e adcOffchipGetPin(adc_channel_e channel) {
	auto entry = findChannel(channel);
	return entry ? entry->pin : Gpio::Invalid;
}

#if EFI_UNIT_TEST
void resetAdcChipsForUnitTest() {
	for (auto& entry : channels) {
		entry = {};
	}
}
#endif
