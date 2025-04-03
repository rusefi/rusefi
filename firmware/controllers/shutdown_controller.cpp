/*
 * @file shutdown_controller.cpp
 *
 */

#include "pch.h"

void doScheduleStopEngine(StopRequestedReason reason) {
#if EFI_SHAFT_POSITION_INPUT
	efiPrintf("Let's stop this engine!");
	getLimpManager()->shutdownController.stopEngine(reason);
#endif // EFI_SHAFT_POSITION_INPUT
	// todo: initiate stepper motor parking
	// make sure we have stored all the info
#if EFI_PROD_CODE
	//todo: FIX kinetis build with this line
	//backupRamFlush();
#endif // EFI_PROD_CODE
}

	void ShutdownController::stopEngine(StopRequestedReason reason) {
		m_engineStopTimer.reset();
    engine->outputChannels.stopEngineCode = (uint8_t)reason;
	}
