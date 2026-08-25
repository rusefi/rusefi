/**
 * @file m74_9_vr_model.cpp
 *
 * VR amplitude model for the L9779 auto-adaptive hysteresis. See the header
 * comment in m74_9_vr_model.h for the model and the calibration contract.
 */

#include "pch.h"
#include "m74_9_vr_model.h"

// PV quantization thresholds (datasheet Table 42, typical values, mV):
// the peak detector's 5 levels are bounded by PV1..PV4.
static constexpr float vrPvThresholdsMv[4] = { 930.0f, 1600.0f, 2300.0f, 3000.0f };

// Hysteresis per level (datasheet Table 43 typical currents x this board's
// 10k series resistor: 5/10/17/32/51 uA -> 50/100/173/322/510 mV).
static constexpr float vrHysteresisMv[5] = { 50.0f, 100.0f, 173.0f, 322.0f, 510.0f };

// Expected missing-teeth gap shift per level (pitch units). SEEDED with the
// coast rawtrg measurement (0.63 pitch, constant in angle across 373-1129
// rpm). The per-level refinement needs the car captures at 2000-7000 rpm:
// when the shift is observed to step with the level, split the entries.
static constexpr float vrExpectedShiftPitch[5] = { 0.63f, 0.63f, 0.63f, 0.63f, 0.63f };

// Calibration scalar: Vp = k * rpm (mV). 0 = model off.
static float vrAmplitudePerRpm = 0.0f;

// eat-risk band: |Vp - PV boundary| / boundary below this fraction = risky.
static constexpr float vrBoundaryRiskFraction = 0.15f;

float vrAmplitudePerRpmGet() {
	return vrAmplitudePerRpm;
}

int vrLevelForRpm(float rpm) {
	if (vrAmplitudePerRpm <= 0) {
		return 0;
	}

	float vp = vrAmplitudePerRpm * rpm;
	int level = 0;
	while (level < 4 && vp >= vrPvThresholdsMv[level]) {
		level++;
	}
	return level;
}

float vrHysteresisMvForLevel(int level) {
	if (level < 0 || level > 4) {
		return 0;
	}
	return vrHysteresisMv[level];
}

float vrExpectedShiftPitchForRpm(float rpm) {
	if (vrAmplitudePerRpm <= 0) {
		return -1.0f;	// model off
	}

	return vrExpectedShiftPitch[vrLevelForRpm(rpm)];
}

float vrBoundaryProximityForRpm(float rpm) {
	if (vrAmplitudePerRpm <= 0) {
		return 0.0f;
	}

	float vp = vrAmplitudePerRpm * rpm;
	float closest = 1.0f;	// 0..1, smaller = closer to a boundary

	for (float threshold : vrPvThresholdsMv) {
		float rel = absF(vp - threshold) / threshold;
		if (rel < closest) {
			closest = rel;
		}
	}

	// 1 = exactly on a boundary, 0 = at/outside the risk band edge
	return closest < vrBoundaryRiskFraction ? (1.0f - closest / vrBoundaryRiskFraction) : 0.0f;
}

void m74_9VrModel() {
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	float k = vrAmplitudePerRpm;
	bool on = k > 0;

	if (!on) {
		efiPrintf("vrmodel: OFF (k=0). Calibrate with 'vrk <mV per rpm>' - one bench VR-peak measurement at a known rpm");
		return;
	}

	int level = vrLevelForRpm(rpm);
	float vp = k * rpm;
	float expectedPitch = vrExpectedShiftPitchForRpm(rpm);
	float proximity = vrBoundaryProximityForRpm(rpm);
	float measuredDeg = getTriggerCentral()->gapAnchorCorrectionDeg;

	efiPrintf("vrmodel: k=%.3f mV/rpm rpm=%.0f Vp=%.0f mV level=%d HI=%.0f mV expected=%.3f pitch (%.2f deg) measured=%.2f deg boundary=%.0f%% risk=%s",
		k, rpm, vp, level + 1, vrHysteresisMv[level],
		expectedPitch, expectedPitch * 6.0f,
		measuredDeg,
		proximity * 100.0f,
		proximity > 0.5f ? "YES" : "no");
}

void m74_9VrModelSetK(const char* arg) {
	if (arg == nullptr || arg[0] == 0) {
		efiPrintf("vrk: usage 'vrk <mV per rpm>' (0 = model off), current k=%.3f", vrAmplitudePerRpm);
		return;
	}

	float k = atof(arg);
	if (k < 0) {
		efiPrintf("vrk: negative k rejected");
		return;
	}

	vrAmplitudePerRpm = k;
	m74_9VrModel();
}
