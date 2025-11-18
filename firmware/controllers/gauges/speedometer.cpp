#include "pch.h"

/**
 * GMT800 platform (1999–2007 classic Chevrolet Silverado/GMC Sierra, Tahoe, Yukon, etc.)
 * 4000 pulses per mile
 * 2485 pulses per kilometer
 */

#include "speedometer.h"

static SimplePwm speedoPwm("speedo");

static bool hasSpeedoInit = false;

void speedoUpdate() {
	if (!hasSpeedoInit) {
		return;
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
