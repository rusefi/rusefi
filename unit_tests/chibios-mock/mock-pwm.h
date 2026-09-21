#pragma once

#include <cstdint>
#include "hardware_pwm.h"

// Host-side register model for stm32_pwm.cpp, used by the normal unit-test
// binary. PWM period is shared; channel compare values are independent.
using pwmcnt_t = uint32_t;
constexpr int PWM_OUTPUT_ACTIVE_HIGH = 1;

struct PWMConfig {
	uint32_t frequency;
	pwmcnt_t period;
	void (*callback)();
	struct {
		int mode;
		void (*callback)();
	} channels[4];
	uint32_t cr2, bdtr, dier;
};

struct PWMDriver {
	pwmcnt_t period = 0;
	pwmcnt_t compare[4] = {};
};

inline void pwmStart(PWMDriver* driver, const PWMConfig* pwmConfig) {
	driver->period = pwmConfig->period;
	for (auto& compare : driver->compare) {
		compare = 0;
	}
}

inline void pwm_lld_enable_channel(PWMDriver* driver, uint8_t channel, pwmcnt_t width) {
	driver->compare[channel] = width;
}

inline void pwmChangePeriodI(PWMDriver* driver, pwmcnt_t period) {
	// ChibiOS pwmChangePeriodI updates the shared period, while the STM32
	// TIMv1 pwm_lld_change_period writes ARR only, leaving CCRx untouched.
	driver->period = period;
}

hardware_pwm* initStm32PwmForUnitTest(PWMDriver& driver, uint8_t channel, float frequency, float duty);
void resetStm32PwmForUnitTest();
