/**
 * @file        stm32_adc_v4.cpp
 * @brief       Port implementation for the STM32 "v4" ADC found on the STM32H7
 *
 * @date February 25, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#include "pch.h"
#include "mpu_util.h"
#include "map_averaging.h"

#ifndef H7_ADC_SPEED
#define H7_ADC_SPEED (10000)
#endif

#ifndef H7_ADC_OVERSAMPLE
#define H7_ADC_OVERSAMPLE (4)
#endif

static_assert((H7_ADC_OVERSAMPLE & (H7_ADC_OVERSAMPLE - 1)) == 0, "H7_ADC_OVERSAMPLE must be a power of 2");

constexpr size_t log2_int(size_t x) {
	size_t result = 0;
	while (x >>= 1) result++;
	return result;
}

// poor man's unit test
static_assert(log2_int(4) == 2);
static_assert(log2_int(16) == 4);

// Shift the result by log2(N) bits to divide by N
static constexpr int H7_ADC_SHIFT_BITS = log2_int(H7_ADC_OVERSAMPLE);

void portInitAdc() {
	// Init slow ADC
	adcStart(&ADCD1, NULL);

	// Connect the analog switches between {PA0_C, PA1_C, PC2_C, PC3_C} and their non-C counterparts
	// This lets us use normal (non-direct) analog on those channels
	SYSCFG->PMCR &= ~(SYSCFG_PMCR_PA0SO | SYSCFG_PMCR_PA1SO | SYSCFG_PMCR_PC2SO | SYSCFG_PMCR_PC3SO);
}

float getMcuTemperature() {
	// Ugh, internal temp sensor is wired to ADC3, which makes it nearly useless on the H7.
	return 0;
}

adcsample_t* fastSampleBuffer;

static void adc_callback(ADCDriver *adcp) {
	// State may not be complete if we get a callback for "half done"
	if (adcp->state == ADC_COMPLETE) {
		onFastAdcComplete(adcp->samples);
	}
}

// ADC Clock is 25MHz
// 16.5 sampling + 8.5 conversion = 25 cycles per sample total
// 16 channels * 4x oversample = 64 samples per batch
// (25 * 64) / 25MHz -> 64 microseconds to sample all channels
#define ADC_SAMPLING_SLOW ADC_SMPR_SMP_16P5

// Sample the 16 channels that line up with the STM32F4/F7
constexpr size_t slowChannelCount = 16;

// Conversion group for slow channels
// This simply samples every channel in sequence
static constexpr ADCConversionGroup convGroupSlow = {
	.circular			= true,		// Continuous mode means we will auto re-trigger on every timer event
	.num_channels		= slowChannelCount,
	.end_cb				= adc_callback,
	.error_cb			= nullptr,
	.cfgr				= ADC_CFGR_EXTEN_0 | (4 << ADC_CFGR_EXTSEL_Pos),	// External trigger ch4, rising edge: TIM3 TRGO
	.cfgr2				= 	(H7_ADC_OVERSAMPLE - 1) << ADC_CFGR2_OVSR_Pos |	// Oversample by Nx (register contains N-1)
							H7_ADC_SHIFT_BITS << ADC_CFGR2_OVSS_Pos |		// shift the result right log2(N) bits to make a 16 bit result out of the internal oversample sum
							ADC_CFGR2_ROVSE,			// Enable oversampling
	.ccr				= 0,
	.pcsel				= 0xFFFFFFFF, // enable analog switches on all channels
	// Thresholds aren't used
	.ltr1 = 0, .htr1 = 0, .ltr2 = 0, .htr2 = 0, .ltr3 = 0, .htr3 = 0,
	.smpr = {
		// Configure all channels to use ADC_SAMPLING_SLOW time
		ADC_SMPR1_SMP_AN0(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN1(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN2(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN3(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN4(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN5(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN6(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN7(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN8(ADC_SAMPLING_SLOW) |
		ADC_SMPR1_SMP_AN9(ADC_SAMPLING_SLOW),
		ADC_SMPR2_SMP_AN10(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN11(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN12(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN13(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN14(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN15(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN16(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN17(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN18(ADC_SAMPLING_SLOW) |
		ADC_SMPR2_SMP_AN19(ADC_SAMPLING_SLOW)
	},
	.sqr = {
		// The seemingly insane values here exist to put the values
		// in the buffer in the same order as the ADCv2 (F4/F7) ADC
		ADC_SQR1_SQ1_N(16) |	// PA0 (aka PA0_C)
		ADC_SQR1_SQ2_N(17) |	// PA1 (aka PA1_C)
		ADC_SQR1_SQ3_N(14) |	// PA2
		ADC_SQR1_SQ4_N(15),		// PA3
		ADC_SQR2_SQ5_N(18) |	// PA4
		ADC_SQR2_SQ6_N(19) |	// PA5
		ADC_SQR2_SQ7_N(3) |		// PA6
		ADC_SQR2_SQ8_N(7) |		// PA7
		ADC_SQR2_SQ9_N(9),		// PB0
		ADC_SQR3_SQ10_N(5) |	// PB1
		ADC_SQR3_SQ11_N(10) |	// PC0
		ADC_SQR3_SQ12_N(11) |	// PC1
		ADC_SQR3_SQ13_N(12) |	// PC2 (aka PC2_C)
		ADC_SQR3_SQ14_N(13),	// PC3 (aka PC3_C)
		ADC_SQR4_SQ15_N(4) |	// PC4
		ADC_SQR4_SQ16_N(8)		// PC5
	},
};

static bool didStart = false;

bool readSlowAnalogInputs(adcsample_t* convertedSamples) {
	// This only needs to happen once, as the timer will continue firing the ADC and writing to the buffer without our help
	if (didStart) {
		return true;
	}
	didStart = true;

	fastSampleBuffer = convertedSamples;

	{
		chibios_rt::CriticalSectionLocker csl;
		// Oversampling and right-shift happen in hardware, so we can sample directly to the output buffer
		adcStartConversionI(&ADCD1, &convGroupSlow, convertedSamples, 1);
	}

	constexpr uint32_t samplingRate = H7_ADC_SPEED;
	constexpr uint32_t timerCountFrequency = samplingRate * 10;
	constexpr uint32_t timerPeriod = timerCountFrequency / samplingRate;

	static constexpr GPTConfig gptCfg = {
		timerCountFrequency,
		nullptr,
		TIM_CR2_MMS_1,	// TRGO on update event
		0
	};

	// Start timer
	gptStart(&GPTD3, &gptCfg);
	gptStartContinuous(&GPTD3, timerPeriod);

	// Return true if OK
	return true;
}

static constexpr FastAdcToken invalidToken = (FastAdcToken)(-1);

FastAdcToken enableFastAdcChannel(const char*, adc_channel_e channel) {
	if (!isAdcChannelValid(channel)) {
		return invalidToken;
	}

	// H7 always samples all fast channels, nothing to do here but compute index
	return channel - EFI_ADC_0;
}

adcsample_t getFastAdc(FastAdcToken token) {
	if (token == invalidToken) {
		return 0;
	}

	return fastSampleBuffer[token];
}
