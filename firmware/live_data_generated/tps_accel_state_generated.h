// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/tps_accel_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of tps_accel_state_s
struct tps_accel_state_s {
	/**
	 * Fuel: TPS AE from
	 * offset 0
	 */
	percent_t tpsFrom = (percent_t)0;
	/**
	 * Fuel: TPS AE to
	 * offset 4
	 */
	percent_t tpsTo = (percent_t)0;
	/**
	 * Fuel: TPS AE change
	 * offset 8
	 */
	percent_t deltaTps = (percent_t)0;
	/**
	 * Fuel: TPS extraFuel
	 * offset 12
	 */
	floatms_t extraFuel = (floatms_t)0;
	/**
	 * Fuel: TPS valueFromTable
	 * offset 16
	 */
	float valueFromTable = (float)0;
	/**
	 * Fuel: TPS AE Active
	offset 20 bit 0 */
	bool isAboveAccelThreshold : 1 {};
	/**
	offset 20 bit 1 */
	bool isBelowDecelThreshold : 1 {};
	/**
	 * Fuel: TPS AE: reset time
	offset 20 bit 2 */
	bool isTimeToResetAccumulator : 1 {};
	/**
	offset 20 bit 3 */
	bool isFractionalEnrichment : 1 {};
	/**
	 * Fuel: TPS AE: below threshold
	offset 20 bit 4 */
	bool belowEpsilon : 1 {};
	/**
	 * Fuel: TPS AE: too short
	offset 20 bit 5 */
	bool tooShort : 1 {};
	/**
	offset 20 bit 6 */
	bool unusedBit_11_6 : 1 {};
	/**
	offset 20 bit 7 */
	bool unusedBit_11_7 : 1 {};
	/**
	offset 20 bit 8 */
	bool unusedBit_11_8 : 1 {};
	/**
	offset 20 bit 9 */
	bool unusedBit_11_9 : 1 {};
	/**
	offset 20 bit 10 */
	bool unusedBit_11_10 : 1 {};
	/**
	offset 20 bit 11 */
	bool unusedBit_11_11 : 1 {};
	/**
	offset 20 bit 12 */
	bool unusedBit_11_12 : 1 {};
	/**
	offset 20 bit 13 */
	bool unusedBit_11_13 : 1 {};
	/**
	offset 20 bit 14 */
	bool unusedBit_11_14 : 1 {};
	/**
	offset 20 bit 15 */
	bool unusedBit_11_15 : 1 {};
	/**
	offset 20 bit 16 */
	bool unusedBit_11_16 : 1 {};
	/**
	offset 20 bit 17 */
	bool unusedBit_11_17 : 1 {};
	/**
	offset 20 bit 18 */
	bool unusedBit_11_18 : 1 {};
	/**
	offset 20 bit 19 */
	bool unusedBit_11_19 : 1 {};
	/**
	offset 20 bit 20 */
	bool unusedBit_11_20 : 1 {};
	/**
	offset 20 bit 21 */
	bool unusedBit_11_21 : 1 {};
	/**
	offset 20 bit 22 */
	bool unusedBit_11_22 : 1 {};
	/**
	offset 20 bit 23 */
	bool unusedBit_11_23 : 1 {};
	/**
	offset 20 bit 24 */
	bool unusedBit_11_24 : 1 {};
	/**
	offset 20 bit 25 */
	bool unusedBit_11_25 : 1 {};
	/**
	offset 20 bit 26 */
	bool unusedBit_11_26 : 1 {};
	/**
	offset 20 bit 27 */
	bool unusedBit_11_27 : 1 {};
	/**
	offset 20 bit 28 */
	bool unusedBit_11_28 : 1 {};
	/**
	offset 20 bit 29 */
	bool unusedBit_11_29 : 1 {};
	/**
	offset 20 bit 30 */
	bool unusedBit_11_30 : 1 {};
	/**
	offset 20 bit 31 */
	bool unusedBit_11_31 : 1 {};
	/**
	 * Fuel: TPS AE fractionalInjFuel
	 * offset 24
	 */
	float fractionalInjFuel = (float)0;
	/**
	 * offset 28
	 */
	floatms_t accumulatedValue = (floatms_t)0;
	/**
	 * offset 32
	 */
	floatms_t maxExtraPerCycle = (floatms_t)0;
	/**
	 * offset 36
	 */
	floatms_t maxExtraPerPeriod = (floatms_t)0;
	/**
	 * offset 40
	 */
	floatms_t maxInjectedPerPeriod = (floatms_t)0;
	/**
	 * offset 44
	 */
	int cycleCnt = (int)0;
};
static_assert(sizeof(tps_accel_state_s) == 48);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/tps_accel_state.txt
