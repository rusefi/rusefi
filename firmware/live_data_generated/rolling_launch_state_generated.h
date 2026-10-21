// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/modules/rolling_launch/rolling_launch_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of rolling_launch_state_s
struct rolling_launch_state_s {
	/**
	 * armed
	offset 0 bit 0 */
	bool isRollingLaunchArmed : 1 {};
	/**
	 * button active
	offset 0 bit 1 */
	bool isRollingLaunchActive : 1 {};
	/**
	 * switch state
	offset 0 bit 2 */
	bool rollingLaunchSwitchState : 1 {};
	/**
	 * Lua trigger
	offset 0 bit 3 */
	bool luaRollingLaunchState : 1 {};
	/**
	 * ramp-out active
	offset 0 bit 4 */
	bool isRollingLaunchRampActive : 1 {};
	/**
	offset 0 bit 5 */
	bool unusedBit_5_5 : 1 {};
	/**
	offset 0 bit 6 */
	bool unusedBit_5_6 : 1 {};
	/**
	offset 0 bit 7 */
	bool unusedBit_5_7 : 1 {};
	/**
	offset 0 bit 8 */
	bool unusedBit_5_8 : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_5_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_5_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_5_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_5_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_5_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_5_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_5_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_5_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_5_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_5_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_5_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_5_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_5_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_5_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_5_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_5_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_5_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_5_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_5_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_5_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_5_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_5_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_5_31 : 1 {};
	/**
	 * captured RPM
	 * offset 4
	 */
	float capturedRpm = (float)0;
	/**
	 * hold target RPM
	 * offset 8
	 */
	float rollingLaunchTargetRpm = (float)0;
	/**
	 * spark skip ratio
	 * offset 12
	 */
	float rollingLaunchSparkSkipRatio = (float)0;
	/**
	 * timing retard
	 * offset 16
	 */
	float rollingLaunchTimingRetardOut = (float)0;
	/**
	 * ramp elapsed
	 * offset 20
	 */
	float rollingLaunchRampElapsed = (float)0;
};
static_assert(sizeof(rolling_launch_state_s) == 24);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/modules/rolling_launch/rolling_launch_state.txt
