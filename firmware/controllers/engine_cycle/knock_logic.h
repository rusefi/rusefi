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

class KnockController : public EngineModule, public knock_controller_s {
public:
    KnockController() {
	    // start with threshold higher than any possible knock to avoid recording spurious knocks
	    m_knockThreshold = 100;
    }
	// EngineModule implementation
	void onFastCallback() override;

	// onKnockSenseCompleted is the callback from the knock sense driver to report a sensed knock level
	bool onKnockSenseCompleted(uint8_t cylinderNumber, float dbv, efitick_t lastKnockTime);

	float getKnockRetard() const;
	uint32_t getKnockCount() const;

	virtual float getKnockThreshold() const;
	virtual float getMaximumRetard() const = 0;

private:
	using PD = PeakDetect<float, MS2NT(100)>;
	PD peakDetectors[12];
	PD allCylinderPeakDetector;
};

class KnockControllerImpl : public KnockController {
public:
	KnockControllerImpl()
	{
	}

	void onConfigurationChange(engine_configuration_s const * /*previousConfig*/) override;

	float getMaximumRetard() const override;

private:
	Map3D<6, 6, uint8_t, uint8_t, uint8_t> m_maxRetardTable;
};
