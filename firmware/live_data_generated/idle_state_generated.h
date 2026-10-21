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
	 * "base value
	 * current position without adjustments (iacByTpsTaper, afterCrankingIACtaperDuration)"
	 * offset 4
	 */
	percent_t baseIdlePosition = (percent_t)0;
	/**
	 * iacByTpsTaper portion
	 * offset 8
	 */
	percent_t iacByTpsTaper = (percent_t)0;
	/**
	 * mightResetPid
	 * The idea of 'mightResetPid' is to reset PID only once - each time when TPS > idlePidDeactivationTpsThreshold.
	 * The throttle pedal can be pressed for a long time, making the PID data obsolete (thus the reset is required).
	 * We set 'mightResetPid' to true only if PID was actually used (i.e. idlePid.getOutput() was called) to save some CPU resources.
	 * See automaticIdleController().
	offset 12 bit 0 */
	bool mightResetPid : 1 {};
	/**
	 * shouldResetPid
	 * This is used when the PID configuration is changed, to guarantee the reset
	offset 12 bit 1 */
	bool shouldResetPid : 1 {};
	/**
	 * wasResetPid
	 * This is needed to slowly turn on the PID back after it was reset.
	offset 12 bit 2 */
	bool wasResetPid : 1 {};
	/**
	 * cranking
	offset 12 bit 3 */
	bool isCranking : 1 {};
	/**
	offset 12 bit 4 */
	bool isIacTableForCoasting : 1 {};
	/**
	 * reset
	offset 12 bit 5 */
	bool needReset : 1 {};
	/**
	 * dead zone
	offset 12 bit 6 */
	bool isInDeadZone : 1 {};
	/**
	offset 12 bit 7 */
	bool isBlipping : 1 {};
	/**
	offset 12 bit 8 */
	bool badTps : 1 {};
	/**
	offset 12 bit 9 */
	bool looksLikeRunning : 1 {};
	/**
	offset 12 bit 10 */
	bool looksLikeCoasting : 1 {};
	/**
	offset 12 bit 11 */
	bool looksLikeCrankToIdle : 1 {};
	/**
	 * coasting
	offset 12 bit 12 */
	bool isIdleCoasting : 1 {};
	/**
	 * Closed loop active
	offset 12 bit 13 */
	bool isIdleClosedLoop : 1 {};
	/**
	 * idling
	offset 12 bit 14 */
	bool isIdling : 1 {};
	/**
	offset 12 bit 15 */
	bool unusedBit_18_15 : 1 {};
	/**
	offset 12 bit 16 */
	bool unusedBit_18_16 : 1 {};
	/**
	offset 12 bit 17 */
	bool unusedBit_18_17 : 1 {};
	/**
	offset 12 bit 18 */
	bool unusedBit_18_18 : 1 {};
	/**
	offset 12 bit 19 */
	bool unusedBit_18_19 : 1 {};
	/**
	offset 12 bit 20 */
	bool unusedBit_18_20 : 1 {};
	/**
	offset 12 bit 21 */
	bool unusedBit_18_21 : 1 {};
	/**
	offset 12 bit 22 */
	bool unusedBit_18_22 : 1 {};
	/**
	offset 12 bit 23 */
	bool unusedBit_18_23 : 1 {};
	/**
	offset 12 bit 24 */
	bool unusedBit_18_24 : 1 {};
	/**
	offset 12 bit 25 */
	bool unusedBit_18_25 : 1 {};
	/**
	offset 12 bit 26 */
	bool unusedBit_18_26 : 1 {};
	/**
	offset 12 bit 27 */
	bool unusedBit_18_27 : 1 {};
	/**
	offset 12 bit 28 */
	bool unusedBit_18_28 : 1 {};
	/**
	offset 12 bit 29 */
	bool unusedBit_18_29 : 1 {};
	/**
	offset 12 bit 30 */
	bool unusedBit_18_30 : 1 {};
	/**
	offset 12 bit 31 */
	bool unusedBit_18_31 : 1 {};
	/**
	 * Target RPM
	 * offset 16
	 */
	uint16_t idleTarget = (uint16_t)0;
	/**
	 * Entry threshold
	 * offset 18
	 */
	uint16_t idleEntryRpm = (uint16_t)0;
	/**
	 * Exit threshold
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
	 * Target RPM base
	 * offset 24
	 */
	int targetRpmByClt = (int)0;
	/**
	 * Target A/C RPM
	 * offset 28
	 */
	int targetRpmAc = (int)0;
	/**
	 * iacByRpmTaper portion
	 * offset 32
	 */
	percent_t iacByRpmTaper = (percent_t)0;
	/**
	 * Lua Adder
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
	 * Position
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
