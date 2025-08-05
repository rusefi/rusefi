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
	bool luaLaunchState : 1 {};
	/**
	offset 4 bit 13 */
	bool unusedBit_14_13 : 1 {};
	/**
	offset 4 bit 14 */
	bool unusedBit_14_14 : 1 {};
	/**
	offset 4 bit 15 */
	bool unusedBit_14_15 : 1 {};
	/**
	offset 4 bit 16 */
	bool unusedBit_14_16 : 1 {};
	/**
	offset 4 bit 17 */
	bool unusedBit_14_17 : 1 {};
	/**
	offset 4 bit 18 */
	bool unusedBit_14_18 : 1 {};
	/**
	offset 4 bit 19 */
	bool unusedBit_14_19 : 1 {};
	/**
	offset 4 bit 20 */
	bool unusedBit_14_20 : 1 {};
	/**
	offset 4 bit 21 */
	bool unusedBit_14_21 : 1 {};
	/**
	offset 4 bit 22 */
	bool unusedBit_14_22 : 1 {};
	/**
	offset 4 bit 23 */
	bool unusedBit_14_23 : 1 {};
	/**
	offset 4 bit 24 */
	bool unusedBit_14_24 : 1 {};
	/**
	offset 4 bit 25 */
	bool unusedBit_14_25 : 1 {};
	/**
	offset 4 bit 26 */
	bool unusedBit_14_26 : 1 {};
	/**
	offset 4 bit 27 */
	bool unusedBit_14_27 : 1 {};
	/**
	offset 4 bit 28 */
	bool unusedBit_14_28 : 1 {};
	/**
	offset 4 bit 29 */
	bool unusedBit_14_29 : 1 {};
	/**
	offset 4 bit 30 */
	bool unusedBit_14_30 : 1 {};
	/**
	offset 4 bit 31 */
	bool unusedBit_14_31 : 1 {};
};
static_assert(sizeof(launch_control_state_s) == 8);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/launch_control_state.txt
