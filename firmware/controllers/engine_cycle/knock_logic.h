/*
 * @file knock_logic.h
 *
 * @date Apr 04, 2021
 * @author Andrey Gusakov
 */

#pragma once

#include "peak_detect.h"

int getCylinderKnockBank(uint8_t cylinderIndex);

class KnockController : public EnginePtr {
public:
	// onKnockSenseCompleted is the callback from the knock sense driver to report a sensed knock level
	bool onKnockSenseCompleted(uint8_t cylinderIndex, float dbv, efitick_t lastKnockTime);
	void periodicFastCallback();

	float getKnockRetard() const;

private:
	// Degrees retarded: larger number = more retard
	float m_knockRetard = 0;

	using PD = PeakDetect<float, MS2NT(100)>;
	PD peakDetectors[12];
	PD allCylinderPeakDetector;


};
