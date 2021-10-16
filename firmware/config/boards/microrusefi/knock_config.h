/**
 * @file knock_config.h
 */

#pragma once

// Knock is on ADC3
#define KNOCK_ADC ADCD3

// knock 1 - pin PA3
#define KNOCK_ADC_CH1 ADC_CHANNEL_IN3
#define KNOCK_PIN_CH1 GPIOA_3

// Sample rate & time - depends on the exact MCU
#define KNOCK_SAMPLE_TIME ADC_SAMPLE_84
#define KNOCK_SAMPLE_RATE (STM32_PCLK2 / (4 * (84 + 12)))
