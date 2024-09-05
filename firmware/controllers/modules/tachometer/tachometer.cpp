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

void TachometerModule::onFastCallback() {
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
	float cycleTimeMs = 60000.0f / getRpm();
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

float TachometerModule::getRpm() {
	float trueRpm = Sensor::getOrZero(SensorType::Rpm);

	if (!m_doTachSweep) {
		return trueRpm;
	}

	float elapsed = m_stateChangeTimer.getElapsedSeconds();
	float sweepPosition = elapsed / engineConfiguration->tachSweepTime;

	if (sweepPosition > 1) {
		// We've done a full sweep time, we're done!
		m_doTachSweep = false;
		return trueRpm;
	} else if (sweepPosition < 0.5f) {
		// First half of the ramp, ramp up from 0 -> max
		return interpolateClamped(0, 0, 0.5f, engineConfiguration->tachSweepMax, sweepPosition);
	} else {
		// Use y2 = trueRpm instead of 0 so that it ramps back down smoothly
		// to the current RPM if the engine started during ther ramp
		return interpolateClamped(0.5f, engineConfiguration->tachSweepMax, 1, trueRpm, sweepPosition);
	}
}

void TachometerModule::onIgnitionStateChanged(bool ignitionOn) {
	if (ignitionOn && engineConfiguration->tachSweepTime != 0) {
		m_stateChangeTimer.reset();
		m_doTachSweep = true;
	}
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
