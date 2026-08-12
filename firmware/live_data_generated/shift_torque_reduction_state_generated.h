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
	bool isBelowTemperatureThreshold : 1 {};
	/**
	offset 0 bit 7 */
	bool unusedBit_7_7 : 1 {};
	/**
	offset 0 bit 8 */
	bool unusedBit_7_8 : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_7_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_7_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_7_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_7_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_7_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_7_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_7_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_7_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_7_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_7_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_7_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_7_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_7_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_7_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_7_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_7_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_7_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_7_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_7_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_7_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_7_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_7_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_7_31 : 1 {};
	/**
	 * offset 4
	 */
	int16_t trqRedCutXaxisValue = (int16_t)0;
	/**
	 * offset 6
	 */
	int16_t trqRedTimeXaxisValue = (int16_t)0;
	/**
	 * offset 8
	 */
	int16_t trqRedIgnRetXaxisValue = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2] = {};
};
static_assert(sizeof(shift_torque_reduction_state_s) == 12);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/shift_torque_reduction_state.txt
