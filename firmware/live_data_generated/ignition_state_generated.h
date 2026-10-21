// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/ignition/ignition_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of ignition_state_s
struct ignition_state_s {
	/**
	 * "base dwell"
	 * units: ms
	 * offset 0
	 */
	float baseDwell = (float)0;
	/**
	 * @@GAUGE_COIL_DWELL_TIME@@
	 * units: ms
	 * offset 4
	 */
	floatms_t sparkDwell = (floatms_t)0;
	/**
	 * dwell duration
	 * as crankshaft angle
	 * NAN if engine is stopped
	 * See also sparkDwell
	 * units: deg
	 * offset 8
	 */
	angle_t dwellDurationAngle = (angle_t)0;
	/**
	 * CLT correction
	 * units: deg
	 * offset 12
	 */
	scaled_channel<int16_t, 100, 1> cltTimingCorrection = (int16_t)0;
	/**
	 * IAT correction
	 * units: deg
	 * offset 14
	 */
	scaled_channel<int16_t, 100, 1> timingIatCorrection = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TIMING_ADVANCE@@
	 * units: deg
	 * offset 16
	 */
	scaled_channel<int16_t, 50, 1> baseIgnitionAdvance = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ADJUSTED_TIMING@@
	 * units: deg
	 * offset 18
	 */
	scaled_channel<int16_t, 50, 1> correctedIgnitionAdvance = (int16_t)0;
	/**
	 * Dwell voltage correction
	 * offset 20
	 */
	float dwellVoltageCorrection = (float)0;
	/**
	 * Lua timing add
	 * units: deg
	 * offset 24
	 */
	float luaTimingAdd = (float)0;
	/**
	 * Lua timing mult
	 * units: deg
	 * offset 28
	 */
	float luaTimingMult = (float)0;
	/**
	 * Lua Spark Skip
	offset 32 bit 0 */
	bool luaIgnitionSkip : 1 {};
	/**
	offset 32 bit 1 */
	bool unusedBit_11_1 : 1 {};
	/**
	offset 32 bit 2 */
	bool unusedBit_11_2 : 1 {};
	/**
	offset 32 bit 3 */
	bool unusedBit_11_3 : 1 {};
	/**
	offset 32 bit 4 */
	bool unusedBit_11_4 : 1 {};
	/**
	offset 32 bit 5 */
	bool unusedBit_11_5 : 1 {};
	/**
	offset 32 bit 6 */
	bool unusedBit_11_6 : 1 {};
	/**
	offset 32 bit 7 */
	bool unusedBit_11_7 : 1 {};
	/**
	offset 32 bit 8 */
	bool unusedBit_11_8 : 1 {};
	/**
	offset 32 bit 9 */
	bool unusedBit_11_9 : 1 {};
	/**
	offset 32 bit 10 */
	bool unusedBit_11_10 : 1 {};
	/**
	offset 32 bit 11 */
	bool unusedBit_11_11 : 1 {};
	/**
	offset 32 bit 12 */
	bool unusedBit_11_12 : 1 {};
	/**
	offset 32 bit 13 */
	bool unusedBit_11_13 : 1 {};
	/**
	offset 32 bit 14 */
	bool unusedBit_11_14 : 1 {};
	/**
	offset 32 bit 15 */
	bool unusedBit_11_15 : 1 {};
	/**
	offset 32 bit 16 */
	bool unusedBit_11_16 : 1 {};
	/**
	offset 32 bit 17 */
	bool unusedBit_11_17 : 1 {};
	/**
	offset 32 bit 18 */
	bool unusedBit_11_18 : 1 {};
	/**
	offset 32 bit 19 */
	bool unusedBit_11_19 : 1 {};
	/**
	offset 32 bit 20 */
	bool unusedBit_11_20 : 1 {};
	/**
	offset 32 bit 21 */
	bool unusedBit_11_21 : 1 {};
	/**
	offset 32 bit 22 */
	bool unusedBit_11_22 : 1 {};
	/**
	offset 32 bit 23 */
	bool unusedBit_11_23 : 1 {};
	/**
	offset 32 bit 24 */
	bool unusedBit_11_24 : 1 {};
	/**
	offset 32 bit 25 */
	bool unusedBit_11_25 : 1 {};
	/**
	offset 32 bit 26 */
	bool unusedBit_11_26 : 1 {};
	/**
	offset 32 bit 27 */
	bool unusedBit_11_27 : 1 {};
	/**
	offset 32 bit 28 */
	bool unusedBit_11_28 : 1 {};
	/**
	offset 32 bit 29 */
	bool unusedBit_11_29 : 1 {};
	/**
	offset 32 bit 30 */
	bool unusedBit_11_30 : 1 {};
	/**
	offset 32 bit 31 */
	bool unusedBit_11_31 : 1 {};
	/**
	 * offset 36
	 */
	float loadForIgnitionTableDot = (float)0;
	/**
	 * Trailing spark deg
	 * units: deg
	 * offset 40
	 */
	scaled_channel<int16_t, 100, 1> trailingSparkAngle = (int16_t)0;
	/**
	 * offset 42
	 */
	int16_t rpmForIgnitionTableDot = (int16_t)0;
	/**
	 * RPM for Idle table
	 * offset 44
	 */
	int16_t rpmForIgnitionIdleTableDot = (int16_t)0;
	/**
	 * Idle: Timing adjustment
	 * units: deg
	 * offset 46
	 */
	scaled_channel<int16_t, 100, 1> timingPidCorrection = (int16_t)0;
	/**
	 * DFCO: Timing retard
	 * units: deg
	 * offset 48
	 */
	scaled_channel<int16_t, 100, 1> dfcoTimingRetard = (int16_t)0;
	/**
	 * Traction: timing correction
	 * units: deg
	 * offset 50
	 */
	scaled_channel<int16_t, 50, 1> tractionAdvanceDrop = (int16_t)0;
};
static_assert(sizeof(ignition_state_s) == 52);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/ignition/ignition_state.txt
