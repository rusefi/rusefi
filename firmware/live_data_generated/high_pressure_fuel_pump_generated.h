// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/engine_cycle/high_pressure_fuel_pump.txt
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
	 * GDI: HPFP requested duty
	 * units: %
	 * offset 4
	 */
	float fuel_requested_percent = (float)0;
	/**
	 * "GDI: percent_pi"
	 * offset 8
	 */
	float fuel_requested_percent_pi = (float)0;
	/**
	 * GDI: HPFP no Valve control output
	offset 12 bit 0 */
	bool noValve : 1 {};
	/**
	 * GDI: HPFP angle above min
	offset 12 bit 1 */
	bool angleAboveMin : 1 {};
	/**
	 * GDI: HPFP Active
	offset 12 bit 2 */
	bool isHpfpActive : 1 {};
	/**
	 * GDI: HPFP Valve state
	offset 12 bit 3 */
	bool HpfpValveState : 1 {};
	/**
	offset 12 bit 4 */
	bool unusedBit_7_4 : 1 {};
	/**
	offset 12 bit 5 */
	bool unusedBit_7_5 : 1 {};
	/**
	offset 12 bit 6 */
	bool unusedBit_7_6 : 1 {};
	/**
	offset 12 bit 7 */
	bool unusedBit_7_7 : 1 {};
	/**
	offset 12 bit 8 */
	bool unusedBit_7_8 : 1 {};
	/**
	offset 12 bit 9 */
	bool unusedBit_7_9 : 1 {};
	/**
	offset 12 bit 10 */
	bool unusedBit_7_10 : 1 {};
	/**
	offset 12 bit 11 */
	bool unusedBit_7_11 : 1 {};
	/**
	offset 12 bit 12 */
	bool unusedBit_7_12 : 1 {};
	/**
	offset 12 bit 13 */
	bool unusedBit_7_13 : 1 {};
	/**
	offset 12 bit 14 */
	bool unusedBit_7_14 : 1 {};
	/**
	offset 12 bit 15 */
	bool unusedBit_7_15 : 1 {};
	/**
	offset 12 bit 16 */
	bool unusedBit_7_16 : 1 {};
	/**
	offset 12 bit 17 */
	bool unusedBit_7_17 : 1 {};
	/**
	offset 12 bit 18 */
	bool unusedBit_7_18 : 1 {};
	/**
	offset 12 bit 19 */
	bool unusedBit_7_19 : 1 {};
	/**
	offset 12 bit 20 */
	bool unusedBit_7_20 : 1 {};
	/**
	offset 12 bit 21 */
	bool unusedBit_7_21 : 1 {};
	/**
	offset 12 bit 22 */
	bool unusedBit_7_22 : 1 {};
	/**
	offset 12 bit 23 */
	bool unusedBit_7_23 : 1 {};
	/**
	offset 12 bit 24 */
	bool unusedBit_7_24 : 1 {};
	/**
	offset 12 bit 25 */
	bool unusedBit_7_25 : 1 {};
	/**
	offset 12 bit 26 */
	bool unusedBit_7_26 : 1 {};
	/**
	offset 12 bit 27 */
	bool unusedBit_7_27 : 1 {};
	/**
	offset 12 bit 28 */
	bool unusedBit_7_28 : 1 {};
	/**
	offset 12 bit 29 */
	bool unusedBit_7_29 : 1 {};
	/**
	offset 12 bit 30 */
	bool unusedBit_7_30 : 1 {};
	/**
	offset 12 bit 31 */
	bool unusedBit_7_31 : 1 {};
	/**
	 * "GDI: target pressure"
	 * offset 16
	 */
	float m_pressureTarget_kPa = (float)0;
	/**
	 * offset 20
	 */
	float hpfp_p_control_percent = (float)0;
	/**
	 * offset 24
	 */
	float hpfp_i_control_percent = (float)0;
	/**
	 * "GDI: next lobe N"
	 * offset 28
	 */
	angle_t nextLobe = (angle_t)0;
	/**
	 * GDI: valve activation angle
	 * units: v
	 * offset 32
	 */
	float di_nextStart = (float)0;
	/**
	 * "GDI: HPFP activation angle"
	 * offset 36
	 */
	float HpfdActivationPhase = (float)0;
	/**
	 * "GDI: HPFP deactivation angle"
	 * offset 40
	 */
	float HpfdDeactivationPhase = (float)0;
};
static_assert(sizeof(high_pressure_fuel_pump_s) == 44);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/engine_cycle/high_pressure_fuel_pump.txt
