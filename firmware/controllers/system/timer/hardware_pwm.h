/**
 * @file hardware_pwm.h
 *
 * Interface of a hardware (timer-driven) PWM channel. Implemented per port (stm32_pwm.cpp) and by the
 * gpio-chip layer (external_hardware_pwm in drivers/gpio/core.cpp). Lives in its own header so that
 * unit tests can provide fakes.
 */

#pragma once

#include "rusefi_enums.h"

struct hardware_pwm {
	static hardware_pwm* tryInitPin(const char* msg, brain_pin_e pin, float frequencyHz, float duty);

	virtual void setDuty(float duty) = 0;

	/**
	 * Reprogram the channel's frequency while it keeps running. Timer frequency is a per-timer
	 * property, so every channel sharing the timer changes with it.
	 * @return false if this device cannot do that frequency; the previous frequency stays in effect.
	 */
	virtual bool setFrequency(float /*frequencyHz*/) { return false; }
};
