#pragma once

// Knock is on ADC3
#define KNOCK_ADC ADCD3

// knock 1 - pin PA3
#define KNOCK_ADC_CH1 ADC_CHANNEL_IN3
#define KNOCK_PIN_CH1 GPIOA_3

// knock 2 - pin PF5 NOT used! - not used for MRE
#define KNOCK_ADC_CH2 ADC_CHANNEL_IN15
#define KNOCK_PIN_CH2 GPIOF_5

// Sample rate & time - depends on the exact MCU
#define KNOCK_SAMPLE_TIME ADC_SAMPLE_84
#define KNOCK_SAMPLE_RATE (STM32_PCLK2 / (4 * (84 + 12)))

#ifndef MRE_SW_KNOCK_ADC
    #define MRE_SW_KNOCK_ADC
#endif
