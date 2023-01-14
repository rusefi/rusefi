// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Sat Jan 14 10:41:18 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of fuel_computer_s
struct fuel_computer_s {
	/**
	 * offset 0
	 */
	float totalFuelCorrection = (float)0;
	/**
	%
	 * offset 4
	 */
	scaled_channel<uint16_t, 100, 1> afrTableYAxis = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_LAMBDA@@
	 * offset 6
	 */
	scaled_channel<uint16_t, 10000, 1> targetLambda = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_AFR@@
	ratio
	 * offset 8
	 */
	scaled_channel<uint16_t, 1000, 1> targetAFR = (uint16_t)0;
	/**
	 * fuel: stoich ratio
	ratio
	 * offset 10
	 */
	scaled_channel<uint16_t, 1000, 1> stoichiometricRatio = (uint16_t)0;
	/**
	 * offset 12
	 */
	float sdTcharge_coff = (float)0;
	/**
	 * SD cylinder mass
	 * Speed-density logic: calculated air mass in one cylinder, in grams
	 * offset 16
	 */
	float sdAirMassInOneCylinder = (float)0;
	/**
	offset 20 bit 0 */
	bool sdIsTChargeAirModel : 1 {};
	/**
	offset 20 bit 1 */
	bool unusedBit_8_1 : 1 {};
	/**
	offset 20 bit 2 */
	bool unusedBit_8_2 : 1 {};
	/**
	offset 20 bit 3 */
	bool unusedBit_8_3 : 1 {};
	/**
	offset 20 bit 4 */
	bool unusedBit_8_4 : 1 {};
	/**
	offset 20 bit 5 */
	bool unusedBit_8_5 : 1 {};
	/**
	offset 20 bit 6 */
	bool unusedBit_8_6 : 1 {};
	/**
	offset 20 bit 7 */
	bool unusedBit_8_7 : 1 {};
	/**
	offset 20 bit 8 */
	bool unusedBit_8_8 : 1 {};
	/**
	offset 20 bit 9 */
	bool unusedBit_8_9 : 1 {};
	/**
	offset 20 bit 10 */
	bool unusedBit_8_10 : 1 {};
	/**
	offset 20 bit 11 */
	bool unusedBit_8_11 : 1 {};
	/**
	offset 20 bit 12 */
	bool unusedBit_8_12 : 1 {};
	/**
	offset 20 bit 13 */
	bool unusedBit_8_13 : 1 {};
	/**
	offset 20 bit 14 */
	bool unusedBit_8_14 : 1 {};
	/**
	offset 20 bit 15 */
	bool unusedBit_8_15 : 1 {};
	/**
	offset 20 bit 16 */
	bool unusedBit_8_16 : 1 {};
	/**
	offset 20 bit 17 */
	bool unusedBit_8_17 : 1 {};
	/**
	offset 20 bit 18 */
	bool unusedBit_8_18 : 1 {};
	/**
	offset 20 bit 19 */
	bool unusedBit_8_19 : 1 {};
	/**
	offset 20 bit 20 */
	bool unusedBit_8_20 : 1 {};
	/**
	offset 20 bit 21 */
	bool unusedBit_8_21 : 1 {};
	/**
	offset 20 bit 22 */
	bool unusedBit_8_22 : 1 {};
	/**
	offset 20 bit 23 */
	bool unusedBit_8_23 : 1 {};
	/**
	offset 20 bit 24 */
	bool unusedBit_8_24 : 1 {};
	/**
	offset 20 bit 25 */
	bool unusedBit_8_25 : 1 {};
	/**
	offset 20 bit 26 */
	bool unusedBit_8_26 : 1 {};
	/**
	offset 20 bit 27 */
	bool unusedBit_8_27 : 1 {};
	/**
	offset 20 bit 28 */
	bool unusedBit_8_28 : 1 {};
	/**
	offset 20 bit 29 */
	bool unusedBit_8_29 : 1 {};
	/**
	offset 20 bit 30 */
	bool unusedBit_8_30 : 1 {};
	/**
	offset 20 bit 31 */
	bool unusedBit_8_31 : 1 {};
};
static_assert(sizeof(fuel_computer_s) == 24);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Sat Jan 14 10:41:18 UTC 2023
