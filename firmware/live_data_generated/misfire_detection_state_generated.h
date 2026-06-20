// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/misfire_detection_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of misfire_detection_state_s
struct misfire_detection_state_s {
	/**
	 * Misfire: active
	offset 0 bit 0 */
	bool misfireDetectionActive : 1 {};
	/**
	 * Misfire: MIL latched
	offset 0 bit 1 */
	bool misfireLatched : 1 {};
	/**
	offset 0 bit 2 */
	bool unusedBit_2_2 : 1 {};
	/**
	offset 0 bit 3 */
	bool unusedBit_2_3 : 1 {};
	/**
	offset 0 bit 4 */
	bool unusedBit_2_4 : 1 {};
	/**
	offset 0 bit 5 */
	bool unusedBit_2_5 : 1 {};
	/**
	offset 0 bit 6 */
	bool unusedBit_2_6 : 1 {};
	/**
	offset 0 bit 7 */
	bool unusedBit_2_7 : 1 {};
	/**
	offset 0 bit 8 */
	bool unusedBit_2_8 : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_2_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_2_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_2_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_2_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_2_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_2_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_2_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_2_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_2_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_2_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_2_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_2_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_2_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_2_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_2_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_2_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_2_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_2_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_2_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_2_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_2_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_2_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_2_31 : 1 {};
	/**
	 * Misfire: total count
	 * units: events
	 * offset 4
	 */
	uint8_t misfireTotalCount = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3] = {};
	/**
	 * Misfire: EMA baseline
	 * units: ms
	 * offset 8
	 */
	float misfireEmaUs = (float)0;
	/**
	 * Misfire: last segment
	 * units: ms
	 * offset 12
	 */
	float misfireLastSegUs = (float)0;
	/**
	 * Misfire: threshold
	 * units: ms
	 * offset 16
	 */
	float misfireThreshUs = (float)0;
	/**
	 * Misfire: wobble EMA
	 * units: ms
	 * offset 20
	 */
	float misfireWobbleUs = (float)0;
	/**
	 * Misfire: wobble thresh
	 * units: ms
	 * offset 24
	 */
	float misfireWobbleThreshUs = (float)0;
};
static_assert(sizeof(misfire_detection_state_s) == 28);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/misfire_detection_state.txt
