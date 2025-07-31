// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/math/short_term_fuel_trim_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of short_term_fuel_trim_state_s
struct short_term_fuel_trim_state_s {
	/**
	 * STFT: used bin
	 * units: Bin
	 * offset 0
	 */
	uint8_t stftCorrectionBinIdx = (uint8_t)0;
	/**
	 * STFT: learning bin
	 * units: Bin
	 * offset 1
	 */
	uint8_t stftLearningBinIdx = (uint8_t)0;
	/**
	 * offset 2
	 */
	stft_state_e stftCorrectionState;
	/**
	 * offset 3
	 */
	stft_state_e stftLearningState[FT_BANK_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3] = {};
	/**
	 * STFT: input Lambda error
	 * units: %
	 * offset 8
	 */
	float stftInputError[FT_BANK_COUNT] = {};
};
static_assert(sizeof(short_term_fuel_trim_state_s) == 16);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/math/short_term_fuel_trim_state.txt
