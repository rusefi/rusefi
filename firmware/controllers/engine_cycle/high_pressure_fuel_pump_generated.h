#pragma once
#include "rusefi_types.h"
struct high_pressure_fuel_pump_s {
	// HPFP duration
	// Computed requested pump duration in degrees (not including deadtime)
	// offset 0
	angle_t m_requested_pump = (angle_t)0;
	// offset 4
	float fuel_requested_percent = (float)0;
	// offset 8
	float fuel_requested_percent_pi = (float)0;
	// offset 12 bit 0
	bool noValve : 1 {};
	// offset 12 bit 1
	bool angleAboveMin : 1 {};
	// offset 12 bit 2
	bool isHpfpInactive : 1 {};
	// offset 12 bit 3
	bool unusedBit_12_3 : 1 {};
	// offset 12 bit 4
	bool unusedBit_12_4 : 1 {};
	// offset 12 bit 5
	bool unusedBit_12_5 : 1 {};
	// offset 12 bit 6
	bool unusedBit_12_6 : 1 {};
	// offset 12 bit 7
	bool unusedBit_12_7 : 1 {};
	// offset 12 bit 8
	bool unusedBit_12_8 : 1 {};
	// offset 12 bit 9
	bool unusedBit_12_9 : 1 {};
	// offset 12 bit 10
	bool unusedBit_12_10 : 1 {};
	// offset 12 bit 11
	bool unusedBit_12_11 : 1 {};
	// offset 12 bit 12
	bool unusedBit_12_12 : 1 {};
	// offset 12 bit 13
	bool unusedBit_12_13 : 1 {};
	// offset 12 bit 14
	bool unusedBit_12_14 : 1 {};
	// offset 12 bit 15
	bool unusedBit_12_15 : 1 {};
	// offset 12 bit 16
	bool unusedBit_12_16 : 1 {};
	// offset 12 bit 17
	bool unusedBit_12_17 : 1 {};
	// offset 12 bit 18
	bool unusedBit_12_18 : 1 {};
	// offset 12 bit 19
	bool unusedBit_12_19 : 1 {};
	// offset 12 bit 20
	bool unusedBit_12_20 : 1 {};
	// offset 12 bit 21
	bool unusedBit_12_21 : 1 {};
	// offset 12 bit 22
	bool unusedBit_12_22 : 1 {};
	// offset 12 bit 23
	bool unusedBit_12_23 : 1 {};
	// offset 12 bit 24
	bool unusedBit_12_24 : 1 {};
	// offset 12 bit 25
	bool unusedBit_12_25 : 1 {};
	// offset 12 bit 26
	bool unusedBit_12_26 : 1 {};
	// offset 12 bit 27
	bool unusedBit_12_27 : 1 {};
	// offset 12 bit 28
	bool unusedBit_12_28 : 1 {};
	// offset 12 bit 29
	bool unusedBit_12_29 : 1 {};
	// offset 12 bit 30
	bool unusedBit_12_30 : 1 {};
	// offset 12 bit 31
	bool unusedBit_12_31 : 1 {};
	// offset 16
	angle_t nextLobe = (angle_t)0;
	// DI: next start
	// v
	// offset 20
	float di_nextStart = (float)0;
};
static_assert(sizeof(high_pressure_fuel_pump_s) == 24);
static_assert(offsetof(high_pressure_fuel_pump_s, m_requested_pump) == 0);
static_assert(offsetof(high_pressure_fuel_pump_s, fuel_requested_percent) == 4);
static_assert(offsetof(high_pressure_fuel_pump_s, fuel_requested_percent_pi) == 8);
static_assert(offsetof(high_pressure_fuel_pump_s, nextLobe) == 16);
static_assert(offsetof(high_pressure_fuel_pump_s, di_nextStart) == 20);

