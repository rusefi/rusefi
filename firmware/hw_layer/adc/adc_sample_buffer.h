#pragma once

#include <cstddef>

// Read a logical channel from a port's slow ADC sample buffer.
template <typename Sample>
int readSlowAdcSample(const volatile Sample* samples, size_t count, int index) {
	// The global channel enum can describe inputs that this ADC port does not
	// sample. Preserve the signed invalid value used by adcGetRawVoltage().
	if (index < 0 || static_cast<size_t>(index) >= count) {
		return -1;
	}
	return samples[index];
}
