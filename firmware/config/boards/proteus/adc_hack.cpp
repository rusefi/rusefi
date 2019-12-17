
#include "ch.h"
#include "hal.h"

#include "mpu_util.h"
#include "io_pins.h"

#include "efilib.h"

#define ADC_SAMPLING_SLOW ADC_SAMPLE_56

static ADCConversionGroup adcConvGroup = { FALSE, 1, nullptr, NULL,
	ADC_TwoSamplingDelay_20Cycles,
	ADC_CR2_SWSTART,

		ADC_SMPR1_SMP_AN10(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN11(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN12(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN13(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN14(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN15(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144)
		, // sample times for channels 10...18
		ADC_SMPR2_SMP_AN0(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN1(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN2(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN3(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN4(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN5(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN6(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN7(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN8(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN9(ADC_SAMPLING_SLOW),

		0,	// htr
		0,	// ltr

		0,	// sqr1
		0,	// sqr2
		9	// sqr3 - vbatt is on pf3 = adc9
};

adcsample_t samples[8];

// we use this as a hook to run near the rest of ADC init...
void setAdcChannelOverrides(void) {
	brain_pin_e vbattPin = GPIOF_3;
	efiSetPadMode("adc input", vbattPin, PAL_MODE_INPUT_ANALOG);

	adcStart(&ADCD3, nullptr);
}

adcsample_t vbattSampleProteus = 0;

void proteusAdcHack()
{
	adcConvert(&ADCD3, &adcConvGroup, samples, 8);

	uint32_t sum = 0;

	for (const auto s : samples) {
		sum += s;
	}

	vbattSampleProteus = sum / efi::size(samples);
}
