/**
 * @file        stm32_adc_v4.cpp
 * @brief       Port implementation for the STM32 "v4" ADC found on the STM32H7
 *
 * @date February 25, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#include "pch.h"

#ifndef EFI_SLOW_ADC
#define EFI_SLOW_ADC ADCD1
#endif

#if HAL_USE_ADC

#include "mpu_util.h"
#include "map_averaging.h"

#ifndef H7_ADC_SPEED
#define H7_ADC_SPEED (10000)
#endif

#ifndef H7_ADC_OVERSAMPLE
#define H7_ADC_OVERSAMPLE (4)
#endif

static_assert(H7_ADC_SPEED > 0, "H7_ADC_SPEED must be positive");
static_assert(H7_ADC_OVERSAMPLE > 0 && (H7_ADC_OVERSAMPLE & (H7_ADC_OVERSAMPLE - 1)) == 0, "H7_ADC_OVERSAMPLE must be a power of 2");

// Each scan contains the 16 physical inputs, in F4/F7 channel order.
constexpr size_t slowChannelCount = 16;
#ifdef ADC_MUX_PIN
constexpr size_t slowSampleCount = 2 * slowChannelCount;

// Board-specific analog settling time, including the mux and input RC network.
// Late interrupts skip a timer tick rather than sampling an unsettled bank.
#ifndef H7_ADC_MUX_SETTLE_US
#define H7_ADC_MUX_SETTLE_US 10
#endif
static_assert(H7_ADC_MUX_SETTLE_US > 0, "ADC mux settling time must be positive");

static OutputPin muxControl;
static size_t muxBank = 0;
static efitick_t muxChangedAtNt;
// H7 NO_CACHE currently selects .ram0, which is cached on H743. The linker
// maps .nocache into the MPU non-cacheable region on both H743 and H723.
// Both banks are overwritten before use; this NOLOAD buffer needs no startup clear.
static volatile adcsample_t muxSampleBuffer[slowSampleCount] __attribute__((section(".nocache"), aligned(4)));

static void selectMuxBank(size_t bank) {
	muxBank = bank;
	muxControl.setValue(bank, /*force*/true);
	muxChangedAtNt = getTimeNowNt();
}

static void muxTimerCallback(GPTDriver*);
static void muxAdcErrorCallback(ADCDriver*, adcerror_t);
#else
constexpr size_t slowSampleCount = slowChannelCount;
#endif

#ifdef SLOW_ADC_CHANNEL_COUNT
static_assert(SLOW_ADC_CHANNEL_COUNT >= slowSampleCount, "Slow ADC output buffer is too small");
#endif

static constexpr size_t log2_int(size_t x) {
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
#ifdef ADC_MUX_PIN
	muxControl.initPin("ADC Mux", ADC_MUX_PIN);
	selectMuxBank(0);
#endif

	// Init slow ADC
	adcStart(&EFI_SLOW_ADC, NULL);

#if STM32_ADC_USE_ADC3
	// Knock/trigger scope ADC
	adcStart(&ADCD3, nullptr);
#endif // STM32_ADC_USE_ADC3

	// Connect the analog switches between {PA0_C, PA1_C, PC2_C, PC3_C} and their non-C counterparts
	// This lets us use normal (non-direct) analog on those channels
	SYSCFG->PMCR &= ~(SYSCFG_PMCR_PA0SO | SYSCFG_PMCR_PA1SO | SYSCFG_PMCR_PC2SO | SYSCFG_PMCR_PC3SO);
}

float getMcuTemperature() {
	// Ugh, internal temp sensor is wired to ADC3, which makes it nearly useless on the H7.
	return 0;
}

float getMcuVrefVoltage() {
	// TODO: implement me!
	return engineConfiguration->adcVcc;
}

float getMcuVbatVoltage() {
	// TODO: implement me!
	return 0;
}

adcsample_t* fastSampleBuffer;

static void adc_callback(ADCDriver *adcp) {
#ifdef ADC_MUX_PIN
	(void)adcp;
	// This group is linear.  ChibiOS calls us once after returning to READY.
	{
		// Non-circular DMA has stopped before ChibiOS calls us. Publish only
		// complete pairs: fast consumers must never count an old bank twice.
		const bool pairComplete = muxBank == 1;
		selectMuxBank(muxBank ^ 1);
		if (pairComplete) {
			for (size_t i = 0; i < slowSampleCount; i++) {
				fastSampleBuffer[i] = muxSampleBuffer[i];
			}
			onFastAdcComplete(fastSampleBuffer);
		}
	}
#else
	// State may not be complete if we get a callback for "half done"
	if (adcIsBufferComplete(adcp)) {
		// here we invoke 'fast' from slow ADC due to https://github.com/rusefi/rusefi/issues/3301
		onFastAdcComplete(adcp->samples);
	}
#endif

	assertInterruptPriority(__func__, EFI_IRQ_ADC_PRIORITY);
}

