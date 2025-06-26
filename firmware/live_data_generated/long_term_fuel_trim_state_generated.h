// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/long_term_fuel_trim_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of long_term_fuel_trim_state_s
struct long_term_fuel_trim_state_s {
	/**
	 * LTFT lerning: hits
	 * units: cnt
	 * offset 0
	 */
	scaled_channel<uint32_t, 1, 1> ltftCntHit = (uint32_t)0;
	/**
	 * LTFT learning: miss
	 * units: cnt
	 * offset 4
	 */
	scaled_channel<uint32_t, 1, 1> ltftCntMiss = (uint32_t)0;
	/**
	 * LTFT learning: in deadband
	 * units: cnt
	 * offset 8
	 */
	scaled_channel<uint32_t, 1, 1> ltftCntDeadband = (uint32_t)0;
	/**
	 * LTFT learning
	offset 12 bit 0 */
	bool ltftLearning : 1 {};
	/**
	 * LTFT correcting
	offset 12 bit 1 */
	bool ltftCorrecting : 1 {};
	/**
	offset 12 bit 2 */
	bool unusedBit_5_2 : 1 {};
	/**
	offset 12 bit 3 */
	bool unusedBit_5_3 : 1 {};
	/**
	offset 12 bit 4 */
	bool unusedBit_5_4 : 1 {};
	/**
	offset 12 bit 5 */
	bool unusedBit_5_5 : 1 {};
	/**
	offset 12 bit 6 */
	bool unusedBit_5_6 : 1 {};
	/**
	offset 12 bit 7 */
	bool unusedBit_5_7 : 1 {};
	/**
	offset 12 bit 8 */
	bool unusedBit_5_8 : 1 {};
	/**
	offset 12 bit 9 */
	bool unusedBit_5_9 : 1 {};
	/**
	offset 12 bit 10 */
	bool unusedBit_5_10 : 1 {};
	/**
	offset 12 bit 11 */
	bool unusedBit_5_11 : 1 {};
	/**
	offset 12 bit 12 */
	bool unusedBit_5_12 : 1 {};
	/**
	offset 12 bit 13 */
	bool unusedBit_5_13 : 1 {};
	/**
	offset 12 bit 14 */
	bool unusedBit_5_14 : 1 {};
	/**
	offset 12 bit 15 */
	bool unusedBit_5_15 : 1 {};
	/**
	offset 12 bit 16 */
	bool unusedBit_5_16 : 1 {};
	/**
	offset 12 bit 17 */
	bool unusedBit_5_17 : 1 {};
	/**
	offset 12 bit 18 */
	bool unusedBit_5_18 : 1 {};
	/**
	offset 12 bit 19 */
	bool unusedBit_5_19 : 1 {};
	/**
	offset 12 bit 20 */
	bool unusedBit_5_20 : 1 {};
	/**
	offset 12 bit 21 */
	bool unusedBit_5_21 : 1 {};
	/**
	offset 12 bit 22 */
	bool unusedBit_5_22 : 1 {};
	/**
	offset 12 bit 23 */
	bool unusedBit_5_23 : 1 {};
	/**
	offset 12 bit 24 */
	bool unusedBit_5_24 : 1 {};
	/**
	offset 12 bit 25 */
	bool unusedBit_5_25 : 1 {};
	/**
	offset 12 bit 26 */
	bool unusedBit_5_26 : 1 {};
	/**
	offset 12 bit 27 */
	bool unusedBit_5_27 : 1 {};
	/**
	offset 12 bit 28 */
	bool unusedBit_5_28 : 1 {};
	/**
	offset 12 bit 29 */
	bool unusedBit_5_29 : 1 {};
	/**
	offset 12 bit 30 */
	bool unusedBit_5_30 : 1 {};
	/**
	offset 12 bit 31 */
	bool unusedBit_5_31 : 1 {};
	/**
	 * LTFT: Bank
	 * units: %
	 * offset 16
	 */
	float ltftCorrection[FT_BANK_COUNT] = {};
};
static_assert(sizeof(long_term_fuel_trim_state_s) == 24);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/long_term_fuel_trim_state.txt
