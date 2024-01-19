#pragma once
#include "rusefi_types.h"
struct idle_state_s {
	// offset 0
	idle_state_e idleState;
	// Idle: Position
	// %
	// offset 4
	float currentIdlePosition = (float)0;
	// Idle: Open loop
	// offset 8
	float baseIdlePosition = (float)0;
	// Idle: Closed loop
	// offset 12
	float idleClosedLoop = (float)0;
	// Idle: iacByTpsTaper portion
	// offset 16
	float iacByTpsTaper = (float)0;
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
	// Idle: coasting
	// offset 24 bit 13
	bool isIdleCoasting : 1 {};
	// Idle: Closed loop active
	// offset 24 bit 14
	bool isIdleClosedLoop : 1 {};
	// offset 24 bit 15
	bool unusedBit_24_15 : 1 {};
	// offset 24 bit 16
	bool unusedBit_24_16 : 1 {};
	// offset 24 bit 17
	bool unusedBit_24_17 : 1 {};
	// offset 24 bit 18
	bool unusedBit_24_18 : 1 {};
	// offset 24 bit 19
	bool unusedBit_24_19 : 1 {};
	// offset 24 bit 20
	bool unusedBit_24_20 : 1 {};
	// offset 24 bit 21
	bool unusedBit_24_21 : 1 {};
	// offset 24 bit 22
	bool unusedBit_24_22 : 1 {};
	// offset 24 bit 23
	bool unusedBit_24_23 : 1 {};
	// offset 24 bit 24
	bool unusedBit_24_24 : 1 {};
	// offset 24 bit 25
	bool unusedBit_24_25 : 1 {};
	// offset 24 bit 26
	bool unusedBit_24_26 : 1 {};
	// offset 24 bit 27
	bool unusedBit_24_27 : 1 {};
	// offset 24 bit 28
	bool unusedBit_24_28 : 1 {};
	// offset 24 bit 29
	bool unusedBit_24_29 : 1 {};
	// offset 24 bit 30
	bool unusedBit_24_30 : 1 {};
	// offset 24 bit 31
	bool unusedBit_24_31 : 1 {};
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
	float iacByRpmTaper = (float)0;
	// Idle: Lua Adder
	// offset 44
	float luaAdd = (float)0;
};
static_assert(sizeof(idle_state_s) == 48);
static_assert(offsetof(idle_state_s, currentIdlePosition) == 4);
static_assert(offsetof(idle_state_s, baseIdlePosition) == 8);
static_assert(offsetof(idle_state_s, idleClosedLoop) == 12);
static_assert(offsetof(idle_state_s, iacByTpsTaper) == 16);
static_assert(offsetof(idle_state_s, throttlePedalUpState) == 20);
static_assert(offsetof(idle_state_s, idleTarget) == 28);
static_assert(offsetof(idle_state_s, targetRpmByClt) == 32);
static_assert(offsetof(idle_state_s, targetRpmAcBump) == 36);
static_assert(offsetof(idle_state_s, iacByRpmTaper) == 40);
static_assert(offsetof(idle_state_s, luaAdd) == 44);

