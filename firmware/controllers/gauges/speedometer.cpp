#include "pch.h"

/**
 * GMT800 platform (1999–2007 classic Chevrolet Silverado/GMC Sierra, Tahoe, Yukon, etc.)
 * 4000 pulses per mile
 * 2485 pulses per kilometer
 */

#include "speedometer.h"

static SimplePwm speedoPwm("speedo");

static bool hasSpeedoInit = false;

// "Test Speedo" bench test: overrides the normal VehicleSpeed-derived frequency for a fixed duration.
static constexpr float SPEEDO_BENCH_TEST_DURATION_SEC = 3.0f;
static Timer speedoBenchTimer;
static bool speedoBenchActive = false;
static float speedoBenchFreq = NAN;

void startSpeedoBenchTest(float freqHz) {
	if (!hasSpeedoInit) {
		efiPrintf("Speedo output pin is not configured, can not bench test");
		return;
	}

	// Match normal speed output: below 1 Hz pauses PWM; zero is not a valid PWM frequency.
	speedoBenchFreq = freqHz < 1 ? NAN : freqHz;
	speedoBenchTimer.reset();
	speedoBenchActive = true;
}

void speedoUpdate() {
	if (!hasSpeedoInit) {
		return;
	}

	if (speedoBenchActive) {
		if (speedoBenchTimer.hasElapsedSec(SPEEDO_BENCH_TEST_DURATION_SEC)) {
			speedoBenchActive = false;
		} else {
			speedoPwm.setFrequency(speedoBenchFreq);
			return;
		}
	}

	float kph = Sensor::getOrZero(SensorType::VehicleSpeed);
	float kps = kph * (1. / 3600);
	float freq = kps * engineConfiguration->speedometerPulsePerKm;

	if (freq < 1) {
		freq = NAN;
	}

	speedoPwm.setFrequency(freq);
}

void initSpeedometer() {
	hasSpeedoInit = false;

	if (!isBrainPinValid(engineConfiguration->speedometerOutputPin)) {
		return;
	}

	startSimplePwm(&speedoPwm,
					"Speedometer",
					&engine->scheduler,
					&enginePins.speedoOut,
					NAN, 0.5f);

	hasSpeedoInit = true;
}
