/**
 * @file        stm32_adc_v2.cpp
 * @brief       Port implementation for the STM32 "v2" ADC found on the STM32F4 and STM32F7
 *
 * @date February 9, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#include "engine_ptr.h"
#include "persistent_configuration.h"

EXTERN_CONFIG;

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

float getMcuTemperature() {
	// 4x oversample is plenty
	constexpr int oversample = 4;

	// Buffer is a full 32 bytes to occupy a full cache line
	__ALIGNED(32) adcsample_t samples[16];

	// Temperature sensor is only physically wired to ADC1
	adcConvert(&ADCD1, &tempSensorConvGroup, samples, oversample);

#if CORTEX_MODEL == 7
	// The STM32F7xx/STM32H7xx has a data cache
	// DMA operations DO NOT invalidate cache lines, since the ARM m7 doesn't have 
	// anything like a CCI that maintains coherency across multiple bus masters.
	// As a result, we have to manually invalidate the D-cache any time we (the CPU)
	// would like to read something that somebody else wrote (ADC via DMA, in this case)
	SCB_InvalidateDCache_by_Addr(reinterpret_cast<uint32_t*>(samples), sizeof(samples));
#endif /* CORTEX_MODEL == 7 */

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
