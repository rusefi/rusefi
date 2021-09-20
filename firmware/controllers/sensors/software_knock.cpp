#include "pch.h"

#include "biquad.h"
#include "thread_controller.h"
#include "knock_logic.h"
#include "software_knock.h"
#include "peak_detect.h"

#if EFI_SOFTWARE_KNOCK

#include "knock_config.h"
#include "ch.hpp"

static NO_CACHE adcsample_t sampleBuffer[2000];
static int8_t currentCylinderIndex = 0;
static efitick_t lastKnockSampleTime = 0;
static Biquad knockFilter;

static volatile bool knockIsSampling = false;
static volatile bool knockNeedsProcess = false;
static volatile size_t sampleCount = 0;
static int cylinderIndexCopy;

chibios_rt::BinarySemaphore knockSem(/* taken =*/ true);

static void completionCallback(ADCDriver* adcp) {
	palClearPad(GPIOD, 2);

	if (adcp->state == ADC_COMPLETE) {
		knockNeedsProcess = true;

		// Notify the processing thread that it's time to process this sample
		chSysLockFromISR();
		knockSem.signalI();
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
#endif // KNOCK_HAS_CH2

const ADCConversionGroup* getConversionGroup(uint8_t cylinderIndex) {
#if KNOCK_HAS_CH2
	if (getCylinderKnockBank(cylinderIndex)) {
		return &adcConvGroupCh2;
	}
#else
	(void)cylinderIndex;
#endif // KNOCK_HAS_CH2

	return &adcConvGroupCh1;
}

static void startKnockSampling(uint8_t cylinderIndex) {
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

	// Sample for XX degrees
	float samplingSeconds = ENGINE(rpmCalculator).oneDegreeUs * CONFIG(knockSamplingDuration) / US_PER_SECOND_F;
	constexpr int sampleRate = KNOCK_SAMPLE_RATE;
	sampleCount = 0xFFFFFFFE & static_cast<size_t>(clampF(100, samplingSeconds * sampleRate, efi::size(sampleBuffer)));

	// Select the appropriate conversion group - it will differ depending on which sensor this cylinder should listen on
	auto conversionGroup = getConversionGroup(cylinderIndex);

	// Stash the current cylinder's index so we can store the result appropriately
	currentCylinderIndex = cylinderIndex;

	adcStartConversionI(&KNOCK_ADC, conversionGroup, sampleBuffer, sampleCount);
	lastKnockSampleTime = getTimeNowNt();
}

static void startKnockSamplingNoParam(void *arg) {
	// ugly as hell but that's error: cast between incompatible function types from 'void (*)(uint8_t)' {aka 'void (*)(unsigned char)'} to 'schfunc_t' {aka 'void (*)(void*)'} [-Werror=cast-function-type]
	startKnockSampling(cylinderIndexCopy);
}

static scheduling_s startSampling;

void knockSamplingCallback(uint8_t cylinderIndex, efitick_t nowNt) {
	cylinderIndexCopy = cylinderIndex;

	scheduleByAngle(&startSampling, nowNt,
			/*angle*/CONFIG(knockDetectionWindowStart), startKnockSamplingNoParam PASS_ENGINE_PARAMETER_SUFFIX);
}

class KnockThread : public ThreadController<256> {
public:
	KnockThread() : ThreadController("knock", PRIO_KNOCK_PROCESS) {}
	void ThreadTask() override;
};

static KnockThread kt;

void initSoftwareKnock() {
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

using PD = PeakDetect<float, MS2NT(100)>;
static PD peakDetectors[12];
static PD allCylinderPeakDetector;

void processLastKnockEvent() {
	if (!knockNeedsProcess) {
		return;
	}

	float sumSq = 0;

	// todo: reduce magic constants. engineConfiguration->adcVcc?
	constexpr float ratio = 3.3f / 4095.0f;

	size_t localCount = sampleCount;

	// Prepare the steady state at vcc/2 so that there isn't a step
	// when samples begin
	// todo: reduce magic constants. engineConfiguration->adcVcc?
	knockFilter.cookSteadyState(3.3f / 2);

	// Compute the sum of squares
	for (size_t i = 0; i < localCount; i++) {
		float volts = ratio * sampleBuffer[i];

		float filtered = knockFilter.filter(volts);
		if (i == localCount - 1 && engineConfiguration->debugMode == DBG_KNOCK) {
			tsOutputChannels.debugFloatField1 = volts;
			tsOutputChannels.debugFloatField2 = filtered;
		}

		sumSq += filtered * filtered;
	}

	// take a local copy
	auto lastKnockTime = lastKnockSampleTime;

	// We're done with inspecting the buffer, another sample can be taken
	knockNeedsProcess = false;

	// mean of squares (not yet root)
	float meanSquares = sumSq / localCount;

	// RMS
	float db = 10 * log10(meanSquares);

	// clamp to reasonable range
	db = clampF(-100, db, 100);

	// Pass through peak detector
	float cylPeak = peakDetectors[currentCylinderIndex].detect(db, lastKnockTime);

	tsOutputChannels.knockLevels[currentCylinderIndex] = roundf(cylPeak);
	tsOutputChannels.knockLevel = allCylinderPeakDetector.detect(db, lastKnockTime);

	// If this was a knock, count it!
	bool isKnock = db > ENGINE(engineState).knockThreshold;
	if (isKnock) {
		tsOutputChannels.knockCount++;
	}
}

void KnockThread::ThreadTask() {
	while (1) {
		knockSem.wait();

		ScopePerf perf(PE::SoftwareKnockProcess);
		processLastKnockEvent();
	}
}

#endif // EFI_SOFTWARE_KNOCK
