// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Sun Nov 06 00:27:04 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of fuel_computer_s
struct fuel_computer_s {
	/**
	%
	 * offset 0
	 */
	scaled_channel<uint16_t, 100, 1> afrTableYAxis = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_LAMBDA@@
	 * offset 2
	 */
	scaled_channel<uint16_t, 10000, 1> targetLambda = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_AFR@@
	ratio
	 * offset 4
	 */
	scaled_channel<uint16_t, 1000, 1> targetAFR = (uint16_t)0;
	/**
	 * fuel: stoich ratio
	ratio
	 * offset 6
	 */
	scaled_channel<uint16_t, 1000, 1> stoichiometricRatio = (uint16_t)0;
	/**
	 * offset 8
	 */
	float sdTcharge_coff = (float)0;
	/**
	 * SD cylinder mass
	 * Speed-density logic: calculated air mass in one cylinder, in grams
	 * offset 12
	 */
	float sdAirMassInOneCylinder = (float)0;
	/**
	offset 16 bit 0 */
	bool sdIsTChargeAirModel : 1 {};
	/**
	offset 16 bit 1 */
	bool unusedBit_7_1 : 1 {};
	/**
	offset 16 bit 2 */
	bool unusedBit_7_2 : 1 {};
	/**
	offset 16 bit 3 */
	bool unusedBit_7_3 : 1 {};
	/**
	offset 16 bit 4 */
	bool unusedBit_7_4 : 1 {};
	/**
	offset 16 bit 5 */
	bool unusedBit_7_5 : 1 {};
	/**
	offset 16 bit 6 */
	bool unusedBit_7_6 : 1 {};
	/**
	offset 16 bit 7 */
	bool unusedBit_7_7 : 1 {};
	/**
	offset 16 bit 8 */
	bool unusedBit_7_8 : 1 {};
	/**
	offset 16 bit 9 */
	bool unusedBit_7_9 : 1 {};
	/**
	offset 16 bit 10 */
	bool unusedBit_7_10 : 1 {};
	/**
	offset 16 bit 11 */
	bool unusedBit_7_11 : 1 {};
	/**
	offset 16 bit 12 */
	bool unusedBit_7_12 : 1 {};
	/**
	offset 16 bit 13 */
	bool unusedBit_7_13 : 1 {};
	/**
	offset 16 bit 14 */
	bool unusedBit_7_14 : 1 {};
	/**
	offset 16 bit 15 */
	bool unusedBit_7_15 : 1 {};
	/**
	offset 16 bit 16 */
	bool unusedBit_7_16 : 1 {};
	/**
	offset 16 bit 17 */
	bool unusedBit_7_17 : 1 {};
	/**
	offset 16 bit 18 */
	bool unusedBit_7_18 : 1 {};
	/**
	offset 16 bit 19 */
	bool unusedBit_7_19 : 1 {};
	/**
	offset 16 bit 20 */
	bool unusedBit_7_20 : 1 {};
	/**
	offset 16 bit 21 */
	bool unusedBit_7_21 : 1 {};
	/**
	offset 16 bit 22 */
	bool unusedBit_7_22 : 1 {};
	/**
	offset 16 bit 23 */
	bool unusedBit_7_23 : 1 {};
	/**
	offset 16 bit 24 */
	bool unusedBit_7_24 : 1 {};
	/**
	offset 16 bit 25 */
	bool unusedBit_7_25 : 1 {};
	/**
	offset 16 bit 26 */
	bool unusedBit_7_26 : 1 {};
	/**
	offset 16 bit 27 */
	bool unusedBit_7_27 : 1 {};
	/**
	offset 16 bit 28 */
	bool unusedBit_7_28 : 1 {};
	/**
	offset 16 bit 29 */
	bool unusedBit_7_29 : 1 {};
	/**
	offset 16 bit 30 */
	bool unusedBit_7_30 : 1 {};
	/**
	offset 16 bit 31 */
	bool unusedBit_7_31 : 1 {};
};
static_assert(sizeof(fuel_computer_s) == 20);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Sun Nov 06 00:27:04 UTC 2022
