/**
 * @file dfco.h
 */

#pragma once
#include "engine_module.h"
#include "timer.h"
#include "limp_manager.h"

// DFCO = deceleration fuel cut off, ie, save gas when your foot is off the pedal
class DfcoController : public EngineModule {
public:
	void update();

	bool cutFuel() const;

	float getTimeSinceCut() const;

private:
	bool getState() const;
	bool m_isDfco = false;

	mutable Hysteresis m_mapHysteresis;

	Timer m_timeSinceCut;
	Timer m_timeSinceNoCut;
};
