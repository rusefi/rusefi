/**
 * @file	adc_onchip.cpp
 * @brief	Low level ChibiOS ADC helpers code
 *
 * @date Aug 23, 2025
 * @author Andrey Gusakov, (c) 2025
 */

#include "pch.h"

#if HAL_USE_ADC

#include "adc_onchip.h"

// Some helpers for ADCConversionGroup manipulations
// mostly for STM32 devices. Should we move this to ports?

#if defined(STM32F4) || defined(STM32F7)

int adcConversionGroupSetSeqInput(ADCConversionGroup* cfg, size_t sqn, size_t input) {
	if (sqn < 6) {
		size_t shift = 5 * (sqn  - 0);
		cfg->sqr3 = (cfg->sqr3 & (~(0x1f << shift))) |
					(input << shift);
	} else if (sqn < 12) {
		size_t shift = 5 * (sqn  - 6);
		cfg->sqr2 = (cfg->sqr2 & (~(0x1f << shift))) |
					(input << shift);
	} else if (sqn < 18) {
		size_t shift = 5 * (sqn - 12);
		cfg->sqr1 = (cfg->sqr1 & (~(0x1f << shift))) |
					(input << shift);
#if ADC_MAX_CHANNELS_COUNT > 16
	} else if (sqn < 24) {
		size_t shift = 5 * (sqn - 18);
		cfg->sqr4 = (cfg->sqr4 & (~(0x1f << shift))) |
					(input << shift);
	} else if (sqn < 30) {
		size_t shift = 5 * (sqn - 24);
		cfg->sqr5 = (cfg->sqr5 & (~(0x1f << shift))) |
					(input << shift);
	}
#endif /* ADC_MAX_CHANNELS_COUNT */
	} else {
		return -1;
	}

	return 0;
}

int adcConversionGroupGetSeqInput(ADCConversionGroup* cfg, size_t sqn) {
	if (sqn < 6) {
		size_t shift = 5 * (sqn  - 0);
		return (cfg->sqr3 >> shift) & 0x1f;
	} else if (sqn < 12) {
		size_t shift = 5 * (sqn  - 6);
		return (cfg->sqr2 >> shift) & 0x1f;
	} else if (sqn < 18) {
		size_t shift = 5 * (sqn - 12);
		return (cfg->sqr1 >> shift) & 0x1f;
#if ADC_MAX_CHANNELS_COUNT > 16
	} else if (sqn < 24) {
		size_t shift = 5 * (sqn - 18);
		return (cfg->sqr4 >> shift) & 0x1f;
	} else if (sqn < 30) {
		size_t shift = 5 * (sqn - 24);
		return (cfg->sqr5 >> shift) & 0x1f;
#endif /* ADC_MAX_CHANNELS_COUNT */
	}

	return -1;
}

#endif // STM32F4 || STM32F7

#endif // HAL_USE_ADC
