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

#if EFI_ADC3_SLOW
// Thread-context start/read; preemption is called with the system locked.
void initAdc3Slow();
void adc3SlowUpdate();
int adc3SlowRead(adc_channel_e channel);
void adc3SlowPreemptI(ADCDriver* adc);
#endif

#if defined(STM32F4) || defined(STM32F7)
int getAdcInternalChannel(ADC_TypeDef* adc, adc_channel_e hwChannel);
int adcConversionGroupSetSeqInput(ADCConversionGroup* cfg, size_t sqn, size_t input);
int adcConversionGroupGetSeqInput(ADCConversionGroup* cfg, size_t sqn);
#endif
