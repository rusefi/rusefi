// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/tps_accel_state.txt Sun Oct 31 02:08:25 UTC 2021
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
	bool unusedBit_9_4 : 1;
	/**
	offset 20 bit 5 */
	bool unusedBit_9_5 : 1;
	/**
	offset 20 bit 6 */
	bool unusedBit_9_6 : 1;
	/**
	offset 20 bit 7 */
	bool unusedBit_9_7 : 1;
	/**
	offset 20 bit 8 */
	bool unusedBit_9_8 : 1;
	/**
	offset 20 bit 9 */
	bool unusedBit_9_9 : 1;
	/**
	offset 20 bit 10 */
	bool unusedBit_9_10 : 1;
	/**
	offset 20 bit 11 */
	bool unusedBit_9_11 : 1;
	/**
	offset 20 bit 12 */
	bool unusedBit_9_12 : 1;
	/**
	offset 20 bit 13 */
	bool unusedBit_9_13 : 1;
	/**
	offset 20 bit 14 */
	bool unusedBit_9_14 : 1;
	/**
	offset 20 bit 15 */
	bool unusedBit_9_15 : 1;
	/**
	offset 20 bit 16 */
	bool unusedBit_9_16 : 1;
	/**
	offset 20 bit 17 */
	bool unusedBit_9_17 : 1;
	/**
	offset 20 bit 18 */
	bool unusedBit_9_18 : 1;
	/**
	offset 20 bit 19 */
	bool unusedBit_9_19 : 1;
	/**
	offset 20 bit 20 */
	bool unusedBit_9_20 : 1;
	/**
	offset 20 bit 21 */
	bool unusedBit_9_21 : 1;
	/**
	offset 20 bit 22 */
	bool unusedBit_9_22 : 1;
	/**
	offset 20 bit 23 */
	bool unusedBit_9_23 : 1;
	/**
	offset 20 bit 24 */
	bool unusedBit_9_24 : 1;
	/**
	offset 20 bit 25 */
	bool unusedBit_9_25 : 1;
	/**
	offset 20 bit 26 */
	bool unusedBit_9_26 : 1;
	/**
	offset 20 bit 27 */
	bool unusedBit_9_27 : 1;
	/**
	offset 20 bit 28 */
	bool unusedBit_9_28 : 1;
	/**
	offset 20 bit 29 */
	bool unusedBit_9_29 : 1;
	/**
	offset 20 bit 30 */
	bool unusedBit_9_30 : 1;
	/**
	offset 20 bit 31 */
	bool unusedBit_9_31 : 1;
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
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/tps_accel_state.txt Sun Oct 31 02:08:25 UTC 2021
