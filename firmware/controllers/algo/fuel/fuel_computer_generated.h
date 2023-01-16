// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Mon Jan 16 20:22:12 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of running_fuel_s
struct running_fuel_s {
	/**
	 * offset 0
	 */
	float postCrankingFuelCorrection = (float)0;
	/**
	 * @@GAUGE_NAME_FUEL_IAT_CORR@@
	 * offset 4
	 */
	float intakeTemperatureCoefficient = (float)0;
	/**
	 * @@GAUGE_NAME_FUEL_CLT_CORR@@
	 * offset 8
	 */
	float coolantTemperatureCoefficient = (float)0;
	/**
	secs
	 * offset 12
	 */
	float timeSinceCrankingInSecs = (float)0;
	/**
	 * offset 16
	 */
	floatms_t baseFuel = (floatms_t)0;
	/**
	 * Fuel: actual
	 * Actual injection duration with CLT, IAT and TPS acceleration corrections per cycle, as squirt duration.
	 * Without injector lag.
	 * @see baseFuel
	 * @see actualLastInjection
	 * offset 20
	 */
	floatms_t fuel = (floatms_t)0;
};
static_assert(sizeof(running_fuel_s) == 24);

// start of fuel_computer_s
struct fuel_computer_s {
	/**
	mult
	 * offset 0
	 */
	float totalFuelCorrection = (float)0;
	/**
	 * offset 4
	 */
	running_fuel_s running;
	/**
	%
	 * offset 28
	 */
	scaled_channel<uint16_t, 100, 1> afrTableYAxis = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_LAMBDA@@
	 * offset 30
	 */
	scaled_channel<uint16_t, 10000, 1> targetLambda = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_AFR@@
	ratio
	 * offset 32
	 */
	scaled_channel<uint16_t, 1000, 1> targetAFR = (uint16_t)0;
	/**
	 * fuel: stoich ratio
	ratio
	 * offset 34
	 */
	scaled_channel<uint16_t, 1000, 1> stoichiometricRatio = (uint16_t)0;
	/**
	 * offset 36
	 */
	float sdTcharge_coff = (float)0;
	/**
	 * SD cylinder mass
	 * Speed-density logic: calculated air mass in one cylinder, in grams
	 * offset 40
	 */
	float sdAirMassInOneCylinder = (float)0;
	/**
	offset 44 bit 0 */
	bool sdIsTChargeAirModel : 1 {};
	/**
	offset 44 bit 1 */
	bool unusedBit_9_1 : 1 {};
	/**
	offset 44 bit 2 */
	bool unusedBit_9_2 : 1 {};
	/**
	offset 44 bit 3 */
	bool unusedBit_9_3 : 1 {};
	/**
	offset 44 bit 4 */
	bool unusedBit_9_4 : 1 {};
	/**
	offset 44 bit 5 */
	bool unusedBit_9_5 : 1 {};
	/**
	offset 44 bit 6 */
	bool unusedBit_9_6 : 1 {};
	/**
	offset 44 bit 7 */
	bool unusedBit_9_7 : 1 {};
	/**
	offset 44 bit 8 */
	bool unusedBit_9_8 : 1 {};
	/**
	offset 44 bit 9 */
	bool unusedBit_9_9 : 1 {};
	/**
	offset 44 bit 10 */
	bool unusedBit_9_10 : 1 {};
	/**
	offset 44 bit 11 */
	bool unusedBit_9_11 : 1 {};
	/**
	offset 44 bit 12 */
	bool unusedBit_9_12 : 1 {};
	/**
	offset 44 bit 13 */
	bool unusedBit_9_13 : 1 {};
	/**
	offset 44 bit 14 */
	bool unusedBit_9_14 : 1 {};
	/**
	offset 44 bit 15 */
	bool unusedBit_9_15 : 1 {};
	/**
	offset 44 bit 16 */
	bool unusedBit_9_16 : 1 {};
	/**
	offset 44 bit 17 */
	bool unusedBit_9_17 : 1 {};
	/**
	offset 44 bit 18 */
	bool unusedBit_9_18 : 1 {};
	/**
	offset 44 bit 19 */
	bool unusedBit_9_19 : 1 {};
	/**
	offset 44 bit 20 */
	bool unusedBit_9_20 : 1 {};
	/**
	offset 44 bit 21 */
	bool unusedBit_9_21 : 1 {};
	/**
	offset 44 bit 22 */
	bool unusedBit_9_22 : 1 {};
	/**
	offset 44 bit 23 */
	bool unusedBit_9_23 : 1 {};
	/**
	offset 44 bit 24 */
	bool unusedBit_9_24 : 1 {};
	/**
	offset 44 bit 25 */
	bool unusedBit_9_25 : 1 {};
	/**
	offset 44 bit 26 */
	bool unusedBit_9_26 : 1 {};
	/**
	offset 44 bit 27 */
	bool unusedBit_9_27 : 1 {};
	/**
	offset 44 bit 28 */
	bool unusedBit_9_28 : 1 {};
	/**
	offset 44 bit 29 */
	bool unusedBit_9_29 : 1 {};
	/**
	offset 44 bit 30 */
	bool unusedBit_9_30 : 1 {};
	/**
	offset 44 bit 31 */
	bool unusedBit_9_31 : 1 {};
};
static_assert(sizeof(fuel_computer_s) == 48);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Mon Jan 16 20:22:12 UTC 2023
