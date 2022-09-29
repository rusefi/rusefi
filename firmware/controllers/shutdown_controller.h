/*
 * @file shutdown_controller.h
 *
 */

#pragma once

#include "timer.h"

void doScheduleStopEngine();

class ShutdownController {
public:
	void stopEngine() {
		m_engineStopTimer.reset();
//		ignitionOnTimeNt = 0;
	}

	float getTimeSinceEngineStop(efitick_t nowNt) const {
		return m_engineStopTimer.getElapsedSeconds(nowNt);
	}

	bool isEngineStop(efitick_t nowNt) const {
		float timeSinceStop = getTimeSinceEngineStop(nowNt);

		// If there was stop requested in the past 5 seconds, we're in stop mode
		return timeSinceStop < 5;
	}

private:
	Timer m_engineStopTimer;

	/**
	 * this is needed by and checkShutdown()
	 */
	// is this an unused boolean value?
//	efitick_t ignitionOnTimeNt = 0;
};
