
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

	// If there's pending processing, skip
	if (knockNeedsProcess) {
		return;
	}

	// Sample for 60 degrees
	float samplingSeconds = ENGINE(rpmCalculator).oneDegreeUs * 60 * 1e-6;
	constexpr int sampleRate = 217000;
	sampleCount = 0xFFFFFFFE & static_cast<size_t>(clampF(100, samplingSeconds * sampleRate, efi::size(sampleBuffer)));

	adcStartConversionI(&ADCD3, &adcConvGroup, sampleBuffer, sampleCount);
}

struct biquad {
	float b0, b1, b2, a1, a2;

	float w0 = 0;
	float w1 = 0;
	float w2 = 0;

	void reset() {
		w0 = 0;
		w1 = 0;
		w2 = 0;
	}

	float filter(float input) {
		// Feedback part
		float accumulator = w2 * a2;
		accumulator += w1 * a1;
		accumulator += input;

		w0 = accumulator;

		// Feedforward part
		accumulator = w0 * b0;
		accumulator += w1 * b1;
		accumulator += w2 * b2;

		// Shuffle history
		w2 = w1;
		w1 = w0;

		return accumulator;
	}
};


biquad firstStage {0.01448672709139305, 0.0289734541827861, 0.01448672709139305, 1.7842324802680964, -0.9036658556014872};
biquad secondStage {0.25, -0.5, 0.25, 1.858554646427233, -0.926428331962136};

void processLastKnockEvent() {
	if (!knockNeedsProcess) {
		return;
	}

	float sum = 0;
	float sumSq = 0;

	constexpr float ratio = 3.3f / 4095.0f;

	firstStage.reset();
	secondStage.reset();

	// Compute the sum and sum of squares
	for (size_t i = 0; i < sampleCount; i++)
	{
		float volts = ratio * sampleBuffer[i];

		float inter = firstStage.filter(volts);
		float filtered = secondStage.filter(inter);

		//sum += filtered;
		sumSq += filtered * filtered;
	}

	//float dcVoltage = sum / sampleCount;
	//float squareDcVoltage = dcVoltage * dcVoltage;

	// mean of squares (not yet root)
	float avgSquaresWithDc = sumSq / sampleCount;

	// Compute RMS of just the AC component
	tsOutputChannels.knockLevel /*db*/ = 10 * log10(avgSquaresWithDc/* - squareDcVoltage*/);

	knockNeedsProcess = false;
}
