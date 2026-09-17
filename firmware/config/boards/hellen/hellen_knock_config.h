/**
 * @file hellen_knock_config.h
 */

#pragma once

// Knock is on ADC3
#define KNOCK_ADC ADCD3

// knock 1 - pin PF7
#define KNOCK_PIN_CH1 Gpio::F7

// Sample rate & time - depends on the exact MCU
#if defined(STM32H7XX)
	// H7
	#define KNOCK_ADC_CH1 3

	#define H7_KNOCK_OVERSAMPLE 4
	#define KNOCK_SAMPLE_TIME ADC_SMPR_SMP_8P5
	#define KNOCK_SAMPLE_RATE (STM32_ADC3_CLOCK / (H7_KNOCK_OVERSAMPLE * (8 + 8)))
#else
	#define KNOCK_ADC_CH1 ADC_CHANNEL_IN5

	#define KNOCK_SAMPLE_TIME ADC_SAMPLE_84
	#define KNOCK_SAMPLE_RATE (STM32_PCLK2 / (4 * (84 + 12)))
#endif
