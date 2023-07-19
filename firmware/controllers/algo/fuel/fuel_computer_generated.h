#pragma once
#include "rusefi_types.h"
struct running_fuel_s {
	// Fuel: Post cranking mult
	// offset 0
	float postCrankingFuelCorrection = (float)0;
	// @@GAUGE_NAME_FUEL_IAT_CORR@@
	// offset 4
	float intakeTemperatureCoefficient = (float)0;
	// @@GAUGE_NAME_FUEL_CLT_CORR@@
	// offset 8
	float coolantTemperatureCoefficient = (float)0;
	// secs
	// offset 12
	float timeSinceCrankingInSecs = (float)0;
	// @@GAUGE_NAME_FUEL_BASE@@
	// This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	// mg
	// offset 16
	scaled_channel<uint16_t, 100, 1> baseFuel = (uint16_t)0;
	// @@GAUGE_NAME_FUEL_RUNNING@@
	// Total fuel with CLT IAT and TPS acceleration without injector lag corrections per cycle, as pulse per cycle
	// mg
	// offset 18
	scaled_channel<uint16_t, 100, 1> fuel = (uint16_t)0;
};
static_assert(sizeof(running_fuel_s) == 20);

struct fuel_computer_s {
	// Fuel: Total correction
	// mult
	// offset 0
	float totalFuelCorrection = (float)0;
	// offset 4
	running_fuel_s running;
	// %
	// offset 24
	scaled_channel<uint16_t, 100, 1> afrTableYAxis = (uint16_t)0;
	// @@GAUGE_NAME_TARGET_LAMBDA@@
	// offset 26
	scaled_channel<uint16_t, 10000, 1> targetLambda = (uint16_t)0;
	// @@GAUGE_NAME_TARGET_AFR@@
	// ratio
	// offset 28
	scaled_channel<uint16_t, 1000, 1> targetAFR = (uint16_t)0;
	// Fuel: Stoich ratio
	// ratio
	// offset 30
	scaled_channel<uint16_t, 1000, 1> stoichiometricRatio = (uint16_t)0;
	// offset 32
	float sdTcharge_coff = (float)0;
	// @@GAUGE_NAME_AIR_MASS@@
	// g
	// offset 36
	float sdAirMassInOneCylinder = (float)0;
};
static_assert(sizeof(fuel_computer_s) == 40);

