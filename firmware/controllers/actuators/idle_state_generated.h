// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/idle_state.txt Mon Jan 10 21:12:38 EST 2022
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
	 * that's current position with CLT and IAT corrections
	 * offset 4
	 */
	percent_t currentIdlePosition = (percent_t)0;
	/**
	 * current position without adjustments (iacByTpsTaper, afterCrankingIACtaperDuration)
	 * offset 8
	 */
	percent_t baseIdlePosition = (percent_t)0;
	/**
	 * true in IDLE throttle pedal state, false if driver is touching the pedal
	 * todo: better name for this field?
	 * offset 12
	 */
	int throttlePedalUpState = (int)0;
	/**
	 * The idea of 'mightResetPid' is to reset PID only once - each time when TPS > idlePidDeactivationTpsThreshold.
	 * The throttle pedal can be pressed for a long time, making the PID data obsolete (thus the reset is required).
	 * We set 'mightResetPid' to true only if PID was actually used (i.e. idlePid.getOutput() was called) to save some CPU resources.
	 * See automaticIdleController().
	offset 16 bit 0 */
	bool mightResetPid : 1 {};
	/**
	offset 16 bit 1 */
	bool shouldResetPid : 1 {};
	/**
	 * This is needed to slowly turn on the PID back after it was reset.
	offset 16 bit 2 */
	bool wasResetPid : 1 {};
	/**
	 * This is used when the PID configuration is changed, to guarantee the reset
	offset 16 bit 3 */
	bool mustResetPid : 1 {};
	/**
	offset 16 bit 4 */
	bool isCoasting : 1 {};
	/**
	offset 16 bit 5 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 16 bit 6 */
	bool notIdling : 1 {};
	/**
	offset 16 bit 7 */
	bool needReset : 1 {};
	/**
	offset 16 bit 8 */
	bool isInDeadZone : 1 {};
	/**
	offset 16 bit 9 */
	bool isBlipping : 1 {};
	/**
	offset 16 bit 10 */
	bool useClosedLoop : 1 {};
	/**
	offset 16 bit 11 */
	bool badTps : 1 {};
	/**
	offset 16 bit 12 */
	bool looksLikeRunning : 1 {};
	/**
	offset 16 bit 13 */
	bool looksLikeCoasting : 1 {};
	/**
	offset 16 bit 14 */
	bool looksLikeCrankToIdle : 1 {};
	/**
	offset 16 bit 15 */
	bool unusedBit_19_15 : 1 {};
	/**
	offset 16 bit 16 */
	bool unusedBit_19_16 : 1 {};
	/**
	offset 16 bit 17 */
	bool unusedBit_19_17 : 1 {};
	/**
	offset 16 bit 18 */
	bool unusedBit_19_18 : 1 {};
	/**
	offset 16 bit 19 */
	bool unusedBit_19_19 : 1 {};
	/**
	offset 16 bit 20 */
	bool unusedBit_19_20 : 1 {};
	/**
	offset 16 bit 21 */
	bool unusedBit_19_21 : 1 {};
	/**
	offset 16 bit 22 */
	bool unusedBit_19_22 : 1 {};
	/**
	offset 16 bit 23 */
	bool unusedBit_19_23 : 1 {};
	/**
	offset 16 bit 24 */
	bool unusedBit_19_24 : 1 {};
	/**
	offset 16 bit 25 */
	bool unusedBit_19_25 : 1 {};
	/**
	offset 16 bit 26 */
	bool unusedBit_19_26 : 1 {};
	/**
	offset 16 bit 27 */
	bool unusedBit_19_27 : 1 {};
	/**
	offset 16 bit 28 */
	bool unusedBit_19_28 : 1 {};
	/**
	offset 16 bit 29 */
	bool unusedBit_19_29 : 1 {};
	/**
	offset 16 bit 30 */
	bool unusedBit_19_30 : 1 {};
	/**
	offset 16 bit 31 */
	bool unusedBit_19_31 : 1 {};
	/**
	 * offset 20
	 */
	int targetRpmByClt = (int)0;
	/**
	 * offset 24
	 */
	int targetRpmAcBump = (int)0;
	/** total size 28*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/idle_state.txt Mon Jan 10 21:12:38 EST 2022
