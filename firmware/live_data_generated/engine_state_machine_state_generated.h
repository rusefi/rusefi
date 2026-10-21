// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/modules/engine_state_machine/engine_state_machine_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of engine_state_machine_state_s
struct engine_state_machine_state_s {
	/**
	 * enabled
	offset 0 bit 0 */
	bool engineSmEnabled : 1 {};
	/**
	 * Off
	offset 0 bit 1 */
	bool engineSmIsOff : 1 {};
	/**
	 * Cranking
	offset 0 bit 2 */
	bool engineSmIsCranking : 1 {};
	/**
	 * Afterstart
	offset 0 bit 3 */
	bool engineSmIsAfterStart : 1 {};
	/**
	 * Idle
	offset 0 bit 4 */
	bool engineSmIsIdle : 1 {};
	/**
	 * Coasting
	offset 0 bit 5 */
	bool engineSmIsCoasting : 1 {};
	/**
	 * Transient
	offset 0 bit 6 */
	bool engineSmIsTransient : 1 {};
	/**
	 * WOT
	offset 0 bit 7 */
	bool engineSmIsWot : 1 {};
	/**
	 * Cruising
	offset 0 bit 8 */
	bool engineSmIsCruising : 1 {};
	/**
	 * Overrun
	offset 0 bit 9 */
	bool engineSmIsOverrun : 1 {};
	/**
	 * Launch Control
	offset 0 bit 10 */
	bool engineSmIsLaunchControl : 1 {};
	/**
	 * Torque Reduction
	offset 0 bit 11 */
	bool engineSmIsTorqueReduction : 1 {};
	/**
	 * Upshifting
	offset 0 bit 12 */
	bool engineSmIsUpshifting : 1 {};
	/**
	 * Downshifting
	offset 0 bit 13 */
	bool engineSmIsDownshifting : 1 {};
	/**
	 * Cold engine
	offset 0 bit 14 */
	bool engineSmIsCold : 1 {};
	/**
	 * At operating temp
	offset 0 bit 15 */
	bool engineSmIsOperating : 1 {};
	/**
	 * Hot engine
	offset 0 bit 16 */
	bool engineSmIsHot : 1 {};
	/**
	 * Accelerating
	offset 0 bit 17 */
	bool engineSmIsAccelerating : 1 {};
	/**
	 * Decelerating
	offset 0 bit 18 */
	bool engineSmIsDecelerating : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_19_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_19_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_19_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_19_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_19_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_19_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_19_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_19_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_19_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_19_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_19_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_19_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_19_31 : 1 {};
	/**
	 * display state
	 * offset 4
	 */
	uint8_t engineSmCurrentState = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3] = {};
	/**
	 * Shift accumulator
	 * offset 8
	 */
	float engineSmShiftAccumulator = (float)0;
	/**
	 * RPM rate (RPM/t)
	 * units: RPM/s
	 * offset 12
	 */
	float engineSmRpmRate = (float)0;
};
static_assert(sizeof(engine_state_machine_state_s) == 16);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/modules/engine_state_machine/engine_state_machine_state.txt
