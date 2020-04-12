/**
 * @file	stm32_common.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "efi_gpio.h"

#ifndef EFI_PIN_ADC9
#define EFI_PIN_ADC9 GPIOB_1
#endif /* EFI_PIN_ADC9 */

#if EFI_PROD_CODE
#include "mpu_util.h"
#include "backup_ram.h"
extern ioportid_t PORTS[];
#if defined(STM32F4XX) || defined(STM32F7XX)
ioportid_t PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH };
#else
ioportid_t PORTS[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOF};
#endif /* defined(STM32F4XX) || defined(STM32F7XX) */
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
struct stm32_hardware_pwm : public hardware_pwm {
	const brain_pin_e BrainPin;
	PWMDriver* const Driver;
	const uint8_t Channel;
	const uint8_t AlternateFunc;

	stm32_hardware_pwm(brain_pin_e pin, PWMDriver* drv, uint8_t channel, uint8_t altFunc)
		: BrainPin(pin)
		, Driver(drv)
		, Channel(channel)
		, AlternateFunc(altFunc)
	{
	}

	uint32_t m_period;

	pwmcnt_t getHighTime(float duty) const {
		return m_period * duty;
	}

	// 2MHz, 16-bit timer gets us a usable frequency range of 31hz to 10khz
	static constexpr uint32_t c_timerFrequency = 2000000;

	void start(const char* msg, float frequency, float duty) {
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
		pwmStart(Driver, &pwmcfg);

		// Set initial duty cycle
		setDuty(duty);

		// Finally connect the timer to physical pin
		efiSetPadMode(msg, BrainPin, PAL_MODE_ALTERNATE(AlternateFunc));
	}

	void setDuty(float duty) override {
		pwm_lld_enable_channel(Driver, Channel, getHighTime(duty));
	}
};

stm32_hardware_pwm pwmChannels[] = {
	stm32_hardware_pwm(GPIOB_6, &PWMD4, 0, 2),
	stm32_hardware_pwm(GPIOB_7, &PWMD4, 1, 2),
	stm32_hardware_pwm(GPIOB_8, &PWMD4, 2, 2),
	stm32_hardware_pwm(GPIOB_9, &PWMD4, 3, 2),

	stm32_hardware_pwm(GPIOC_6, &PWMD8, 0, 3),
	stm32_hardware_pwm(GPIOC_7, &PWMD8, 1, 3),
	stm32_hardware_pwm(GPIOC_8, &PWMD8, 2, 3),
	stm32_hardware_pwm(GPIOC_9, &PWMD8, 3, 3),

	stm32_hardware_pwm(GPIOD_12, &PWMD4, 0, 2),
	stm32_hardware_pwm(GPIOD_13, &PWMD4, 1, 2),
	stm32_hardware_pwm(GPIOD_14, &PWMD4, 2, 2),
	stm32_hardware_pwm(GPIOD_15, &PWMD4, 3, 2),
};

/*static*/ hardware_pwm* hardware_pwm::tryInitPin(const char* msg, brain_pin_e pin, float frequencyHz, float duty)
{
	for (size_t i = 0; i < efi::size(pwmChannels); i++) {
		auto& channel = pwmChannels[i];
		if (channel.BrainPin == pin) {
			channel.start(msg, frequencyHz, duty);

			return &channel;
		}
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
