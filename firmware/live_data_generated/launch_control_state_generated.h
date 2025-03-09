// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/launch_control_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of launch_control_state_s
struct launch_control_state_s {
	/**
	 * Launch: Retard threshold RPM
	 * offset 0
	 */
	int retardThresholdRpm = (int)0;
	/**
	offset 4 bit 0 */
	bool launchActivatePinState : 1 {};
	/**
	offset 4 bit 1 */
	bool isPreLaunchCondition : 1 {};
	/**
	 * Launch: isLaunchCondition
	offset 4 bit 2 */
	bool isLaunchCondition : 1 {};
	/**
	 * Launch: isSwitchActivated
	offset 4 bit 3 */
	bool isSwitchActivated : 1 {};
	/**
	 * Launch: isClutchActivated
	offset 4 bit 4 */
	bool isClutchActivated : 1 {};
	/**
	offset 4 bit 5 */
	bool isBrakePedalActivated : 1 {};
	/**
	 * Launch: isValidInputPin
	offset 4 bit 6 */
	bool isValidInputPin : 1 {};
	/**
	 * Launch: activateSwitchCondition
	offset 4 bit 7 */
	bool activateSwitchCondition : 1 {};
	/**
	offset 4 bit 8 */
	bool rpmLaunchCondition : 1 {};
	/**
	offset 4 bit 9 */
	bool rpmPreLaunchCondition : 1 {};
	/**
	 * Launch: speedCondition
	offset 4 bit 10 */
	bool speedCondition : 1 {};
	/**
	 * Launch: tpsCondition
	offset 4 bit 11 */
	bool tpsCondition : 1 {};
	/**
	offset 4 bit 12 */
	bool unusedBit_13_12 : 1 {};
	/**
	offset 4 bit 13 */
	bool unusedBit_13_13 : 1 {};
	/**
	offset 4 bit 14 */
	bool unusedBit_13_14 : 1 {};
	/**
	offset 4 bit 15 */
	bool unusedBit_13_15 : 1 {};
	/**
	offset 4 bit 16 */
	bool unusedBit_13_16 : 1 {};
	/**
	offset 4 bit 17 */
	bool unusedBit_13_17 : 1 {};
	/**
	offset 4 bit 18 */
	bool unusedBit_13_18 : 1 {};
	/**
	offset 4 bit 19 */
	bool unusedBit_13_19 : 1 {};
	/**
	offset 4 bit 20 */
	bool unusedBit_13_20 : 1 {};
	/**
	offset 4 bit 21 */
	bool unusedBit_13_21 : 1 {};
	/**
	offset 4 bit 22 */
	bool unusedBit_13_22 : 1 {};
	/**
	offset 4 bit 23 */
	bool unusedBit_13_23 : 1 {};
	/**
	offset 4 bit 24 */
	bool unusedBit_13_24 : 1 {};
	/**
	offset 4 bit 25 */
	bool unusedBit_13_25 : 1 {};
	/**
	offset 4 bit 26 */
	bool unusedBit_13_26 : 1 {};
	/**
	offset 4 bit 27 */
	bool unusedBit_13_27 : 1 {};
	/**
	offset 4 bit 28 */
	bool unusedBit_13_28 : 1 {};
	/**
	offset 4 bit 29 */
	bool unusedBit_13_29 : 1 {};
	/**
	offset 4 bit 30 */
	bool unusedBit_13_30 : 1 {};
	/**
	offset 4 bit 31 */
	bool unusedBit_13_31 : 1 {};
};
static_assert(sizeof(launch_control_state_s) == 8);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/launch_control_state.txt
