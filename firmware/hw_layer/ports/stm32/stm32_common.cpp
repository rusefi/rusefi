/**
 * @file	stm32_common.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "efi_gpio.h"
#include "expected.h"

#ifndef EFI_PIN_ADC9
#define EFI_PIN_ADC9 GPIOB_1
#endif /* EFI_PIN_ADC9 */

#if EFI_PROD_CODE
#include "mpu_util.h"
#include "backup_ram.h"
#endif /* EFI_PROD_CODE */

#if HAL_USE_ADC

// ADC_CHANNEL_IN0 // PA0
// ADC_CHANNEL_IN1 // PA1
// ADC_CHANNEL_IN2 // PA2
// ADC_CHANNEL_IN3 // PA3
// ADC_CHANNEL_IN4 // PA4
// ADC_CHANNEL_IN5 // PA5 - this is also TIM2_CH1
// ADC_CHANNEL_IN6 // PA6
// ADC_CHANNEL_IN7 // PA7
// ADC_CHANNEL_IN8 // PB0
// ADC_CHANNEL_IN9 // PB1
// ADC_CHANNEL_IN10 // PC0
// ADC_CHANNEL_IN11 // PC1
// ADC_CHANNEL_IN12 // PC2
// ADC_CHANNEL_IN13 // PC3
// ADC_CHANNEL_IN14 // PC4
// ADC_CHANNEL_IN15 // PC5

brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel) {
	static_assert(EFI_ADC_NONE == ADC_CHANNEL_NONE);

	// todo: replace this with an array :)
	switch (hwChannel) {
	case EFI_ADC_0:
		return GPIOA_0;
	case EFI_ADC_1:
		return GPIOA_1;
	case EFI_ADC_2:
		return GPIOA_2;
	case EFI_ADC_3:
		return GPIOA_3;
	case EFI_ADC_4:
		return GPIOA_4;
	case EFI_ADC_5:
		return GPIOA_5;
	case EFI_ADC_6:
		return GPIOA_6;
	case EFI_ADC_7:
		return GPIOA_7;
	case EFI_ADC_8:
		return GPIOB_0;
	case EFI_ADC_9:
		return EFI_PIN_ADC9;
	case EFI_ADC_10:
		return GPIOC_0;
	case EFI_ADC_11:
		return GPIOC_1;
	case EFI_ADC_12:
		return GPIOC_2;
	case EFI_ADC_13:
		return GPIOC_3;
	case EFI_ADC_14:
		return GPIOC_4;
	case EFI_ADC_15:
		return GPIOC_5;
	default:
		firmwareError(CUSTOM_ERR_ADC_UNKNOWN_CHANNEL, "Unknown hw channel %d [%s]", hwChannel, msg);
		return GPIO_INVALID;
	}
}

adc_channel_e getAdcChannel(brain_pin_e pin) {
	switch (pin) {
	case GPIOA_0:
		return EFI_ADC_0;
	case GPIOA_1:
		return EFI_ADC_1;
	case GPIOA_2:
		return EFI_ADC_2;
	case GPIOA_3:
		return EFI_ADC_3;
	case GPIOA_4:
		return EFI_ADC_4;
	case GPIOA_5:
		return EFI_ADC_5;
	case GPIOA_6:
		return EFI_ADC_6;
	case GPIOA_7:
		return EFI_ADC_7;
	case GPIOB_0:
		return EFI_ADC_8;
	case EFI_PIN_ADC9:
		return EFI_ADC_9;
	case GPIOC_0:
		return EFI_ADC_10;
	case GPIOC_1:
		return EFI_ADC_11;
	case GPIOC_2:
		return EFI_ADC_12;
	case GPIOC_3:
		return EFI_ADC_13;
	case GPIOC_4:
		return EFI_ADC_14;
	case GPIOC_5:
		return EFI_ADC_15;
	case GPIO_UNASSIGNED:
		return EFI_ADC_NONE;
	default:
		return EFI_ADC_ERROR;
	}
}

// deprecated - inline?
ioportid_t getAdcChannelPort(const char *msg, adc_channel_e hwChannel) {
	brain_pin_e brainPin = getAdcChannelBrainPin(msg, hwChannel);
	return getHwPort(msg, brainPin);
}

// deprecated - inline?
int getAdcChannelPin(adc_channel_e hwChannel) {
	brain_pin_e brainPin = getAdcChannelBrainPin("get_pin", hwChannel);
	return getHwPin("get_pin", brainPin);
}

#endif /* HAL_USE_ADC */

