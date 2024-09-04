// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/actuators/idle_state.txt Wed Jan 03 20:30:10 UTC 2024
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of idle_state_s
struct idle_state_s {
	/**
	 * offset 0
	 */
	idle_state_e idleState = (idle_state_e)0;
	/**
	 * @@GAUGE_NAME_IAC@@
	 * units: %
	 * offset 4
	 */
	percent_t currentIdlePosition = (percent_t)0;
	/**
	 * "idle: base value
	 * current position without adjustments (iacByTpsTaper, afterCrankingIACtaperDuration)"
	 * offset 8
	 */
	percent_t baseIdlePosition = (percent_t)0;
	/**
	 * Idle: Closed loop
	 * offset 12
	 */
	percent_t idleClosedLoop = (percent_t)0;
	/**
	 * idle: iacByTpsTaper portion
	 * offset 16
	 */
	percent_t iacByTpsTaper = (percent_t)0;
	/**
	 * idle: throttlePedalUpState
	 * true in IDLE throttle pedal state, false if driver is touching the pedal
	 * todo: better name for this field?
	 * offset 20
	 */
	int throttlePedalUpState = (int)0;
	/**
	 * idle: mightResetPid
	 * The idea of 'mightResetPid' is to reset PID only once - each time when TPS > idlePidDeactivationTpsThreshold.
	 * The throttle pedal can be pressed for a long time, making the PID data obsolete (thus the reset is required).
	 * We set 'mightResetPid' to true only if PID was actually used (i.e. idlePid.getOutput() was called) to save some CPU resources.
	 * See automaticIdleController().
	offset 24 bit 0 */
	bool mightResetPid : 1 {};
	/**
	 * Idle: shouldResetPid
	offset 24 bit 1 */
	bool shouldResetPid : 1 {};
	/**
	 * Idle: wasResetPid
	 * This is needed to slowly turn on the PID back after it was reset.
	offset 24 bit 2 */
	bool wasResetPid : 1 {};
	/**
	 * Idle: mustResetPid
	 * This is used when the PID configuration is changed, to guarantee the reset
	offset 24 bit 3 */
	bool mustResetPid : 1 {};
	/**
	 * Idle: cranking
	offset 24 bit 4 */
	bool isCranking : 1 {};
	/**
	offset 24 bit 5 */
	bool isIacTableForCoasting : 1 {};
	/**
	offset 24 bit 6 */
	bool notIdling : 1 {};
	/**
	 * Idle: reset
	offset 24 bit 7 */
	bool needReset : 1 {};
	/**
	 * Idle: dead zone
	offset 24 bit 8 */
	bool isInDeadZone : 1 {};
	/**
	offset 24 bit 9 */
	bool isBlipping : 1 {};
	/**
	offset 24 bit 10 */
	bool useClosedLoop : 1 {};
	/**
	offset 24 bit 11 */
	bool badTps : 1 {};
	/**
	offset 24 bit 12 */
	bool looksLikeRunning : 1 {};
	/**
	offset 24 bit 13 */
	bool looksLikeCoasting : 1 {};
	/**
	offset 24 bit 14 */
	bool looksLikeCrankToIdle : 1 {};
	/**
	 * Idle: coasting
	offset 24 bit 15 */
	bool isIdleCoasting : 1 {};
	/**
	 * Idle: Closed loop active
	offset 24 bit 16 */
	bool isIdleClosedLoop : 1 {};
	/**
	offset 24 bit 17 */
	bool unusedBit_23_17 : 1 {};
	/**
	offset 24 bit 18 */
	bool unusedBit_23_18 : 1 {};
	/**
	offset 24 bit 19 */
	bool unusedBit_23_19 : 1 {};
	/**
	offset 24 bit 20 */
	bool unusedBit_23_20 : 1 {};
	/**
	offset 24 bit 21 */
	bool unusedBit_23_21 : 1 {};
	/**
	offset 24 bit 22 */
	bool unusedBit_23_22 : 1 {};
	/**
	offset 24 bit 23 */
	bool unusedBit_23_23 : 1 {};
	/**
	offset 24 bit 24 */
	bool unusedBit_23_24 : 1 {};
	/**
	offset 24 bit 25 */
	bool unusedBit_23_25 : 1 {};
	/**
	offset 24 bit 26 */
	bool unusedBit_23_26 : 1 {};
	/**
	offset 24 bit 27 */
	bool unusedBit_23_27 : 1 {};
	/**
	offset 24 bit 28 */
	bool unusedBit_23_28 : 1 {};
	/**
	offset 24 bit 29 */
	bool unusedBit_23_29 : 1 {};
	/**
	offset 24 bit 30 */
	bool unusedBit_23_30 : 1 {};
	/**
	offset 24 bit 31 */
	bool unusedBit_23_31 : 1 {};
	/**
	 * Idle: Target RPM
	 * offset 28
	 */
	int idleTarget = (int)0;
	/**
	 * Idle: Target RPM base
	 * offset 32
	 */
	int targetRpmByClt = (int)0;
	/**
	 * Idle: Target A/C RPM
	 * offset 36
	 */
	int targetRpmAc = (int)0;
	/**
	 * idle: iacByRpmTaper portion
	 * offset 40
	 */
	percent_t iacByRpmTaper = (percent_t)0;
	/**
	 * idle: Lua Adder
	 * offset 44
	 */
	percent_t luaAdd = (percent_t)0;
};
static_assert(sizeof(idle_state_s) == 48);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/actuators/idle_state.txt Wed Jan 03 20:30:10 UTC 2024
