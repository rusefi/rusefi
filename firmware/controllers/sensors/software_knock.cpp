
#include "global.h"
#include "engine.h"
#include "biquad.h"
#include "perf_trace.h"
#include "thread_controller.h"
#include "software_knock.h"

#if EFI_SOFTWARE_KNOCK

EXTERN_ENGINE;

#include "knock_config.h"

adcsample_t sampleBuffer[2000];
int8_t currentCylinderIndex = 0;
Biquad knockFilter;

static volatile bool knockIsSampling = false;
static volatile bool knockNeedsProcess = false;
static volatile size_t sampleCount = 0;

binary_semaphore_t knockSem;

static void completionCallback(ADCDriver* adcp, adcsample_t*, size_t) {
	palClearPad(GPIOD, 2);

	if (adcp->state == ADC_COMPLETE) {
		knockNeedsProcess = true;

		// Notify the processing thread that it's time to process this sample
		chSysLockFromISR();
		chBSemSignalI(&knockSem);
		chSysUnlockFromISR();
	}
}

static void errorCallback(ADCDriver*, adcerror_t) {
}

static const uint32_t smpr1 = 
	ADC_SMPR1_SMP_AN10(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN11(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN12(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN13(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN14(KNOCK_SAMPLE_TIME) |
	ADC_SMPR1_SMP_AN15(KNOCK_SAMPLE_TIME);

static const uint32_t smpr2 =
	ADC_SMPR2_SMP_AN0(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN1(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN2(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN3(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN4(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN5(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN6(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN7(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN8(KNOCK_SAMPLE_TIME) |
	ADC_SMPR2_SMP_AN9(KNOCK_SAMPLE_TIME);

static const ADCConversionGroup adcConvGroupCh1 = { FALSE, 1, &completionCallback, &errorCallback,
	0,
	ADC_CR2_SWSTART,
	// sample times for channels 10...18
	smpr1,
	// sample times for channels 0...9
	smpr2,

	0,	// htr
	0,	// ltr

	0,	// sqr1
	0,	// sqr2
	ADC_SQR3_SQ1_N(KNOCK_ADC_CH1)
};

// Not all boards have a second channel - configure it if it exists
#if KNOCK_HAS_CH2
static const ADCConversionGroup adcConvGroupCh2 = { FALSE, 1, &completionCallback, &errorCallback,
	0,
	ADC_CR2_SWSTART,
	// sample times for channels 10...18
	smpr1,
	// sample times for channels 0...9
	smpr2,

	0,	// htr
	0,	// ltr

	0,	// sqr1
	0,	// sqr2
	ADC_SQR3_SQ1_N(KNOCK_ADC_CH2)
};

static bool cylinderUsesChannel2(uint8_t cylinderIndex) {
	// C/C++ can't index in to bit fields, we have to provide lookup ourselves
	switch (cylinderIndex) {
		case 0: return CONFIG(knockBankCyl1);
		case 1: return CONFIG(knockBankCyl2);
		case 2: return CONFIG(knockBankCyl3);
		case 3: return CONFIG(knockBankCyl4);
		case 4: return CONFIG(knockBankCyl5);
		case 5: return CONFIG(knockBankCyl6);
		case 6: return CONFIG(knockBankCyl7);
		case 7: return CONFIG(knockBankCyl8);
		case 8: return CONFIG(knockBankCyl9);
		case 9: return CONFIG(knockBankCyl10);
		case 10: return CONFIG(knockBankCyl11);
		case 11: return CONFIG(knockBankCyl12);
		default: return false;
	}
}

#endif // KNOCK_HAS_CH2

const ADCConversionGroup* getConversionGroup(uint8_t cylinderIndex) {
#if KNOCK_HAS_CH2
	if (cylinderUsesChannel2(cylinderIndex)) {
		return &adcConvGroupCh2;
	}
#endif // KNOCK_HAS_CH2

	return &adcConvGroupCh1;
}

void startKnockSampling(uint8_t cylinderIndex) {
	if (!CONFIG(enableSoftwareKnock)) {
		return;
	}

	if (!engine->rpmCalculator.isRunning()) {
		return;
	}

	// Cancel if ADC isn't ready
	if (!((KNOCK_ADC.state == ADC_READY) ||
			(KNOCK_ADC.state == ADC_COMPLETE) ||
			(KNOCK_ADC.state == ADC_ERROR))) {
		return;
	}

	// If there's pending processing, skip this event
	if (knockNeedsProcess) {
		return;
	}

	// Sample for 45 degrees
	float samplingSeconds = ENGINE(rpmCalculator).oneDegreeUs * 45 * 1e-6;
	constexpr int sampleRate = KNOCK_SAMPLE_RATE;
	sampleCount = 0xFFFFFFFE & static_cast<size_t>(clampF(100, samplingSeconds * sampleRate, efi::size(sampleBuffer)));

	// Select the appropriate conversion group - it will differ depending on which sensor this cylinder should listen on
	auto conversionGroup = getConversionGroup(cylinderIndex);

	// Stash the current cylinder's index so we can store the result appropriately
	currentCylinderIndex = cylinderIndex;

	adcStartConversionI(&KNOCK_ADC, conversionGroup, sampleBuffer, sampleCount);
}

class KnockThread : public ThreadController<256> {
public:
	KnockThread() : ThreadController("knock", NORMALPRIO - 10) {}
	void ThreadTask() override;
};

static KnockThread kt;

void initSoftwareKnock() {
	chBSemObjectInit(&knockSem, TRUE);

	if (CONFIG(enableSoftwareKnock)) {
		knockFilter.configureBandpass(KNOCK_SAMPLE_RATE, 1000 * CONFIG(knockBandCustom), 3);
		adcStart(&KNOCK_ADC, nullptr);

		efiSetPadMode("knock ch1", KNOCK_PIN_CH1, PAL_MODE_INPUT_ANALOG);
#if KNOCK_HAS_CH2		
		efiSetPadMode("knock ch2", KNOCK_PIN_CH2, PAL_MODE_INPUT_ANALOG);
#endif
		kt.Start();
	}
}

void processLastKnockEvent() {
	if (!knockNeedsProcess) {
		return;
	}

	float sumSq = 0;

	constexpr float ratio = 3.3f / 4095.0f;

	size_t localCount = sampleCount;

	// Prepare the steady state at vcc/2 so that there isn't a step
	// when samples begin
	knockFilter.cookSteadyState(3.3f / 2);

	// Compute the sum of squares
	for (size_t i = 0; i < localCount; i++)
	{
		float volts = ratio * sampleBuffer[i];

		float filtered = knockFilter.filter(volts);

		sumSq += filtered * filtered;
	}

	// mean of squares (not yet root)
	float meanSquares = sumSq / localCount;

	// RMS
	float db = 10 * log10(meanSquares);

	tsOutputChannels.knockLevels[currentCylinderIndex] = roundf(clampF(-100, db, 100));
	tsOutputChannels.knockLevel = db;

	knockNeedsProcess = false;
}

void KnockThread::ThreadTask() {
	while(1) {
		chBSemWait(&knockSem);

		ScopePerf perf(PE::SoftwareKnockProcess);
		processLastKnockEvent();
	}
}

#endif // EFI_SOFTWARE_KNOCK
