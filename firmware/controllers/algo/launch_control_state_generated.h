#pragma once
#include "rusefi_types.h"
struct launch_control_state_s {
	// offset 0
	int retardThresholdRpm = (int)0;
	// offset 4 bit 0
	bool combinedConditions : 1 {};
	// offset 4 bit 1
	bool launchActivatePinState : 1 {};
	// offset 4 bit 2
	bool isLaunchCondition : 1 {};
	// offset 4 bit 3
	bool isSwitchActivated : 1 {};
	// offset 4 bit 4
	bool isClutchActivated : 1 {};
	// offset 4 bit 5
	bool isValidInputPin : 1 {};
	// offset 4 bit 6
	bool activateSwitchCondition : 1 {};
	// offset 4 bit 7
	bool rpmCondition : 1 {};
	// offset 4 bit 8
	bool speedCondition : 1 {};
	// offset 4 bit 9
	bool tpsCondition : 1 {};
	// offset 4 bit 10
	bool unusedBit_11_10 : 1 {};
	// offset 4 bit 11
	bool unusedBit_11_11 : 1 {};
	// offset 4 bit 12
	bool unusedBit_11_12 : 1 {};
	// offset 4 bit 13
	bool unusedBit_11_13 : 1 {};
	// offset 4 bit 14
	bool unusedBit_11_14 : 1 {};
	// offset 4 bit 15
	bool unusedBit_11_15 : 1 {};
	// offset 4 bit 16
	bool unusedBit_11_16 : 1 {};
	// offset 4 bit 17
	bool unusedBit_11_17 : 1 {};
	// offset 4 bit 18
	bool unusedBit_11_18 : 1 {};
	// offset 4 bit 19
	bool unusedBit_11_19 : 1 {};
	// offset 4 bit 20
	bool unusedBit_11_20 : 1 {};
	// offset 4 bit 21
	bool unusedBit_11_21 : 1 {};
	// offset 4 bit 22
	bool unusedBit_11_22 : 1 {};
	// offset 4 bit 23
	bool unusedBit_11_23 : 1 {};
	// offset 4 bit 24
	bool unusedBit_11_24 : 1 {};
	// offset 4 bit 25
	bool unusedBit_11_25 : 1 {};
	// offset 4 bit 26
	bool unusedBit_11_26 : 1 {};
	// offset 4 bit 27
	bool unusedBit_11_27 : 1 {};
	// offset 4 bit 28
	bool unusedBit_11_28 : 1 {};
	// offset 4 bit 29
	bool unusedBit_11_29 : 1 {};
	// offset 4 bit 30
	bool unusedBit_11_30 : 1 {};
	// offset 4 bit 31
	bool unusedBit_11_31 : 1 {};
};
static_assert(sizeof(launch_control_state_s) == 8);

