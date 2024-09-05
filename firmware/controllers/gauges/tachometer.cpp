/*
 * @file tachometer.cpp
 * @brief This is about driving external analog tachometers
 *
 * This implementation produces one pulse per engine cycle
 *
 * @date Aug 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "tachometer.h"

static SimplePwm tachControl("tach"); 
static float tachFreq;  
static float duty;   

#if EFI_UNIT_TEST
float getTachFreq() {
	return tachFreq;
}

float getTachDuty() {
	return duty;
}
#endif

static bool tachHasInit = false;

void tachUpdate() {
	// Only do anything if tach enabled
	if (!tachHasInit) {
		return;
	}

	// How many tach pulse periods do we have?
	int periods = engineConfiguration->tachPulsePerRev;

	if (periods == 0 || periods > 10) {
		firmwareError(ObdCode::CUSTOM_ERR_6709, "Invalid tachometer pulse per rev: %d", periods);
		return;
	}

	// What is the angle per tach output period?
	float cycleTimeMs = 60000.0f / Sensor::getOrZero(SensorType::Rpm);
	float periodTimeMs = cycleTimeMs / periods;
	tachFreq = 1000.0f / periodTimeMs;
	
	if (engineConfiguration->tachPulseDurationAsDutyCycle) {
		// Simple case - duty explicitly set
		duty = engineConfiguration->tachPulseDuractionMs;
	} else {
		// Constant high-time mode - compute the correct duty cycle
		duty = engineConfiguration->tachPulseDuractionMs / periodTimeMs;
	}

	// In case Freq is under 1Hz, we stop pwm to avoid warnings!
	if (tachFreq < 1) {
		tachFreq = NAN;
	}
	
	tachControl.setSimplePwmDutyCycle(duty);
	tachControl.setFrequency(tachFreq);
}

void initTachometer() {
	tachHasInit = false;

	if (!isBrainPinValid(engineConfiguration->tachOutputPin)) {
		return;
	}

	startSimplePwm(&tachControl,
				"Tachometer",
				&engine->scheduler,
				&enginePins.tachOut,
				NAN, 0.1f);

	tachHasInit = true;
}
