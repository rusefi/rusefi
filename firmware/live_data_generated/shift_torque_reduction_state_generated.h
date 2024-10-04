// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/shift_torque_reduction_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of shift_torque_reduction_state_s
struct shift_torque_reduction_state_s {
	/**
	offset 0 bit 0 */
	bool isTorqueReductionTriggerPinValid : 1 {};
	/**
	offset 0 bit 1 */
	bool torqueReductionTriggerPinState : 1 {};
	/**
	offset 0 bit 2 */
	bool isTimeConditionSatisfied : 1 {};
	/**
	offset 0 bit 3 */
	bool isRpmConditionSatisfied : 1 {};
	/**
	offset 0 bit 4 */
	bool isAppConditionSatisfied : 1 {};
	/**
	offset 0 bit 5 */
	bool isFlatShiftConditionSatisfied : 1 {};
	/**
	offset 0 bit 6 */
	bool unusedBit_6_6 : 1 {};
	/**
	offset 0 bit 7 */
	bool unusedBit_6_7 : 1 {};
	/**
	offset 0 bit 8 */
	bool unusedBit_6_8 : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_6_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_6_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_6_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_6_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_6_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_6_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_6_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_6_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_6_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_6_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_6_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_6_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_6_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_6_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_6_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_6_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_6_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_6_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_6_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_6_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_6_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_6_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_6_31 : 1 {};
};
static_assert(sizeof(shift_torque_reduction_state_s) == 4);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/shift_torque_reduction_state.txt
