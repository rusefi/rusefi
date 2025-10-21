/**
 * @file knock_config.h
 */

#pragma once

// Define the ADC for Knock
#define KNOCK_ADC ADCD3

// Knock sensor 1 settings - Pin PC0
#define KNOCK_ADC_CH1 ADC_CHANNEL_IN10
#define KNOCK_PIN_CH1 Gpio::C0

// Knock sensor 2 settings - Pin PC1
#define KNOCK_HAS_CH2 true
#define KNOCK_ADC_CH2 ADC_CHANNEL_IN11
#define KNOCK_PIN_CH2 Gpio::C1

// Set the sample rate & time (varies by MCU)
#define KNOCK_SAMPLE_TIME ADC_SAMPLE_84
#define KNOCK_SAMPLE_RATE (STM32_PCLK2 / (4 * (84 + 12)))
