// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Fri May 05 17:56:33 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of running_fuel_s
struct running_fuel_s {
	/**
	 * Fuel: Post cranking mult
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
	 * @@GAUGE_NAME_FUEL_BASE@@
	 * This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	mg
	 * offset 16
	 */
	scaled_channel<uint16_t, 100, 1> baseFuel = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_RUNNING@@
	 * Total fuel with CLT IAT and TPS acceleration without injector lag corrections per cycle, as pulse per cycle
	mg
	 * offset 18
	 */
	scaled_channel<uint16_t, 100, 1> fuel = (uint16_t)0;
};
static_assert(sizeof(running_fuel_s) == 20);

// start of fuel_computer_s
struct fuel_computer_s {
	/**
	 * Fuel: Total correction
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
	 * offset 24
	 */
	scaled_channel<uint16_t, 100, 1> afrTableYAxis = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_LAMBDA@@
	 * offset 26
	 */
	scaled_channel<uint16_t, 10000, 1> targetLambda = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_AFR@@
	ratio
	 * offset 28
	 */
	scaled_channel<uint16_t, 1000, 1> targetAFR = (uint16_t)0;
	/**
	 * Fuel: Stoich ratio
	ratio
	 * offset 30
	 */
	scaled_channel<uint16_t, 1000, 1> stoichiometricRatio = (uint16_t)0;
	/**
	 * offset 32
	 */
	float sdTcharge_coff = (float)0;
	/**
	 * @@GAUGE_NAME_AIR_MASS@@
	g
	 * offset 36
	 */
	float sdAirMassInOneCylinder = (float)0;
	/**
	offset 40 bit 0 */
	bool injectorHwIssue : 1 {};
	/**
	offset 40 bit 1 */
	bool unusedBit_9_1 : 1 {};
	/**
	offset 40 bit 2 */
	bool unusedBit_9_2 : 1 {};
	/**
	offset 40 bit 3 */
	bool unusedBit_9_3 : 1 {};
	/**
	offset 40 bit 4 */
	bool unusedBit_9_4 : 1 {};
	/**
	offset 40 bit 5 */
	bool unusedBit_9_5 : 1 {};
	/**
	offset 40 bit 6 */
	bool unusedBit_9_6 : 1 {};
	/**
	offset 40 bit 7 */
	bool unusedBit_9_7 : 1 {};
	/**
	offset 40 bit 8 */
	bool unusedBit_9_8 : 1 {};
	/**
	offset 40 bit 9 */
	bool unusedBit_9_9 : 1 {};
	/**
	offset 40 bit 10 */
	bool unusedBit_9_10 : 1 {};
	/**
	offset 40 bit 11 */
	bool unusedBit_9_11 : 1 {};
	/**
	offset 40 bit 12 */
	bool unusedBit_9_12 : 1 {};
	/**
	offset 40 bit 13 */
	bool unusedBit_9_13 : 1 {};
	/**
	offset 40 bit 14 */
	bool unusedBit_9_14 : 1 {};
	/**
	offset 40 bit 15 */
	bool unusedBit_9_15 : 1 {};
	/**
	offset 40 bit 16 */
	bool unusedBit_9_16 : 1 {};
	/**
	offset 40 bit 17 */
	bool unusedBit_9_17 : 1 {};
	/**
	offset 40 bit 18 */
	bool unusedBit_9_18 : 1 {};
	/**
	offset 40 bit 19 */
	bool unusedBit_9_19 : 1 {};
	/**
	offset 40 bit 20 */
	bool unusedBit_9_20 : 1 {};
	/**
	offset 40 bit 21 */
	bool unusedBit_9_21 : 1 {};
	/**
	offset 40 bit 22 */
	bool unusedBit_9_22 : 1 {};
	/**
	offset 40 bit 23 */
	bool unusedBit_9_23 : 1 {};
	/**
	offset 40 bit 24 */
	bool unusedBit_9_24 : 1 {};
	/**
	offset 40 bit 25 */
	bool unusedBit_9_25 : 1 {};
	/**
	offset 40 bit 26 */
	bool unusedBit_9_26 : 1 {};
	/**
	offset 40 bit 27 */
	bool unusedBit_9_27 : 1 {};
	/**
	offset 40 bit 28 */
	bool unusedBit_9_28 : 1 {};
	/**
	offset 40 bit 29 */
	bool unusedBit_9_29 : 1 {};
	/**
	offset 40 bit 30 */
	bool unusedBit_9_30 : 1 {};
	/**
	offset 40 bit 31 */
	bool unusedBit_9_31 : 1 {};
};
static_assert(sizeof(fuel_computer_s) == 44);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Fri May 05 17:56:33 UTC 2023
