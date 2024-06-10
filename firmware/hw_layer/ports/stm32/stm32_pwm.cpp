/**
 * @file	stm32_common_pwm.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#define _2_MHZ 2'000'000

#if HAL_USE_PWM

namespace {
struct stm32_pwm_config {
	PWMDriver* const Driver;
	const uint8_t Channel;
	const uint8_t AlternateFunc;
};

class stm32_hardware_pwm : public hardware_pwm {
public:
	bool hasInit() const {
		return m_driver != nullptr;
	}

	// 2MHz, 16-bit timer gets us a usable frequency range of 31hz to 10khz
	static constexpr uint32_t c_timerFrequency = _2_MHZ;

	void start(const char* msg, const stm32_pwm_config& config, float frequency, float duty) {
		m_driver = config.Driver;
		m_channel = config.Channel;

		m_period = c_timerFrequency / frequency;

		// These timers are only 16 bit - don't risk overflow
		if (m_period > 0xFFF0) {
			firmwareError(ObdCode::CUSTOM_OBD_LOW_FREQUENCY, "PWM Frequency too low %.1f hz on pin \"%s\"", frequency, msg);
			return;
		}

		// If we have too few usable bits, we run out of resolution, so don't allow that either.
		// 200 counts = 0.5% resolution
		if (m_period < 200) {
			firmwareError(ObdCode::CUSTOM_OBD_HIGH_FREQUENCY, "PWM Frequency too high %.1f hz on pin \"%s\"", frequency, msg);
			return;
		}

		const PWMConfig pwmcfg = {
			.frequency = c_timerFrequency,
			.period = m_period,
			.callback = nullptr,
			.channels = {
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr}
			},
			.cr2 = 0,
		    #if STM32_PWM_USE_ADVANCED
		    .bdtr = 0,
		    #endif
			.dier = 0,
		};

		// Start the timer running
		pwmStart(m_driver, &pwmcfg);

		// Set initial duty cycle
		setDuty(duty);
	}

	void setDuty(float duty) override {
		if (!m_driver) {
			criticalError("Attempted to set duty on null hard PWM device");
			return;
		}

		pwm_lld_enable_channel(m_driver, m_channel, getHighTime(duty));
	}

private:
	PWMDriver* m_driver = nullptr;
	uint8_t m_channel = 0;
	uint32_t m_period = 0;

	pwmcnt_t getHighTime(float duty) const {
		return m_period * duty;
	}
};
}

/**
  * Could this be unified with getIcuParams() method?
  */
static expected<stm32_pwm_config> getConfigForPin(brain_pin_e pin) {
	switch (pin) {
#if STM32_PWM_USE_TIM1
	case Gpio::A8:  return stm32_pwm_config{&PWMD1, /*channel*/0, /*AF*/1};
	case Gpio::A9:  return stm32_pwm_config{&PWMD1, /*channel*/1, /*AF*/1};
	case Gpio::A10: return stm32_pwm_config{&PWMD1, 2, 1};
	case Gpio::A11: return stm32_pwm_config{&PWMD1, 3, 1};

	case Gpio::E9:  return stm32_pwm_config{&PWMD1, 0, 1};
	case Gpio::E11: return stm32_pwm_config{&PWMD1, 1, 1};
	case Gpio::E13: return stm32_pwm_config{&PWMD1, 2, 1};
	case Gpio::E14: return stm32_pwm_config{&PWMD1, 3, 1};
#endif
#if STM32_PWM_USE_TIM2
	case Gpio::A15: return stm32_pwm_config{&PWMD2, 0, 1};
	case Gpio::B3:  return stm32_pwm_config{&PWMD2, 1, 1};
	case Gpio::B10: return stm32_pwm_config{&PWMD2, 2, 1};
	case Gpio::B11: return stm32_pwm_config{&PWMD2, 3, 1};
#endif
#if STM32_PWM_USE_TIM3
	case Gpio::B4:  return stm32_pwm_config{&PWMD3, 0, 2};
	case Gpio::B5:  return stm32_pwm_config{&PWMD3, 1, 2};

	case Gpio::C6:  return stm32_pwm_config{&PWMD3, 0, 2};
	case Gpio::C7:  return stm32_pwm_config{&PWMD3, 1, 2};
#endif
#if STM32_PWM_USE_TIM4
	case Gpio::B6:  return stm32_pwm_config{&PWMD4, 0, 2};
	case Gpio::B7:  return stm32_pwm_config{&PWMD4, 1, 2};
	case Gpio::B8:  return stm32_pwm_config{&PWMD4, 2, 2};
	case Gpio::B9:  return stm32_pwm_config{&PWMD4, 3, 2};

	case Gpio::D12: return stm32_pwm_config{&PWMD4, 0, 2};
	case Gpio::D13: return stm32_pwm_config{&PWMD4, 1, 2};
	case Gpio::D14: return stm32_pwm_config{&PWMD4, 2, 2};
	case Gpio::D15: return stm32_pwm_config{&PWMD4, 3, 2};
#endif
#if STM32_PWM_USE_TIM5
	case Gpio::A0:  return stm32_pwm_config{&PWMD5, 0, 2};
	case Gpio::A1:  return stm32_pwm_config{&PWMD5, 1, 2};
	case Gpio::A2:  return stm32_pwm_config{&PWMD5, 2, 2};
	case Gpio::A3:  return stm32_pwm_config{&PWMD5, 3, 2};
#endif
#if STM32_PWM_USE_TIM8

#if !STM32_PWM_USE_TIM3
	// If TIM3 is not used, put these pins on TIM8 instead..
	// See https://github.com/rusefi/rusefi/issues/639
	// See https://github.com/rusefi/rusefi/pull/3032
	case Gpio::C6:  return stm32_pwm_config{&PWMD8, 0, 3};
	case Gpio::C7:  return stm32_pwm_config{&PWMD8, 1, 3};
#endif

	case Gpio::C8:  return stm32_pwm_config{&PWMD8, 2, 3};
	case Gpio::C9:  return stm32_pwm_config{&PWMD8, 3, 3};
#endif
#if STM32_PWM_USE_TIM9
	case Gpio::E5:  return stm32_pwm_config{&PWMD9, 0, 3};
	case Gpio::E6:  return stm32_pwm_config{&PWMD9, 1, 3};
#endif
	default: return unexpected;
	}
};

static stm32_hardware_pwm hardPwms[5];

stm32_hardware_pwm* getNextPwmDevice() {
	for (size_t i = 0; i < efi::size(hardPwms); i++) {
		if (!hardPwms[i].hasInit()) {
			return &hardPwms[i];
		}
	}

	criticalError("Run out of hardware PWM devices!");
	return nullptr;
}

/*static*/ hardware_pwm* hardware_pwm::tryInitPin(const char* msg, brain_pin_e pin, float frequencyHz, float duty) {
	// Hardware PWM can't do very slow PWM - the timer counter is only 16 bits, so at 2MHz counting, that's a minimum of 31hz.
	if (frequencyHz < 50) {
		return nullptr;
	}

	auto cfg = getConfigForPin(pin);

	// This pin can't do hardware PWM
	if (!cfg) {
		return nullptr;
	}

	if (stm32_hardware_pwm* device = getNextPwmDevice()) {
		device->start(msg, cfg.Value, frequencyHz, duty);

		// Finally connect the timer to physical pin
		efiSetPadMode(msg, pin, PAL_MODE_ALTERNATE(cfg.Value.AlternateFunc));

		return device;
	}

	return nullptr;
}
#endif /* HAL_USE_PWM */
