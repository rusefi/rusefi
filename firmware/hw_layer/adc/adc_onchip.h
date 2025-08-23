/**
 * @file	adc_onchip.h
 * @brief	Low level ChibiOS ADC helpers header
 *
 * @date Aug 23, 2025
 * @author Andrey Gusakov, (c) 2025
 */

#pragma once

#include "global.h"

#ifndef ADC_MAX_CHANNELS_COUNT
#define ADC_MAX_CHANNELS_COUNT 16
#endif /* ADC_MAX_CHANNELS_COUNT */

#if defined(STM32F4) || defined(STM32F7)
int adcConversionGroupSetSeqInput(ADCConversionGroup* cfg, size_t sqn, size_t input);
int adcConversionGroupGetSeqInput(ADCConversionGroup* cfg, size_t sqn);
#endif
