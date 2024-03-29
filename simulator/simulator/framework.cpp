/**
 * @file	framework.cpp
 *
 * @date Sep 25, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "mpu_util.h"

// Since all the time logic in the firmware is centered around this function, we only provide this
// function in the firmware.  It forces us to exercise the functions that build on this one.
uint32_t getTimeNowLowerNt(void) {
	return US2NT(chVTGetSystemTimeX() * (1000000 / CH_CFG_ST_FREQUENCY));
}

#if HAL_USE_CAN
CANDriver* detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx);
#endif // HAL_USE_CAN

bool allowFlashWhileRunning() { return true; }
//void causeHardFault() { }


Reset_Cause_t getMCUResetCause() {
	return Reset_Cause_Unknown;
}

const char *getMCUResetCause(Reset_Cause_t) {
	return "Unknown";
}


// we use 'int' for compatibility with addConsoleActionI()
// can be called multiple times to change the timeout
void startWatchdog(int) { }

// Can be called for debug reasons to test the watchdog
void setWatchdogResetPeriod(int resetMs) { }

// A reset is done only if enough time has passed since the last reset.
void tryResetWatchdog() { }
