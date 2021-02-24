/**
 * @file        stm32_adc_v2.cpp
 * @brief       Port implementation for the STM32 "v2" ADC found on the STM32F4 and STM32F7
 *
 * @date February 9, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#include "engine_ptr.h"
#include "persistent_configuration.h"

#if HAL_USE_ADC

EXTERN_CONFIG;

void portInitAdc() {
	// Init slow ADC
	adcStart(&ADCD1, NULL);

	// Init fast ADC (MAP sensor)
	adcStart(&ADCD2, NULL);

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
		firmwareError(OBD_PCM_Processor_Fault, "Invalid CPU temperature measured %f", degrees);
	}

	return degrees;
}

#endif // HAL_USE_ADC
