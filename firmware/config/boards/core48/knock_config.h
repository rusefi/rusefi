/**
 * @file knock_config.h
 */

#pragma once

// Knock is on ADC3
#define KNOCK_ADC ADCD3

// knock 1 - pin PC0
#define KNOCK_ADC_CH1 ADC_CHANNEL_IN10
#define KNOCK_PIN_CH1 Gpio::C0

// knock 2 - pin PC1
#define KNOCK_HAS_CH2 true
#define KNOCK_ADC_CH2 ADC_CHANNEL_IN11
#define KNOCK_PIN_CH2 Gpio::C1

// Sample rate & time - depends on the exact MCU
#define KNOCK_SAMPLE_TIME ADC_SAMPLE_84
#define KNOCK_SAMPLE_RATE (STM32_PCLK2 / (4 * (84 + 12)))