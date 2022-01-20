// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/launch_control_state.txt Wed Jan 05 06:47:26 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of launch_control_state_s
struct launch_control_state_s {
	/**
	 * offset 0
	 */
	int retardThresholdRpm = (int)0;
	/**
	offset 4 bit 0 */
	bool combinedConditions : 1 {};
	/**
	offset 4 bit 1 */
	bool launchActivatePinState : 1 {};
	/**
	offset 4 bit 2 */
	bool isLaunchCondition : 1 {};
	/**
	offset 4 bit 3 */
	bool activateSwitchCondition : 1 {};
	/**
	offset 4 bit 4 */
	bool rpmCondition : 1 {};
	/**
	offset 4 bit 5 */
	bool speedCondition : 1 {};
	/**
	offset 4 bit 6 */
	bool tpsCondition : 1 {};
	/**
	offset 4 bit 7 */
	bool unusedBit_8_7 : 1 {};
	/**
	offset 4 bit 8 */
	bool unusedBit_8_8 : 1 {};
	/**
	offset 4 bit 9 */
	bool unusedBit_8_9 : 1 {};
	/**
	offset 4 bit 10 */
	bool unusedBit_8_10 : 1 {};
	/**
	offset 4 bit 11 */
	bool unusedBit_8_11 : 1 {};
	/**
	offset 4 bit 12 */
	bool unusedBit_8_12 : 1 {};
	/**
	offset 4 bit 13 */
	bool unusedBit_8_13 : 1 {};
	/**
	offset 4 bit 14 */
	bool unusedBit_8_14 : 1 {};
	/**
	offset 4 bit 15 */
	bool unusedBit_8_15 : 1 {};
	/**
	offset 4 bit 16 */
	bool unusedBit_8_16 : 1 {};
	/**
	offset 4 bit 17 */
	bool unusedBit_8_17 : 1 {};
	/**
	offset 4 bit 18 */
	bool unusedBit_8_18 : 1 {};
	/**
	offset 4 bit 19 */
	bool unusedBit_8_19 : 1 {};
	/**
	offset 4 bit 20 */
	bool unusedBit_8_20 : 1 {};
	/**
	offset 4 bit 21 */
	bool unusedBit_8_21 : 1 {};
	/**
	offset 4 bit 22 */
	bool unusedBit_8_22 : 1 {};
	/**
	offset 4 bit 23 */
	bool unusedBit_8_23 : 1 {};
	/**
	offset 4 bit 24 */
	bool unusedBit_8_24 : 1 {};
	/**
	offset 4 bit 25 */
	bool unusedBit_8_25 : 1 {};
	/**
	offset 4 bit 26 */
	bool unusedBit_8_26 : 1 {};
	/**
	offset 4 bit 27 */
	bool unusedBit_8_27 : 1 {};
	/**
	offset 4 bit 28 */
	bool unusedBit_8_28 : 1 {};
	/**
	offset 4 bit 29 */
	bool unusedBit_8_29 : 1 {};
	/**
	offset 4 bit 30 */
	bool unusedBit_8_30 : 1 {};
	/**
	offset 4 bit 31 */
	bool unusedBit_8_31 : 1 {};
	/** total size 8*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/launch_control_state.txt Wed Jan 05 06:47:26 UTC 2022
