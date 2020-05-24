
#include "global.h"
#include "hal.h"
#include "engine.h"

#include "software_knock.h"

EXTERN_ENGINE;


adcsample_t sampleBuffer[2000];



static volatile bool knockIsSampling = false;
static volatile bool knockNeedsProcess = false;
static volatile size_t sampleCount = 0;

static void completionCallback(ADCDriver*, adcsample_t*, size_t) {
	knockNeedsProcess = true;
	knockIsSampling = false;

	//processLastKnockEvent();
}

static ADCConversionGroup adcConvGroup = { FALSE, 1, &completionCallback, nullptr,
	0,
	ADC_CR2_SWSTART,
	0, // sample times for channels 10...18
	ADC_SMPR2_SMP_AN9(ADC_SAMPLE_84),

	0,	// htr
	0,	// ltr

	0,	// sqr1
	0,	// sqr2
	ADC_SQR3_SQ1_N(ADC_CHANNEL_IN14)	// knock 1 - pin PF4
};

void startKnockSampling(uint8_t cylinderIndex) {
	if (cylinderIndex == 2) {
		return;
	}

	// Don't overlap sampling
	if (knockIsSampling) {
		return;
	}

	knockIsSampling = true;

	// If there's pending processing, skip
	if (knockNeedsProcess) {
		return;
	}

	// Sample for 60 degrees
	float samplingSeconds = ENGINE(rpmCalculator).oneDegreeUs * 60 * 1e-6;
	constexpr int sampleRate = 200000;
	sampleCount = clampF(100, samplingSeconds * sampleRate, efi::size(sampleBuffer));

	adcStartConversionI(&ADCD3, &adcConvGroup, sampleBuffer, sampleCount);
}

void processLastKnockEvent() {
	if (!knockNeedsProcess) {
		return;
	}

	float sum = 0;
	float sumSq = 0;

	constexpr float ratio = 3.3f / 4095.0f;

	// Compute the sum and sum of squares
	for (size_t i = 0; i < sampleCount; i++)
	{
		float volts = ratio * sampleBuffer[i];

		sum += volts;
		sumSq += volts * volts;
	}

	float dcVoltage = sum / sampleCount;
	float squareDcVoltage = dcVoltage * dcVoltage;

	// mean of squares (not yet root)
	float avgSquaresWithDc = sumSq / sampleCount;

	// Compute RMS of just the AC component
	tsOutputChannels.knockLevel /*db*/ = 10 * log10(avgSquaresWithDc - squareDcVoltage);

	knockNeedsProcess = false;
}
