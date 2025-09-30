#pragma once

// knock 1 - pin PA2
#define KNOCK_PIN_CH1 Gpio::A2

// Sample rate & time - depends on the exact MCU
#if defined(STM32H7XX)
	// TODO: ADC1 and ADC2 are tightly coupled and ChibiOS represent both as single ADC12 devide...
	// H7
	// Knock is on ADC2
	#define KNOCK_ADC ADCD2

	// ADC12_INP14
	#define KNOCK_ADC_CH1 14

	#define H7_KNOCK_OVERSAMPLE 4
	#define KNOCK_SAMPLE_TIME ADC_SMPR_SMP_8P5
	#define KNOCK_SAMPLE_RATE (STM32_ADC3_CLOCK / (H7_KNOCK_OVERSAMPLE * (8 + 8)))
#else
	// F4/F7
	// Knock is on ADC3
	#define KNOCK_ADC ADCD3

	#define KNOCK_ADC_CH1 ADC_CHANNEL_IN2

	#define KNOCK_SAMPLE_TIME ADC_SAMPLE_84
	#define KNOCK_SAMPLE_RATE (STM32_PCLK2 / (4 * (84 + 12)))
#endif
