/**
 * @file	adc_hack.cpp
 * @brief	Hacky support for a single channel on adc3
 *
 * @date December 17, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#include "ch.h"
#include "hal.h"

#include "mpu_util.h"
#include "io_pins.h"

#include "efilib.h"

static ADCConversionGroup adcConvGroup = { FALSE, 1, nullptr, nullptr,
	0,
	ADC_CR2_SWSTART,
	0, // sample times for channels 10...18
	ADC_SMPR2_SMP_AN9(ADC_SAMPLE_56),

	0,	// htr
	0,	// ltr

	0,	// sqr1
	0,	// sqr2
	ADC_SQR3_SQ1_N(ADC_CHANNEL_IN9)	// sqr3 - vbatt is on pf3 = adc9
};

__ALIGNED(32) adcsample_t samples[8];

// we use this as a hook to run near the rest of ADC init...
void setAdcChannelOverrides(void) {
	efiSetPadMode("adc input", GPIOF_3, PAL_MODE_INPUT_ANALOG);

	adcStart(&ADCD3, nullptr);
}

adcsample_t vbattSampleProteus = 0;

void proteusAdcHack()
{
	adcConvert(&ADCD3, &adcConvGroup, samples, 8);
#if defined(STM32F7XX)
	SCB_InvalidateDCache_by_Addr(reinterpret_cast<uint32_t*>(samples), sizeof(samples));
#endif /* STM32F7XX */

	uint32_t sum = 0;

	for (int i = 0; i < 8; i++) {
		sum += samples[i];
	}

	vbattSampleProteus = sum / efi::size(samples);
}
