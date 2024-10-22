/*
 * @file knock_logic.h
 *
 * @date Apr 04, 2021
 * @author Andrey Gusakov
 */

#pragma once

#include "peak_detect.h"
#include "knock_controller_generated.h"

int getCylinderKnockBank(uint8_t cylinderNumber);

#define bore2frequency(bore)		(900 / (CONST_PI * (bore) / 2))

class KnockControllerBase : public EngineModule, public knock_controller_s {
public:
    KnockControllerBase() {
	    // start with threshold higher than any possible knock to avoid recording spurious knocks
	    m_knockThreshold = 100;
    }
	// EngineModule implementation
	void onFastCallback() override;

	// onKnockSenseCompleted is the callback from the knock sense driver to report a sensed knock level
	void onKnockSenseCompleted(uint8_t cylinderNumber, float dbv, efitick_t lastKnockTime);

	float getKnockRetard() const;
	uint32_t getKnockCount() const;

	virtual float getKnockThreshold() const = 0;
	virtual float getMaximumRetard() const = 0;

private:
	using PD = PeakDetect<float, MS2NT(50)>;
	PD peakDetectors[12];
	PD allCylinderPeakDetector;
};

class KnockController : public KnockControllerBase {
public:
	float getKnockThreshold() const override;
	float getMaximumRetard() const override;
};
