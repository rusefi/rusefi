#pragma once

#if HAL_USE_CAN
CANDriver* getCanDevice(size_t index);
inline bool isValidCanTxPin(brain_pin_e) { return true; }
inline bool isValidCanRxPin(brain_pin_e) { return true; }
inline void canHwInfo(CANDriver*) { return; }
#endif // HAL_USE_CAN

 bool mcuCanFlashWhileRunning() ;
 void causeHardFault() ;


// Reset Cause
typedef enum {
	Reset_Cause_Unknown = 0,
	Reset_Cause_IWatchdog,		// Independent hardware watchdog (we use it)
	Reset_Cause_WWatchdog,		// Window watchdog
	Reset_Cause_Soft_Reset,		// NVIC_SystemReset or by debugger
	Reset_Cause_NRST_Pin,		// Reset from NRST pin
	Reset_Cause_Illegal_Mode,	// Reset after illegal Stop, Standby or Shutdown mode entry
	Reset_Cause_BOR,			// BOR reset
	Reset_Cause_Firewall,		// Firewall reset
	Reset_Cause_Option_Byte,	// Option byte load reset
} Reset_Cause_t;

 Reset_Cause_t getMCUResetCause() ;

 const char *getMCUResetCause(Reset_Cause_t);

#include "sim_watchdog.h"
