#pragma once
#include "rusefi_types.h"
struct idle_state_s {
	// offset 0
	idle_state_e idleState = (idle_state_e)0;
	// @@GAUGE_NAME_IAC@@
	// %
	// offset 4
	percent_t currentIdlePosition = (percent_t)0;
	// Idle: Open loop
	// offset 8
	percent_t baseIdlePosition = (percent_t)0;
	// Idle: Closed loop
	// offset 12
	percent_t idleClosedLoop = (percent_t)0;
	// Idle: iacByTpsTaper portion
	// offset 16
	percent_t iacByTpsTaper = (percent_t)0;
	// Idle: throttlePedalUpState
	// true in IDLE throttle pedal state, false if driver is touching the pedal
	// todo: better name for this field?
	// offset 20
	int throttlePedalUpState = (int)0;
	// Idle: mightResetPid
	// The idea of 'mightResetPid' is to reset PID only once - each time when TPS > idlePidDeactivationTpsThreshold.
	// The throttle pedal can be pressed for a long time, making the PID data obsolete (thus the reset is required).
	// We set 'mightResetPid' to true only if PID was actually used (i.e. idlePid.getOutput() was called) to save some CPU resources.
	// See automaticIdleController().
	// offset 24 bit 0
	bool mightResetPid : 1 {};
	// Idle: shouldResetPid
	// offset 24 bit 1
	bool shouldResetPid : 1 {};
	// Idle: wasResetPid
	// This is needed to slowly turn on the PID back after it was reset.
	// offset 24 bit 2
	bool wasResetPid : 1 {};
	// Idle: mustResetPid
	// This is used when the PID configuration is changed, to guarantee the reset
	// offset 24 bit 3
	bool mustResetPid : 1 {};
	// Idle: cranking
	// offset 24 bit 4
	bool isCranking : 1 {};
	// offset 24 bit 5
	bool isIacTableForCoasting : 1 {};
	// offset 24 bit 6
	bool notIdling : 1 {};
	// Idle: reset
	// offset 24 bit 7
	bool needReset : 1 {};
	// Idle: dead zone
	// offset 24 bit 8
	bool isInDeadZone : 1 {};
	// offset 24 bit 9
	bool isBlipping : 1 {};
	// offset 24 bit 10
	bool looksLikeRunning : 1 {};
	// offset 24 bit 11
	bool looksLikeCoasting : 1 {};
	// offset 24 bit 12
	bool looksLikeCrankToIdle : 1 {};
	// offset 24 bit 13
	bool isVerboseIAC : 1 {};
	// Idle: coasting
	// offset 24 bit 14
	bool isIdleCoasting : 1 {};
	// Idle: Closed loop active
	// offset 24 bit 15
	bool isIdleClosedLoop : 1 {};
	// offset 24 bit 16
	bool unusedBit_22_16 : 1 {};
	// offset 24 bit 17
	bool unusedBit_22_17 : 1 {};
	// offset 24 bit 18
	bool unusedBit_22_18 : 1 {};
	// offset 24 bit 19
	bool unusedBit_22_19 : 1 {};
	// offset 24 bit 20
	bool unusedBit_22_20 : 1 {};
	// offset 24 bit 21
	bool unusedBit_22_21 : 1 {};
	// offset 24 bit 22
	bool unusedBit_22_22 : 1 {};
	// offset 24 bit 23
	bool unusedBit_22_23 : 1 {};
	// offset 24 bit 24
	bool unusedBit_22_24 : 1 {};
	// offset 24 bit 25
	bool unusedBit_22_25 : 1 {};
	// offset 24 bit 26
	bool unusedBit_22_26 : 1 {};
	// offset 24 bit 27
	bool unusedBit_22_27 : 1 {};
	// offset 24 bit 28
	bool unusedBit_22_28 : 1 {};
	// offset 24 bit 29
	bool unusedBit_22_29 : 1 {};
	// offset 24 bit 30
	bool unusedBit_22_30 : 1 {};
	// offset 24 bit 31
	bool unusedBit_22_31 : 1 {};
	// Idle: Target RPM
	// offset 28
	int idleTarget = (int)0;
	// Idle: Target RPM base
	// offset 32
	int targetRpmByClt = (int)0;
	// Idle: Target A/C bump
	// offset 36
	int targetRpmAcBump = (int)0;
	// Idle: iacByRpmTaper portion
	// offset 40
	percent_t iacByRpmTaper = (percent_t)0;
	// Idle: Lua Adder
	// offset 44
	percent_t luaAdd = (percent_t)0;
};
static_assert(sizeof(idle_state_s) == 48);

