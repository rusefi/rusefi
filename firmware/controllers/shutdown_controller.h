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

	/**
	 * Engine-stop window: returns true while we are within the post-stop "cool-down" period.
	 *
	 * After a stop is requested (via start button, Lua script, console, TS command or a board hook -
	 * see StopRequestedReason) we stay in "engine stop" mode for `engineShutDownPeriod` seconds.
	 *
	 * While in this window:
	 *  - LimpManager cuts fuel (see LimpManager::updateState / ClearReason::StopRequested)
	 *  - RpmCalculator::checkIfSpinning() reports the engine as not spinning, which in turn lets
	 *    the rest of the code treat trigger events as if the engine is fully stopped and reset
	 *    trigger-related state (e.g. noise filter accumulators, sync state) cleanly.
	 *
	 * The timer is (re)started by ShutdownController::stopEngine().
	 */
	bool isEngineStop(efitick_t nowNt) const {
		float timeSinceStopRequested = m_engineStopTimer.getElapsedSeconds(nowNt);

		// If there was a stop request within the last `engineShutDownPeriod` seconds, we're still
		// in the engine-stop window.
		return timeSinceStopRequested < engineConfiguration->engineShutDownPeriod;
	}

private:
	Timer m_engineStopTimer;
};
