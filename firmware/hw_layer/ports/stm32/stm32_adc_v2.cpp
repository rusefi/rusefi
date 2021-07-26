/**
 * @file        stm32_adc_v2.cpp
 * @brief       Port implementation for the STM32 "v2" ADC found on the STM32F4 and STM32F7
 *
 * @date February 9, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#include "pch.h"

#if HAL_USE_ADC

/* Depth of the conversion buffer, channels are sampled X times each.*/
#define SLOW_ADC_OVERSAMPLE      8

void portInitAdc() {
	// Init slow ADC
	adcStart(&ADCD1, NULL);

#if EFI_USE_FAST_ADC
	// Init fast ADC (MAP sensor)
	adcStart(&ADCD2, NULL);
#endif

	// Enable internal temperature reference
	adcSTM32EnableTSVREFE(); // Internal temperature sensor

#if defined(STM32F7XX)
	/* the temperature sensor is internally
	 * connected to the same input channel as VBAT. Only one conversion,
	 * temperature sensor or VBAT, must be selected at a time. */
	adcSTM32DisableVBATE();
#endif

	/* Enable this code only when you absolutly sure
	 * that there is no possible errors from ADC */
#if 0
	/* All ADC use DMA and DMA calls end_cb from its IRQ
	 * If none of ADC users need error callback - we can disable
	 * shared ADC IRQ and save some CPU ticks */
	if ((adcgrpcfgSlow.error_cb == NULL) &&
		(adcgrpcfgFast.error_cb == NULL)
		/* TODO: Add ADC3? */) {
		nvicDisableVector(STM32_ADC_NUMBER);
	}
#endif
}

/*
 * ADC conversion group.
 */
static const ADCConversionGroup tempSensorConvGroup = {
	.circular			= FALSE,
	.num_channels		= 1,
	.end_cb				= nullptr,
	.error_cb			= nullptr,
	/* HW dependent part below */
	.cr1				= 0,
	.cr2				= ADC_CR2_SWSTART,
	// sample times for channels 10...18
	.smpr1 =
		ADC_SMPR1_SMP_VBAT(ADC_SAMPLE_144)    |	/* input18 - temperature and vbat input on some STM32F7xx */
		ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144),	/* input16 - temperature sensor input on STM32F4xx */
	.smpr2 = 0,
	.htr = 0, .ltr = 0,
	.sqr1 = 0,
	.sqr2 = 0,
#if defined(STM32F4XX)
	.sqr3 = ADC_SQR3_SQ1_N(16),
#endif
#if defined(STM32F7XX)
	.sqr3 = ADC_SQR3_SQ1_N(18),
#endif
};

// 4x oversample is plenty
static constexpr int oversample = 4;
static adcsample_t samples[oversample];

float getMcuTemperature() {
	// Temperature sensor is only physically wired to ADC1
	adcConvert(&ADCD1, &tempSensorConvGroup, samples, oversample);

	uint32_t sum = 0;
	for (size_t i = 0; i < oversample; i++) {
		sum += samples[i];
	}

	float volts = (float)sum / (4096 * oversample);
	volts *= CONFIG(adcVcc);

	volts -= 0.760f; // Subtract the reference voltage at 25 deg C
	float degrees = volts / 0.0025f; // Divide by slope 2.5mV

	degrees += 25.0; // Add the 25 deg C

	if (degrees > 150.0f || degrees < -50.0f) {
/*
 * we have a sporadic issue with this check todo https://github.com/rusefi/rusefi/issues/2552
		firmwareError(OBD_PCM_Processor_Fault, "Invalid CPU temperature measured %f", degrees);
 */
	}

	return degrees;
}

// See https://github.com/rusefi/rusefi/issues/976 for discussion on these values
#define ADC_SAMPLING_SLOW ADC_SAMPLE_56
#define ADC_SAMPLING_FAST ADC_SAMPLE_28

// Slow ADC has 16 channels we can sample
constexpr size_t slowChannelCount = 16;

// Conversion group for slow channels
// This simply samples every channel in sequence
static constexpr ADCConversionGroup convGroupSlow = {
	.circular			= FALSE,
	.num_channels		= slowChannelCount,
	.end_cb				= nullptr,
	.error_cb			= nullptr,
	/* HW dependent part.*/
	.cr1				= 0,
	.cr2				= ADC_CR2_SWSTART,
	// Configure all channels to ADC_SAMPLING_SLOW sample time
	.smpr1 =
		ADC_SMPR1_SMP_AN10(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN11(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN12(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN13(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN14(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN15(ADC_SAMPLING_SLOW),
	.smpr2 =
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
	.htr	= 0,
	.ltr	= 0,
	// Simply sequence every channel in order
	.sqr1	= ADC_SQR1_SQ13_N(12) | ADC_SQR1_SQ14_N(13) | ADC_SQR1_SQ15_N(14) | ADC_SQR1_SQ16_N(15) | ADC_SQR1_NUM_CH(16), // Conversion group sequence 13...16 + sequence length
	.sqr2	= ADC_SQR2_SQ7_N(6)   | ADC_SQR2_SQ8_N(7)   | ADC_SQR2_SQ9_N(8)   | ADC_SQR2_SQ10_N(9)  | ADC_SQR2_SQ11_N(10) | ADC_SQR2_SQ12_N(11), // Conversion group sequence 7...12
	.sqr3	= ADC_SQR3_SQ1_N(0)   | ADC_SQR3_SQ2_N(1)   | ADC_SQR3_SQ3_N(2)   |  ADC_SQR3_SQ4_N(3)  |   ADC_SQR3_SQ5_N(4) |   ADC_SQR3_SQ6_N(5), // Conversion group sequence 1...6
};

static NO_CACHE adcsample_t slowSampleBuffer[SLOW_ADC_OVERSAMPLE * slowChannelCount];

bool readSlowAnalogInputs(adcsample_t* convertedSamples) {
	msg_t result = adcConvert(&ADCD1, &convGroupSlow, slowSampleBuffer, SLOW_ADC_OVERSAMPLE);

	// If something went wrong - try again later
	if (result != MSG_OK) {
		return false;
	}

	// Average samples to get some noise filtering and oversampling
	for (size_t i = 0; i < slowChannelCount; i++) {
		uint32_t sum = 0;
		size_t index = i;
		for (size_t j = 0; j < SLOW_ADC_OVERSAMPLE; j++) {
			sum += slowSampleBuffer[index];
			index += slowChannelCount;
		}

		adcsample_t value = static_cast<adcsample_t>(sum / SLOW_ADC_OVERSAMPLE);
		convertedSamples[i] = value;
	}

	return true;
}

#endif // HAL_USE_ADC
