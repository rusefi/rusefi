/*
 * @file knock_logic.c
 *
 * @date Apr 04, 2021
 * @author Andrey Gusakov
 */

#include "pch.h"
#include "knock_logic.h"
#include "os_access.h"
#include "peak_detect.h"

int getCylinderKnockBank(uint8_t cylinderIndex) {
	// C/C++ can't index in to bit fields, we have to provide lookup ourselves
	switch (cylinderIndex) {
#if EFI_PROD_CODE
		case 0:
			return CONFIG(knockBankCyl1);
		case 1:
			return CONFIG(knockBankCyl2);
		case 2:
			return CONFIG(knockBankCyl3);
		case 3:
			return CONFIG(knockBankCyl4);
		case 4:
			return CONFIG(knockBankCyl5);
		case 5:
			return CONFIG(knockBankCyl6);
		case 6:
			return CONFIG(knockBankCyl7);
		case 7:
			return CONFIG(knockBankCyl8);
		case 8:
			return CONFIG(knockBankCyl9);
		case 9:
			return CONFIG(knockBankCyl10);
		case 10:
			return CONFIG(knockBankCyl11);
		case 11:
			return CONFIG(knockBankCyl12);
#endif
		default:
			return 0;
	}
}

using PD = PeakDetect<float, MS2NT(100)>;
static PD peakDetectors[12];
static PD allCylinderPeakDetector;

void onKnockSenseCompleted(uint8_t cylinderIndex, float dbv, efitick_t lastKnockTime) {
#if EFI_TUNER_STUDIO
	// Pass through per-cylinder peak detector
	float cylPeak = peakDetectors[cylinderIndex].detect(dbv, lastKnockTime);
	tsOutputChannels.knockLevels[cylinderIndex] = roundf(cylPeak);

	// Pass through all-cylinders peak detector
	tsOutputChannels.knockLevel = allCylinderPeakDetector.detect(dbv, lastKnockTime);

	// If this was a knock, count it!
	bool isKnock = dbv > ENGINE(engineState).knockThreshold;
	if (isKnock) {
		tsOutputChannels.knockCount++;
	}
#endif // EFI_TUNER_STUDIO

	// TODO: retard timing, then put it back!
}

// This callback is to be implemented by the knock sense driver
void onStartKnockSampling(uint8_t cylinderIndex, float samplingTimeSeconds, uint8_t channelIdx);

static uint8_t cylinderIndexCopy;

// Called when its time to start listening for knock
// Does some math, then hands off to the driver to start any sampling hardware
static void startKnockSampling(void*) {
	if (!engine->rpmCalculator.isRunning()) {
		return;
	}

	// Convert sampling angle to time
	float samplingSeconds = ENGINE(rpmCalculator).oneDegreeUs * CONFIG(knockSamplingDuration) / US_PER_SECOND_F;

	// Look up which channel this cylinder uses
	auto channel = getCylinderKnockBank(cylinderIndexCopy);

	// ugly as hell but that's error: cast between incompatible function types from 'void (*)(uint8_t)' {aka 'void (*)(unsigned char)'} to 'schfunc_t' {aka 'void (*)(void*)'} [-Werror=cast-function-type]
	onStartKnockSampling(cylinderIndexCopy, samplingSeconds, channel);
}

static scheduling_s startSampling;

void onSparkFireKnockSense(uint8_t cylinderIndex, efitick_t nowNt) {
	cylinderIndexCopy = cylinderIndex;

	scheduleByAngle(&startSampling, nowNt,
			/*angle*/CONFIG(knockDetectionWindowStart), startKnockSampling PASS_ENGINE_PARAMETER_SUFFIX);

#if EFI_HIP_9011
	hip9011_onFireEvent(cylinderIndex, nowNt);
#endif
}
