#pragma once

#include "rusefi_types.h"
#include "rusefi/expected.h"

struct AdcSample {
	int raw;
	float voltage;
};

struct AdcChip {
	// Return a cached sample. This is called from the sensor loop: do not do bus I/O here.
	virtual expected<AdcSample> readAdc(size_t channel) = 0;
};

// Registration happens during board hardware init, before sensors subscribe.
bool adcchipCanRegister(adc_channel_e base, size_t count);
bool adcchipRegister(adc_channel_e base, AdcChip& chip, brain_pin_e pinBase, size_t count);
expected<AdcSample> adcOffchipRead(adc_channel_e channel);
brain_pin_e adcOffchipGetPin(adc_channel_e channel);

#if EFI_UNIT_TEST
void resetAdcChipsForUnitTest();
#endif
