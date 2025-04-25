// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_state_primary.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of trigger_state_primary_s
struct trigger_state_primary_s {
	/**
	 * sync: Phase Re-Sync Counter
	 * offset 0
	 */
	uint8_t camResyncCounter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3] = {};
	/**
	 * sync: We Know Engine Phase
	offset 4 bit 0 */
	bool m_hasSynchronizedPhase : 1 {};
	/**
	offset 4 bit 1 */
	bool unusedBit_3_1 : 1 {};
	/**
	offset 4 bit 2 */
	bool unusedBit_3_2 : 1 {};
	/**
	offset 4 bit 3 */
	bool unusedBit_3_3 : 1 {};
	/**
	offset 4 bit 4 */
	bool unusedBit_3_4 : 1 {};
	/**
	offset 4 bit 5 */
	bool unusedBit_3_5 : 1 {};
	/**
	offset 4 bit 6 */
	bool unusedBit_3_6 : 1 {};
	/**
	offset 4 bit 7 */
	bool unusedBit_3_7 : 1 {};
	/**
	offset 4 bit 8 */
	bool unusedBit_3_8 : 1 {};
	/**
	offset 4 bit 9 */
	bool unusedBit_3_9 : 1 {};
	/**
	offset 4 bit 10 */
	bool unusedBit_3_10 : 1 {};
	/**
	offset 4 bit 11 */
	bool unusedBit_3_11 : 1 {};
	/**
	offset 4 bit 12 */
	bool unusedBit_3_12 : 1 {};
	/**
	offset 4 bit 13 */
	bool unusedBit_3_13 : 1 {};
	/**
	offset 4 bit 14 */
	bool unusedBit_3_14 : 1 {};
	/**
	offset 4 bit 15 */
	bool unusedBit_3_15 : 1 {};
	/**
	offset 4 bit 16 */
	bool unusedBit_3_16 : 1 {};
	/**
	offset 4 bit 17 */
	bool unusedBit_3_17 : 1 {};
	/**
	offset 4 bit 18 */
	bool unusedBit_3_18 : 1 {};
	/**
	offset 4 bit 19 */
	bool unusedBit_3_19 : 1 {};
	/**
	offset 4 bit 20 */
	bool unusedBit_3_20 : 1 {};
	/**
	offset 4 bit 21 */
	bool unusedBit_3_21 : 1 {};
	/**
	offset 4 bit 22 */
	bool unusedBit_3_22 : 1 {};
	/**
	offset 4 bit 23 */
	bool unusedBit_3_23 : 1 {};
	/**
	offset 4 bit 24 */
	bool unusedBit_3_24 : 1 {};
	/**
	offset 4 bit 25 */
	bool unusedBit_3_25 : 1 {};
	/**
	offset 4 bit 26 */
	bool unusedBit_3_26 : 1 {};
	/**
	offset 4 bit 27 */
	bool unusedBit_3_27 : 1 {};
	/**
	offset 4 bit 28 */
	bool unusedBit_3_28 : 1 {};
	/**
	offset 4 bit 29 */
	bool unusedBit_3_29 : 1 {};
	/**
	offset 4 bit 30 */
	bool unusedBit_3_30 : 1 {};
	/**
	offset 4 bit 31 */
	bool unusedBit_3_31 : 1 {};
};
static_assert(sizeof(trigger_state_primary_s) == 8);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_state_primary.txt
