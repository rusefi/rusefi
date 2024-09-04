// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/engine_cycle/high_pressure_fuel_pump.txt Wed Jan 03 20:30:10 UTC 2024
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of high_pressure_fuel_pump_s
struct high_pressure_fuel_pump_s {
	/**
	 * GDI: HPFP duration
	 * Computed requested pump duration in degrees (not including deadtime)
	 * offset 0
	 */
	angle_t m_requested_pump = (angle_t)0;
	/**
	 * "GDI: requested_percent"
	 * offset 4
	 */
	float fuel_requested_percent = (float)0;
	/**
	 * "GDI: percent_pi"
	 * offset 8
	 */
	float fuel_requested_percent_pi = (float)0;
	/**
	offset 12 bit 0 */
	bool noValve : 1 {};
	/**
	offset 12 bit 1 */
	bool angleAboveMin : 1 {};
	/**
	offset 12 bit 2 */
	bool isHpfpInactive : 1 {};
	/**
	offset 12 bit 3 */
	bool unusedBit_6_3 : 1 {};
	/**
	offset 12 bit 4 */
	bool unusedBit_6_4 : 1 {};
	/**
	offset 12 bit 5 */
	bool unusedBit_6_5 : 1 {};
	/**
	offset 12 bit 6 */
	bool unusedBit_6_6 : 1 {};
	/**
	offset 12 bit 7 */
	bool unusedBit_6_7 : 1 {};
	/**
	offset 12 bit 8 */
	bool unusedBit_6_8 : 1 {};
	/**
	offset 12 bit 9 */
	bool unusedBit_6_9 : 1 {};
	/**
	offset 12 bit 10 */
	bool unusedBit_6_10 : 1 {};
	/**
	offset 12 bit 11 */
	bool unusedBit_6_11 : 1 {};
	/**
	offset 12 bit 12 */
	bool unusedBit_6_12 : 1 {};
	/**
	offset 12 bit 13 */
	bool unusedBit_6_13 : 1 {};
	/**
	offset 12 bit 14 */
	bool unusedBit_6_14 : 1 {};
	/**
	offset 12 bit 15 */
	bool unusedBit_6_15 : 1 {};
	/**
	offset 12 bit 16 */
	bool unusedBit_6_16 : 1 {};
	/**
	offset 12 bit 17 */
	bool unusedBit_6_17 : 1 {};
	/**
	offset 12 bit 18 */
	bool unusedBit_6_18 : 1 {};
	/**
	offset 12 bit 19 */
	bool unusedBit_6_19 : 1 {};
	/**
	offset 12 bit 20 */
	bool unusedBit_6_20 : 1 {};
	/**
	offset 12 bit 21 */
	bool unusedBit_6_21 : 1 {};
	/**
	offset 12 bit 22 */
	bool unusedBit_6_22 : 1 {};
	/**
	offset 12 bit 23 */
	bool unusedBit_6_23 : 1 {};
	/**
	offset 12 bit 24 */
	bool unusedBit_6_24 : 1 {};
	/**
	offset 12 bit 25 */
	bool unusedBit_6_25 : 1 {};
	/**
	offset 12 bit 26 */
	bool unusedBit_6_26 : 1 {};
	/**
	offset 12 bit 27 */
	bool unusedBit_6_27 : 1 {};
	/**
	offset 12 bit 28 */
	bool unusedBit_6_28 : 1 {};
	/**
	offset 12 bit 29 */
	bool unusedBit_6_29 : 1 {};
	/**
	offset 12 bit 30 */
	bool unusedBit_6_30 : 1 {};
	/**
	offset 12 bit 31 */
	bool unusedBit_6_31 : 1 {};
	/**
	 * "GDI: target pressure"
	 * offset 16
	 */
	float m_pressureTarget_kPa = (float)0;
	/**
	 * "GDI: nextLobe"
	 * offset 20
	 */
	angle_t nextLobe = (angle_t)0;
	/**
	 * GDI: next start
	 * units: v
	 * offset 24
	 */
	float di_nextStart = (float)0;
};
static_assert(sizeof(high_pressure_fuel_pump_s) == 28);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/engine_cycle/high_pressure_fuel_pump.txt Wed Jan 03 20:30:10 UTC 2024