#if EFI_PROD_CODE

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
	static constexpr uint32_t c_timerFrequency = 2000000;

	void start(const char* msg, const stm32_pwm_config& config, float frequency, float duty) {
		m_driver = config.Driver;
		m_channel = config.Channel;

		m_period = c_timerFrequency / frequency;

		// These timers are only 16 bit - don't risk overflow
		if (m_period > 0xFFF0) {
			firmwareError(CUSTOM_OBD_LOW_FREQUENCY, "PWM Frequency too low %f hz on pin \"%s\"", frequency, msg);
			return;
		}

		// If we have too few usable bits, we run out of resolution, so don't allow that either.
		// 200 counts = 0.5% resolution
		if (m_period < 200) {
			firmwareError(CUSTOM_OBD_HIGH_FREQUENCY, "PWM Frequency too high % hz on pin \"%s\"", frequency, msg);
			return;
		}

		const PWMConfig pwmcfg = {
			c_timerFrequency,
			m_period,
			nullptr,
			{
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr},
				{PWM_OUTPUT_ACTIVE_HIGH, nullptr}
			},
			0,
			0
		};

		// Start the timer running
		pwmStart(m_driver, &pwmcfg);

		// Set initial duty cycle
		setDuty(duty);
	}

	void setDuty(float duty) override {
		if (!m_driver) {
			firmwareError(OBD_PCM_Processor_Fault, "Attempted to set duty on null hard PWM device");
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

static expected<stm32_pwm_config> getConfigForPin(brain_pin_e pin) {
	switch (pin) {
#if STM32_PWM_USE_TIM1
	case GPIOA_8: return stm32_pwm_config{&PWMD1, 0, 1};
	case GPIOA_9: return stm32_pwm_config{&PWMD1, 1, 1};
	case GPIOA_10: return stm32_pwm_config{&PWMD1, 2, 1};
	case GPIOA_11: return stm32_pwm_config{&PWMD1, 3, 1};

	case GPIOE_9: return stm32_pwm_config{&PWMD1, 0, 1};
	case GPIOE_11: return stm32_pwm_config{&PWMD1, 1, 1};
	case GPIOE_13: return stm32_pwm_config{&PWMD1, 2, 1};
	case GPIOE_14: return stm32_pwm_config{&PWMD1, 3, 1};
#endif
#if STM32_PWM_USE_TIM2
	case GPIOA_15: return stm32_pwm_config{&PWMD2, 0, 1};
	case GPIOB_3: return stm32_pwm_config{&PWMD2, 1, 1};
	case GPIOB_10: return stm32_pwm_config{&PWMD2, 2, 1};
	case GPIOB_11: return stm32_pwm_config{&PWMD2, 3, 1};
#endif
#if STM32_PWM_USE_TIM3
	case GPIOB_4: return stm32_pwm_config{&PWMD3, 0, 2};
	case GPIOB_5: return stm32_pwm_config{&PWMD3, 1, 2};
#endif
#if STM32_PWM_USE_TIM4
	case GPIOB_6: return stm32_pwm_config{&PWMD4, 0, 2};
	case GPIOB_7: return stm32_pwm_config{&PWMD4, 1, 2};
	case GPIOB_8: return stm32_pwm_config{&PWMD4, 2, 2};
	case GPIOB_9: return stm32_pwm_config{&PWMD4, 3, 2};

	case GPIOD_12: return stm32_pwm_config{&PWMD4, 0, 2};
	case GPIOD_13: return stm32_pwm_config{&PWMD4, 1, 2};
	case GPIOD_14: return stm32_pwm_config{&PWMD4, 2, 2};
	case GPIOD_15: return stm32_pwm_config{&PWMD4, 3, 2};
#endif
#if STM32_PWM_USE_TIM8
	case GPIOC_6: return stm32_pwm_config{&PWMD8, 0, 3};
	case GPIOC_7: return stm32_pwm_config{&PWMD8, 1, 3};
	case GPIOC_8: return stm32_pwm_config{&PWMD8, 2, 3};
	case GPIOC_9: return stm32_pwm_config{&PWMD8, 3, 3};
#endif
	default: return unexpected;
	}
};

stm32_hardware_pwm pwms[5];

stm32_hardware_pwm* getNextPwmDevice() {
	for (size_t i = 0; i < efi::size(pwms); i++) {
		if (!pwms[i].hasInit()) {
			return &pwms[i];
		}
	}

	firmwareError(OBD_PCM_Processor_Fault, "Run out of hardware PWM devices!");
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
#endif

void jump_to_bootloader() {
	// leave DFU breadcrumb which assmebly startup code would check, see [rusefi][DFU] section in assembly code
	*((unsigned long *)0x2001FFF0) = 0xDEADBEEF; // End of RAM
	// and now reboot
	NVIC_SystemReset();
}
#endif /* EFI_PROD_CODE */

#if EFI_AUX_SERIAL

static bool isValidUART6TxPin(brain_pin_e pin) {
	return pin == GPIOC_6 || pin == GPIOG_14;
}

static bool isValidUART6RxPin(brain_pin_e pin) {
	return pin == GPIOC_7 || pin == GPIOG_9;
}

bool isValidSerialTxPin(brain_pin_e pin) {
   return isValidUART6TxPin(pin);
}

bool isValidSerialRxPin(brain_pin_e pin) {
   return isValidUART6RxPin(pin);
}

#endif /*EFI_AUX_SERIAL*/

uint32_t getTimeNowLowerNt() {
	return port_rt_get_counter_value();
}
