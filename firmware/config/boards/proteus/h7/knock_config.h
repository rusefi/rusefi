/**
 * @file knock_config.h
 */

#pragma once

// Knock is on ADC3
#define KNOCK_ADC ADCD3

// knock 1 - pin PF4
#define KNOCK_PIN_CH1 Gpio::F4

// knock 2 - pin PF5
#define KNOCK_HAS_CH2 true
#define KNOCK_PIN_CH2 Gpio::F5

#define KNOCK_ADC_CH1 9
#define KNOCK_ADC_CH2 4

#define H7_KNOCK_OVERSAMPLE 4
#define KNOCK_SAMPLE_TIME ADC_SMPR_SMP_8P5
#define KNOCK_SAMPLE_RATE (STM32_ADC3_CLOCK / (H7_KNOCK_OVERSAMPLE * (8 + 8)))
