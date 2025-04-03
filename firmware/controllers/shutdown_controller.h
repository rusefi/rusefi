/*
 * @file shutdown_controller.h
 *
 */

#pragma once

#include <rusefi/timer.h>

enum class StopRequestedReason : uint8_t {
    None, // 0
    StartButton, // 1
    Lua, // 2
    Console, // 3
  	TsCommand, // 4

    Board1,
    Board2,
    Board3,

};

void doScheduleStopEngine(StopRequestedReason reason);

class ShutdownController {
public:
	void stopEngine(StopRequestedReason reason);

	bool isEngineStop(efitick_t nowNt) const {
		float timeSinceStopRequested = m_engineStopTimer.getElapsedSeconds(nowNt);

		// If there was stop requested in the past 5 seconds, we're in stop mode
		return timeSinceStopRequested < 5;
	}

private:
	Timer m_engineStopTimer;
};
