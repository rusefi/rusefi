// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/idle_state.txt Sun Oct 17 03:59:16 UTC 2021
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
	 * ETB position adjustment related to idle RPM control
	 * offset 16
	 */
	percent_t etbIdleAddition = (percent_t)0;
	/**
	 * The idea of 'mightResetPid' is to reset PID only once - each time when TPS > idlePidDeactivationTpsThreshold.
	 * The throttle pedal can be pressed for a long time, making the PID data obsolete (thus the reset is required).
	 * We set 'mightResetPid' to true only if PID was actually used (i.e. idlePid.getOutput() was called) to save some CPU resources.
	 * See automaticIdleController().
	offset 20 bit 0 */
	bool mightResetPid : 1;
	/**
	offset 20 bit 1 */
	bool shouldResetPid : 1;
	/**
	 * This is needed to slowly turn on the PID back after it was reset.
	offset 20 bit 2 */
	bool wasResetPid : 1;
	/**
	 * This is used when the PID configuration is changed, to guarantee the reset
	offset 20 bit 3 */
	bool mustResetPid : 1;
	/**
	offset 20 bit 4 */
	bool unusedBit_9_4 : 1;
	/**
	offset 20 bit 5 */
	bool unusedBit_9_5 : 1;
	/**
	offset 20 bit 6 */
	bool unusedBit_9_6 : 1;
	/**
	offset 20 bit 7 */
	bool unusedBit_9_7 : 1;
	/**
	offset 20 bit 8 */
	bool unusedBit_9_8 : 1;
	/**
	offset 20 bit 9 */
	bool unusedBit_9_9 : 1;
	/**
	offset 20 bit 10 */
	bool unusedBit_9_10 : 1;
	/**
	offset 20 bit 11 */
	bool unusedBit_9_11 : 1;
	/**
	offset 20 bit 12 */
	bool unusedBit_9_12 : 1;
	/**
	offset 20 bit 13 */
	bool unusedBit_9_13 : 1;
	/**
	offset 20 bit 14 */
	bool unusedBit_9_14 : 1;
	/**
	offset 20 bit 15 */
	bool unusedBit_9_15 : 1;
	/**
	offset 20 bit 16 */
	bool unusedBit_9_16 : 1;
	/**
	offset 20 bit 17 */
	bool unusedBit_9_17 : 1;
	/**
	offset 20 bit 18 */
	bool unusedBit_9_18 : 1;
	/**
	offset 20 bit 19 */
	bool unusedBit_9_19 : 1;
	/**
	offset 20 bit 20 */
	bool unusedBit_9_20 : 1;
	/**
	offset 20 bit 21 */
	bool unusedBit_9_21 : 1;
	/**
	offset 20 bit 22 */
	bool unusedBit_9_22 : 1;
	/**
	offset 20 bit 23 */
	bool unusedBit_9_23 : 1;
	/**
	offset 20 bit 24 */
	bool unusedBit_9_24 : 1;
	/**
	offset 20 bit 25 */
	bool unusedBit_9_25 : 1;
	/**
	offset 20 bit 26 */
	bool unusedBit_9_26 : 1;
	/**
	offset 20 bit 27 */
	bool unusedBit_9_27 : 1;
	/**
	offset 20 bit 28 */
	bool unusedBit_9_28 : 1;
	/**
	offset 20 bit 29 */
	bool unusedBit_9_29 : 1;
	/**
	offset 20 bit 30 */
	bool unusedBit_9_30 : 1;
	/**
	offset 20 bit 31 */
	bool unusedBit_9_31 : 1;
	/** total size 24*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/idle_state.txt Sun Oct 17 03:59:16 UTC 2021
