#pragma once

struct ProtectedGpioConfig {
	// Output config
	brain_pin_e Pin;

	// Sense config
	adc_channel_e SenseChannel;
	float AmpsPerVolt;

	// Instantaneous trip current
	float MaximumAllowedCurrent;
};

int protectedGpio_add(brain_pin_e base, const ProtectedGpioConfig* const configs);
void protectedGpio_check(efitick_t nowNt);
