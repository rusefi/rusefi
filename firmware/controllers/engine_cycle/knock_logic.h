/*
 * @file knock_logic.h
 *
 * @date Apr 04, 2021
 * @author Andrey Gusakov
 */

#pragma once

#include "peak_detect.h"

int getCylinderKnockBank(uint8_t cylinderNumber);

class KnockController : public EngineModule {
public:
	// EngineModule implementation
	void onFastCallback() override;

	// onKnockSenseCompleted is the callback from the knock sense driver to report a sensed knock level
	bool onKnockSenseCompleted(uint8_t cylinderNumber, float dbv, efitick_t lastKnockTime);

	float getKnockRetard() const;
	uint32_t getKnockCount() const;

	virtual float getKnockThreshold() const;

private:
	float m_knockThreshold = -1e5;

	// Degrees retarded: larger number = more retard
	float m_knockRetard = 0;

	uint32_t m_knockCount = 0;

	using PD = PeakDetect<float, MS2NT(100)>;
	PD peakDetectors[12];
	PD allCylinderPeakDetector;


};
