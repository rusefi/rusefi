// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/long_term_fuel_trim_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of long_term_fuel_trim_state_s
struct long_term_fuel_trim_state_s {
	/**
	 * LTFT learning: hits
	 * units: cnt
	 * offset 0
	 */
	uint32_t ltftCntHit = (uint32_t)0;
	/**
	 * LTFT learning: miss
	 * units: cnt
	 * offset 4
	 */
	uint32_t ltftCntMiss = (uint32_t)0;
	/**
	 * LTFT learning: in deadband
	 * units: cnt
	 * offset 8
	 */
	uint32_t ltftCntDeadband = (uint32_t)0;
	/**
	 * LTFT learning
	offset 12 bit 0 */
	bool ltftLearning : 1 {};
	/**
	 * LTFT correcting
	offset 12 bit 1 */
	bool ltftCorrecting : 1 {};
	/**
	 * LTFT saving
	offset 12 bit 2 */
	bool ltftSavePending : 1 {};
	/**
	 * LTFT loading calibration
	offset 12 bit 3 */
	bool ltftLoadPending : 1 {};
	/**
	 * LTFT loading calibration error
	offset 12 bit 4 */
	bool ltftLoadError : 1 {};
	/**
	offset 12 bit 5 */
	bool unusedBit_8_5 : 1 {};
	/**
	offset 12 bit 6 */
	bool unusedBit_8_6 : 1 {};
	/**
	offset 12 bit 7 */
	bool unusedBit_8_7 : 1 {};
	/**
	offset 12 bit 8 */
	bool unusedBit_8_8 : 1 {};
	/**
	offset 12 bit 9 */
	bool unusedBit_8_9 : 1 {};
	/**
	offset 12 bit 10 */
	bool unusedBit_8_10 : 1 {};
	/**
	offset 12 bit 11 */
	bool unusedBit_8_11 : 1 {};
	/**
	offset 12 bit 12 */
	bool unusedBit_8_12 : 1 {};
	/**
	offset 12 bit 13 */
	bool unusedBit_8_13 : 1 {};
	/**
	offset 12 bit 14 */
	bool unusedBit_8_14 : 1 {};
	/**
	offset 12 bit 15 */
	bool unusedBit_8_15 : 1 {};
	/**
	offset 12 bit 16 */
	bool unusedBit_8_16 : 1 {};
	/**
	offset 12 bit 17 */
	bool unusedBit_8_17 : 1 {};
	/**
	offset 12 bit 18 */
	bool unusedBit_8_18 : 1 {};
	/**
	offset 12 bit 19 */
	bool unusedBit_8_19 : 1 {};
	/**
	offset 12 bit 20 */
	bool unusedBit_8_20 : 1 {};
	/**
	offset 12 bit 21 */
	bool unusedBit_8_21 : 1 {};
	/**
	offset 12 bit 22 */
	bool unusedBit_8_22 : 1 {};
	/**
	offset 12 bit 23 */
	bool unusedBit_8_23 : 1 {};
	/**
	offset 12 bit 24 */
	bool unusedBit_8_24 : 1 {};
	/**
	offset 12 bit 25 */
	bool unusedBit_8_25 : 1 {};
	/**
	offset 12 bit 26 */
	bool unusedBit_8_26 : 1 {};
	/**
	offset 12 bit 27 */
	bool unusedBit_8_27 : 1 {};
	/**
	offset 12 bit 28 */
	bool unusedBit_8_28 : 1 {};
	/**
	offset 12 bit 29 */
	bool unusedBit_8_29 : 1 {};
	/**
	offset 12 bit 30 */
	bool unusedBit_8_30 : 1 {};
	/**
	offset 12 bit 31 */
	bool unusedBit_8_31 : 1 {};
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
