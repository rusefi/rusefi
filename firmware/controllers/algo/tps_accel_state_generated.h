// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/tps_accel_state.txt Sun Oct 31 02:30:53 UTC 2021
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of tps_accel_state_s
struct tps_accel_state_s {
	/**
	 * offset 0
	 */
	percent_t tpsFrom = (percent_t)0;
	/**
	 * offset 4
	 */
	percent_t tpsTo = (percent_t)0;
	/**
	 * offset 8
	 */
	percent_t deltaTps = (percent_t)0;
	/**
	 * offset 12
	 */
	floatms_t extraFuel = (floatms_t)0;
	/**
	 * offset 16
	 */
	float valueFromTable = (float)0;
	/**
	offset 20 bit 0 */
	bool isAboveAccelThreshold : 1;
	/**
	offset 20 bit 1 */
	bool isBelowDecelThreshold : 1;
	/**
	offset 20 bit 2 */
	bool isTimeToResetAccumulator : 1;
	/**
	offset 20 bit 3 */
	bool isFractionalEnrichment : 1;
	/**
	offset 20 bit 4 */
	bool belowEpsilon : 1;
	/**
	offset 20 bit 5 */
	bool unusedBit_10_5 : 1;
	/**
	offset 20 bit 6 */
	bool unusedBit_10_6 : 1;
	/**
	offset 20 bit 7 */
	bool unusedBit_10_7 : 1;
	/**
	offset 20 bit 8 */
	bool unusedBit_10_8 : 1;
	/**
	offset 20 bit 9 */
	bool unusedBit_10_9 : 1;
	/**
	offset 20 bit 10 */
	bool unusedBit_10_10 : 1;
	/**
	offset 20 bit 11 */
	bool unusedBit_10_11 : 1;
	/**
	offset 20 bit 12 */
	bool unusedBit_10_12 : 1;
	/**
	offset 20 bit 13 */
	bool unusedBit_10_13 : 1;
	/**
	offset 20 bit 14 */
	bool unusedBit_10_14 : 1;
	/**
	offset 20 bit 15 */
	bool unusedBit_10_15 : 1;
	/**
	offset 20 bit 16 */
	bool unusedBit_10_16 : 1;
	/**
	offset 20 bit 17 */
	bool unusedBit_10_17 : 1;
	/**
	offset 20 bit 18 */
	bool unusedBit_10_18 : 1;
	/**
	offset 20 bit 19 */
	bool unusedBit_10_19 : 1;
	/**
	offset 20 bit 20 */
	bool unusedBit_10_20 : 1;
	/**
	offset 20 bit 21 */
	bool unusedBit_10_21 : 1;
	/**
	offset 20 bit 22 */
	bool unusedBit_10_22 : 1;
	/**
	offset 20 bit 23 */
	bool unusedBit_10_23 : 1;
	/**
	offset 20 bit 24 */
	bool unusedBit_10_24 : 1;
	/**
	offset 20 bit 25 */
	bool unusedBit_10_25 : 1;
	/**
	offset 20 bit 26 */
	bool unusedBit_10_26 : 1;
	/**
	offset 20 bit 27 */
	bool unusedBit_10_27 : 1;
	/**
	offset 20 bit 28 */
	bool unusedBit_10_28 : 1;
	/**
	offset 20 bit 29 */
	bool unusedBit_10_29 : 1;
	/**
	offset 20 bit 30 */
	bool unusedBit_10_30 : 1;
	/**
	offset 20 bit 31 */
	bool unusedBit_10_31 : 1;
	/**
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
	/** total size 48*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/tps_accel_state.txt Sun Oct 31 02:30:53 UTC 2021
