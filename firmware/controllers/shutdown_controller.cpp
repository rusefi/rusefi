/*
 * @file shutdown_controller.cpp
 *
 */

#include "pch.h"

void doScheduleStopEngine() {
	efiPrintf("Starting doScheduleStopEngine");
	getLimpManager()->shutdownController.stopEngine();
	// todo: initiate stepper motor parking
	// make sure we have stored all the info
#if EFI_PROD_CODE
	//todo: FIX kinetis build with this line
	//backupRamFlush();
#endif // EFI_PROD_CODE
}
