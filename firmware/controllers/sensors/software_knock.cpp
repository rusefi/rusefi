
#include "global.h"
#include "hal.h"
#include "engine.h"

#include "software_knock.h"

EXTERN_ENGINE;


adcsample_t sampleBuffer[8000];



static volatile bool knockIsSampling = false;
static volatile bool knockNeedsProcess = false;
static volatile size_t sampleCount = 0;

static void completionCallback(ADCDriver*, adcsample_t*, size_t) {
	knockNeedsProcess = true;
}

static void errorCallback(ADCDriver*, adcerror_t err) {
}

static ADCConversionGroup adcConvGroup = { FALSE, 1, &completionCallback, &errorCallback,
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
	if (cylinderIndex == 2) {
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
}

struct biquad {
	float a0, a1, a2, b1, b2;

	float z1 = 0;
	float z2 = 0;

	void reset() {
		z1 = 0;
		z2 = 0;
	}

	float filter(float input) {
		float result = input * a0 + z1;
		z1 = input * a1 + z2 - b1 * result;
		z2 = input * a2 - b2 * result;
		return result;
	}

	void configureBandpass(float samplingFrequency, float centerFrequency, float Q) {
		float K = tanf(3.14159 * centerFrequency / samplingFrequency);
		float norm = 1 / (1 + K / Q + K * K);

		a0 = K / Q * norm;
		a1 = 0;
		a2 = -a0;
		b1 = 2 * (K * K - 1) * norm;
		b2 = (1 - K / Q + K * K) * norm;
	}
};

biquad biquadFilter;

void initSoftwareKnock() {
	biquadFilter.configureBandpass(217000, CONFIG(knockBandCustom), 3);
	adcStart(&ADCD3, nullptr);
}

void processLastKnockEvent() {
	if (!knockNeedsProcess) {
		return;
	}

	float sum = 0;
	float sumSq = 0;

	constexpr float ratio = 3.3f / 4095.0f;

	biquadFilter.reset();
	biquadFilter.configureBandpass(217000, 11000, 10);

	// Compute the sum and sum of squares
	for (size_t i = 0; i < sampleCount; i++)
	{
		float volts = ratio * (sampleBuffer[i] - 2048);

		float filtered = biquadFilter.filter(volts);

		sumSq += filtered * filtered;
	}

	// mean of squares (not yet root)
	float meanSquares = sumSq / sampleCount;

	// RMS
	float db = 10 * log10(meanSquares);

	tsOutputChannels.knockLevel = db;

	knockNeedsProcess = false;
}