// ADC Clock is 25MHz
// 16.5 sampling + 8.5 conversion = 25 cycles per sample total
// 16 channels * 4x oversample = 64 samples per batch
// (25 * 64) / 25MHz -> 64 microseconds to sample all channels
#define ADC_SAMPLING_SLOW ADC_SMPR_SMP_16P5

// Conversion group for slow channels
// This simply samples every channel in sequence
static constexpr ADCConversionGroup convGroupSlow = {
#ifdef ADC_MUX_PIN
	.circular = false,
#else
	.circular			= true,		// Continuous mode means we will auto re-trigger on every timer event
#endif
	.num_channels		= slowChannelCount,
	.end_cb				= adc_callback,
#ifdef ADC_MUX_PIN
	.error_cb = muxAdcErrorCallback,
	.cfgr = ADC_CFGR_OVRMOD, // One software-started scan per eligible TIM3 tick
#else
	.error_cb			= nullptr,
	// OVRMOD=1 is required by H7 errata "ADC slave data may be shifted in Dual regular simultaneous mode" -
	// without it, a single-DMA-channel read of CDR can leave the slave's samples offset from the master's.
	.cfgr = ADC_CFGR_EXTEN_0 | (4 << ADC_CFGR_EXTSEL_Pos) | ADC_CFGR_OVRMOD, // External trigger ch4, rising edge: TIM3 TRGO
#endif
	.cfgr2				= 	(H7_ADC_OVERSAMPLE - 1) << ADC_CFGR2_OVSR_Pos |	// Oversample by Nx (register contains N-1)
							H7_ADC_SHIFT_BITS << ADC_CFGR2_OVSS_Pos |		// shift the result right log2(N) bits to make a 16 bit result out of the internal oversample sum
							ADC_CFGR2_ROVSE,			// Enable oversampling
	.ccr				= 0,
	.pcsel				= 0xFFFFFFFF, // enable analog switches on all channels
	// Thresholds aren't used
	.ltr1 = 0, .htr1 = 0, .ltr2 = 0, .htr2 = 0, .ltr3 = 0, .htr3 = 0,
	.awd2cr = 0,
	.awd3cr = 0,
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

#ifdef ADC_MUX_PIN
static void muxTimerCallback(GPTDriver*) {
	chSysLockFromISR();
	// ADC_COMPLETE/ADC_ERROR also exclude a preempted completion/error callback.
	// Never re-arm a live conversion, including when sampling takes > one tick.
	if (EFI_SLOW_ADC.state == ADC_READY && getTimeNowNt() - muxChangedAtNt >= US2NT(H7_ADC_MUX_SETTLE_US)) {
		auto* bankSamples = const_cast<adcsample_t*>(&muxSampleBuffer[muxBank * slowChannelCount]);
		adcStartConversionI(&EFI_SLOW_ADC, &convGroupSlow, bankSamples, 1);
	}
	chSysUnlockFromISR();
}

static void muxAdcErrorCallback(ADCDriver*, adcerror_t err) {
	engine->outputChannels.slowAdcErrorCount++;
	if (err & ADC_ERR_OVERFLOW) {
		engine->outputChannels.slowAdcOverrunCount++;
	}
	// ChibiOS stopped DMA and will return to ADC_READY after this callback.
	// Discard the partial pair; the next timer tick retries from bank zero.
	selectMuxBank(0);
}
#endif

static bool didStart = false;

bool readSlowAnalogInputs(adcsample_t* convertedSamples) {
	// This only needs to happen once, as the timer will continue firing the ADC and writing to the buffer without our help

	if (didStart) {
		return true;
	}
	{
		chibios_rt::CriticalSectionLocker csl;
		if (EFI_SLOW_ADC.state != ADC_READY) {
			return false;
		}
		fastSampleBuffer = convertedSamples;
#ifndef ADC_MUX_PIN
		// Oversampling and right-shift happen in hardware, so we can sample directly to the output buffer
		adcStartConversionI(&EFI_SLOW_ADC, &convGroupSlow, convertedSamples, 1);
#endif
	}

	constexpr uint32_t samplingRate = H7_ADC_SPEED;
	constexpr uint32_t timerCountFrequency = samplingRate * 10;
	constexpr uint32_t timerPeriod = timerCountFrequency / samplingRate;

	static constexpr GPTConfig gptCfg = {
		timerCountFrequency,
#ifdef ADC_MUX_PIN
		muxTimerCallback,
		0,	// No TRGO: only the callback may start a scan after checking settling
#else
		nullptr,
		TIM_CR2_MMS_1,	// TRGO on update event
#endif
		0
	};

	// Start timer
	gptStart(&GPTD3, &gptCfg);
	gptStartContinuous(&GPTD3, timerPeriod);
	didStart = true;

	// Return true if OK
	return true;
}

AdcToken enableFastAdcChannel(const char*, adc_channel_e channel) {
	if (channel < EFI_ADC_0 || channel >= EFI_ADC_0 + slowSampleCount) {
		return invalidAdcToken;
	}

	// H7 always samples all fast channels, nothing to do here but compute index
	return channel - EFI_ADC_0;
}

adcsample_t getFastAdc(AdcToken token) {
	if (!fastSampleBuffer || token >= slowSampleCount) {
		return 0;
	}

	return fastSampleBuffer[token];
}

#ifdef EFI_SOFTWARE_KNOCK
#include "knock_config.h"

static_assert((H7_KNOCK_OVERSAMPLE & (H7_KNOCK_OVERSAMPLE - 1)) == 0, "H7_ADC_OVERSAMPLE must be a power of 2");
static constexpr int H7_KNOCK_ADC_SHIFT_BITS = log2_int(H7_KNOCK_OVERSAMPLE);

static void knockCompletionCallback(ADCDriver*) {
	// Knock conversions are linear, so the callback is the completion event.
	onKnockSamplingComplete();

	assertInterruptPriority(__func__, EFI_IRQ_ADC_PRIORITY);
}

static void knockErrorCallback(ADCDriver*, adcerror_t) {
}

static const uint32_t smpr1 =
	ADC_SMPR1_SMP_AN0(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN1(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN2(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN3(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN4(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN5(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN6(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN7(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN8(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN9(KNOCK_SAMPLE_TIME);

static const uint32_t smpr2 =
	ADC_SMPR2_SMP_AN10(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN11(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN12(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN13(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN14(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN15(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN16(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN17(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN18(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN19(KNOCK_SAMPLE_TIME);

static const ADCConversionGroup adcConvGroupCh1 = {
	.circular = FALSE,
	.num_channels = 1,
	.end_cb = &knockCompletionCallback,
	.error_cb = &knockErrorCallback,
	.cfgr				= 0,
	.cfgr2				= 	(H7_KNOCK_OVERSAMPLE - 1) << ADC_CFGR2_OVSR_Pos |	// Oversample by Nx (register contains N-1)
							H7_KNOCK_ADC_SHIFT_BITS << ADC_CFGR2_OVSS_Pos |		// shift the result right log2(N) bits to make a 16 bit result out of the internal oversample sum
							ADC_CFGR2_ROVSE,			// Enable oversampling
	.ccr				= 0,
	.pcsel				= 0xFFFFFFFF, // enable analog switches on all channels
	// Thresholds aren't used
	.ltr1 = 0, .htr1 = 0, .ltr2 = 0, .htr2 = 0, .ltr3 = 0, .htr3 = 0,
	.awd2cr = 0,
	.awd3cr = 0,
	.smpr = {smpr1, smpr2},
	.sqr = {
		ADC_SQR1_SQ1_N(KNOCK_ADC_CH1),
		0,
		0
	},
};

// Not all boards have a second channel - configure it if it exists
#if KNOCK_HAS_CH2
static const ADCConversionGroup adcConvGroupCh2 = {
	.circular = FALSE,
	.num_channels = 1,
	.end_cb = &knockCompletionCallback,
	.error_cb = &knockErrorCallback,
	.cfgr				= 0,
	.cfgr2				= 	(H7_ADC_OVERSAMPLE - 1) << ADC_CFGR2_OVSR_Pos |	// Oversample by Nx (register contains N-1)
							H7_ADC_SHIFT_BITS << ADC_CFGR2_OVSS_Pos |		// shift the result right log2(N) bits to make a 16 bit result out of the internal oversample sum
							ADC_CFGR2_ROVSE,			// Enable oversampling
	.ccr				= 0,
	.pcsel				= 0xFFFFFFFF, // enable analog switches on all channels
	// Thresholds aren't used
	.ltr1 = 0, .htr1 = 0, .ltr2 = 0, .htr2 = 0, .ltr3 = 0, .htr3 = 0,
	.awd2cr = 0,
	.awd3cr = 0,
	.smpr = {smpr1, smpr2},
	.sqr = {
		ADC_SQR1_SQ1_N(KNOCK_ADC_CH2),
		0,
		0
	},
};
#endif // KNOCK_HAS_CH2

const ADCConversionGroup* getKnockConversionGroup(uint8_t channelIdx) {
#if KNOCK_HAS_CH2
	if (channelIdx == 1) {
		return &adcConvGroupCh2;
	}
#else
	(void)channelIdx;
#endif // KNOCK_HAS_CH2

	return &adcConvGroupCh1;
}

#endif // EFI_SOFTWARE_KNOCK

#endif // HAL_USE_ADC
