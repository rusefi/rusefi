// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/idle_state.txt
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
	 * "idle: base value
	 * current position without adjustments (iacByTpsTaper, afterCrankingIACtaperDuration)"
	 * offset 4
	 */
	percent_t baseIdlePosition = (percent_t)0;
	/**
	 * idle: iacByTpsTaper portion
	 * offset 8
	 */
	percent_t iacByTpsTaper = (percent_t)0;
	/**
	 * idle: mightResetPid
	 * The idea of 'mightResetPid' is to reset PID only once - each time when TPS > idlePidDeactivationTpsThreshold.
	 * The throttle pedal can be pressed for a long time, making the PID data obsolete (thus the reset is required).
	 * We set 'mightResetPid' to true only if PID was actually used (i.e. idlePid.getOutput() was called) to save some CPU resources.
	 * See automaticIdleController().
	offset 12 bit 0 */
	bool mightResetPid : 1 {};
	/**
	 * Idle: shouldResetPid
	offset 12 bit 1 */
	bool shouldResetPid : 1 {};
	/**
	 * Idle: wasResetPid
	 * This is needed to slowly turn on the PID back after it was reset.
	offset 12 bit 2 */
	bool wasResetPid : 1 {};
	/**
	 * Idle: mustResetPid
	 * This is used when the PID configuration is changed, to guarantee the reset
	offset 12 bit 3 */
	bool mustResetPid : 1 {};
	/**
	 * Idle: cranking
	offset 12 bit 4 */
	bool isCranking : 1 {};
	/**
	offset 12 bit 5 */
	bool isIacTableForCoasting : 1 {};
	/**
	 * Idle: reset
	offset 12 bit 6 */
	bool needReset : 1 {};
	/**
	 * Idle: dead zone
	offset 12 bit 7 */
	bool isInDeadZone : 1 {};
	/**
	offset 12 bit 8 */
	bool isBlipping : 1 {};
	/**
	offset 12 bit 9 */
	bool badTps : 1 {};
	/**
	offset 12 bit 10 */
	bool looksLikeRunning : 1 {};
	/**
	offset 12 bit 11 */
	bool looksLikeCoasting : 1 {};
	/**
	offset 12 bit 12 */
	bool looksLikeCrankToIdle : 1 {};
	/**
	 * Idle: coasting
	offset 12 bit 13 */
	bool isIdleCoasting : 1 {};
	/**
	 * Idle: Closed loop active
	offset 12 bit 14 */
	bool isIdleClosedLoop : 1 {};
	/**
	 * Idle: idling
	offset 12 bit 15 */
	bool isIdling : 1 {};
	/**
	offset 12 bit 16 */
	bool unusedBit_19_16 : 1 {};
	/**
	offset 12 bit 17 */
	bool unusedBit_19_17 : 1 {};
	/**
	offset 12 bit 18 */
	bool unusedBit_19_18 : 1 {};
	/**
	offset 12 bit 19 */
	bool unusedBit_19_19 : 1 {};
	/**
	offset 12 bit 20 */
	bool unusedBit_19_20 : 1 {};
	/**
	offset 12 bit 21 */
	bool unusedBit_19_21 : 1 {};
	/**
	offset 12 bit 22 */
	bool unusedBit_19_22 : 1 {};
	/**
	offset 12 bit 23 */
	bool unusedBit_19_23 : 1 {};
	/**
	offset 12 bit 24 */
	bool unusedBit_19_24 : 1 {};
	/**
	offset 12 bit 25 */
	bool unusedBit_19_25 : 1 {};
	/**
	offset 12 bit 26 */
	bool unusedBit_19_26 : 1 {};
	/**
	offset 12 bit 27 */
	bool unusedBit_19_27 : 1 {};
	/**
	offset 12 bit 28 */
	bool unusedBit_19_28 : 1 {};
	/**
	offset 12 bit 29 */
	bool unusedBit_19_29 : 1 {};
	/**
	offset 12 bit 30 */
	bool unusedBit_19_30 : 1 {};
	/**
	offset 12 bit 31 */
	bool unusedBit_19_31 : 1 {};
	/**
	 * Idle: Target RPM
	 * offset 16
	 */
	uint16_t idleTarget = (uint16_t)0;
	/**
	 * Idle: Entry threshold
	 * offset 18
	 */
	uint16_t idleEntryRpm = (uint16_t)0;
	/**
	 * Idle: Exit threshold
	 * offset 20
	 */
	uint16_t idleExitRpm = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 22
	 */
	uint8_t alignmentFill_at_22[2] = {};
	/**
	 * Idle: Target RPM base
	 * offset 24
	 */
	int targetRpmByClt = (int)0;
	/**
	 * Idle: Target A/C RPM
	 * offset 28
	 */
	int targetRpmAc = (int)0;
	/**
	 * idle: iacByRpmTaper portion
	 * offset 32
	 */
	percent_t iacByRpmTaper = (percent_t)0;
	/**
	 * idle: Lua Adder
	 * offset 36
	 */
	percent_t luaAdd = (percent_t)0;
	/**
	 * offset 40
	 */
	int m_lastTargetRpm = (int)0;
	/**
	 * Closed loop
	 * offset 44
	 */
	percent_t idleClosedLoop = (percent_t)0;
	/**
	 * @@GAUGE_NAME_IAC@@
	 * units: %
	 * offset 48
	 */
	percent_t currentIdlePosition = (percent_t)0;
	/**
	 * Target airmass
	 * units: mg
	 * offset 52
	 */
	uint16_t idleTargetAirmass = (uint16_t)0;
	/**
	 * Target airflow
	 * units: kg/h
	 * offset 54
	 */
	scaled_channel<uint16_t, 100, 1> idleTargetFlow = (uint16_t)0;
};
static_assert(sizeof(idle_state_s) == 56);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/idle_state.txt
