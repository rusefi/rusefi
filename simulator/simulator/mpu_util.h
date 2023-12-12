#pragma once

#if HAL_USE_CAN
CANDriver* detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx);
static bool isValidCanTxPin(brain_pin_e) { return true; }
static bool isValidCanRxPin(brain_pin_e) { return true; }
static void canHwInfo(CANDriver*) { return; }
#endif // HAL_USE_CAN

static bool allowFlashWhileRunning() { return true; }
static void causeHardFault() { }

static Reset_Cause_t getMCUResetCause() {
	return Reset_Cause_Unknown;
}

static const char *getMCUResetCause(Reset_Cause_t) {
	return "Unknown";
}

#include "sim_watchdog.h"
