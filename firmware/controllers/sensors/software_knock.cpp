
#include "global.h"
#include "hal.h"
#include "engine.h"
#include "biquad.h"
#include "perf_trace.h"
#include "thread_controller.h"

#include "software_knock.h"

EXTERN_ENGINE;


adcsample_t sampleBuffer[8000];
Biquad knockFilter;

static volatile bool knockIsSampling = false;
static volatile bool knockNeedsProcess = false;
static volatile size_t sampleCount = 0;

binary_semaphore_t knockSem;

static void completionCallback(ADCDriver* adcp, adcsample_t*, size_t) {
	palClearPad(GPIOD, 2);

	if (adcp->state == ADC_COMPLETE) {
		knockNeedsProcess = true;

		chSysLockFromISR();
		chBSemSignalI(&knockSem);
		chSysUnlockFromISR();
	}
}

static void errorCallback(ADCDriver*, adcerror_t err) {
}

static const ADCConversionGroup adcConvGroup = { FALSE, 1, &completionCallback, &errorCallback,
	0,
	ADC_CR2_SWSTART,
	ADC_SMPR1_SMP_AN14(ADC_SAMPLE_84), // sample times for channels 10...18
	0,

	0,	// htr
	0,	// ltr

	0,	// sqr1
	0,	// sqr2
	ADC_SQR3_SQ1_N(ADC_CHANNEL_IN14)	// knock 1 - pin PF4
};

void startKnockSampling(uint8_t cylinderIndex) {
	if (cylinderIndex != 2) {
		return;
	}

	// Cancel if ADC isn't ready
	if (!((ADCD3.state == ADC_READY) ||
			(ADCD3.state == ADC_COMPLETE) ||
			(ADCD3.state == ADC_ERROR))) {
		return;
	}

	// If there's pending processing, skip
	if (knockNeedsProcess) {
		return;
	}

	// Sample for 60 degrees
	float samplingSeconds = ENGINE(rpmCalculator).oneDegreeUs * 45 * 1e-6;
	constexpr int sampleRate = 217000;
	sampleCount = 0xFFFFFFFE & static_cast<size_t>(clampF(100, samplingSeconds * sampleRate, efi::size(sampleBuffer)));

	adcStartConversionI(&ADCD3, &adcConvGroup, sampleBuffer, sampleCount);
	palSetPad(GPIOD, 2);
}

class KnockThread : public ThreadController<256> {
public:
	KnockThread() : ThreadController("knock", NORMALPRIO - 10) {}
	void ThreadTask() override;
};

KnockThread kt;

void initSoftwareKnock() {
	chBSemObjectInit(&knockSem, TRUE);
	knockFilter.configureBandpass(217000, 11500, 3);
	adcStart(&ADCD3, nullptr);
	palSetPadMode(GPIOD, 2, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOD, 4, PAL_MODE_OUTPUT_PUSHPULL);

	kt.Start();
}

void processLastKnockEvent() {
	if (!knockNeedsProcess) {
		return;
	}

	palSetPad(GPIOD, 4);


	float sumSq = 0;

	constexpr float ratio = 3.3f / 4095.0f;

	knockFilter.reset();

	size_t localCount = sampleCount;

	// Compute the sum and sum of squares
	for (size_t i = 0; i < localCount; i++)
	{
		float volts = ratio * (sampleBuffer[i] - 2048);

		float filtered = knockFilter.filter(volts);

		sumSq += filtered * filtered;
	}

	// mean of squares (not yet root)
	float meanSquares = sumSq / localCount;

	// RMS
	float db = 10 * log10(meanSquares);

	tsOutputChannels.knockLevel = db;

	knockNeedsProcess = false;

	palClearPad(GPIOD, 4);
}

void KnockThread::ThreadTask() {
	while(1) {
		chBSemWait(&knockSem);

		ScopePerf perf(PE::SoftwareKnockProcess);
		processLastKnockEvent();
	}
}
