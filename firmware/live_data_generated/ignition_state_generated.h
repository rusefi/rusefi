// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/ignition_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of ignition_state_s
struct ignition_state_s {
	/**
	 * "Ignition: base dwell"
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
	 * Ignition: dwell duration
	 * as crankshaft angle
	 * NAN if engine is stopped
	 * See also sparkDwell
	 * units: deg
	 * offset 8
	 */
	angle_t dwellDurationAngle = (angle_t)0;
	/**
	 * Ign: CLT correction
	 * units: deg
	 * offset 12
	 */
	scaled_channel<int16_t, 100, 1> cltTimingCorrection = (int16_t)0;
	/**
	 * Ign: IAT correction
	 * units: deg
	 * offset 14
	 */
	scaled_channel<int16_t, 100, 1> timingIatCorrection = (int16_t)0;
	/**
	 * Idle: Timing adjustment
	 * units: deg
	 * offset 16
	 */
	scaled_channel<int16_t, 100, 1> timingPidCorrection = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TIMING_ADVANCE@@
	 * units: deg
	 * offset 18
	 */
	scaled_channel<int16_t, 50, 1> baseIgnitionAdvance = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ADJUSTED_TIMING@@
	 * units: deg
	 * offset 20
	 */
	scaled_channel<int16_t, 50, 1> correctedIgnitionAdvance = (int16_t)0;
	/**
	 * Traction: timing correction
	 * units: deg
	 * offset 22
	 */
	scaled_channel<int16_t, 50, 1> tractionAdvanceDrop = (int16_t)0;
	/**
	offset 24 bit 0 */
	bool accelThresholdThrigger : 1 {};
	/**
	offset 24 bit 1 */
	bool unusedBit_10_1 : 1 {};
	/**
	offset 24 bit 2 */
	bool unusedBit_10_2 : 1 {};
	/**
	offset 24 bit 3 */
	bool unusedBit_10_3 : 1 {};
	/**
	offset 24 bit 4 */
	bool unusedBit_10_4 : 1 {};
	/**
	offset 24 bit 5 */
	bool unusedBit_10_5 : 1 {};
	/**
	offset 24 bit 6 */
	bool unusedBit_10_6 : 1 {};
	/**
	offset 24 bit 7 */
	bool unusedBit_10_7 : 1 {};
	/**
	offset 24 bit 8 */
	bool unusedBit_10_8 : 1 {};
	/**
	offset 24 bit 9 */
	bool unusedBit_10_9 : 1 {};
	/**
	offset 24 bit 10 */
	bool unusedBit_10_10 : 1 {};
	/**
	offset 24 bit 11 */
	bool unusedBit_10_11 : 1 {};
	/**
	offset 24 bit 12 */
	bool unusedBit_10_12 : 1 {};
	/**
	offset 24 bit 13 */
	bool unusedBit_10_13 : 1 {};
	/**
	offset 24 bit 14 */
	bool unusedBit_10_14 : 1 {};
	/**
	offset 24 bit 15 */
	bool unusedBit_10_15 : 1 {};
	/**
	offset 24 bit 16 */
	bool unusedBit_10_16 : 1 {};
	/**
	offset 24 bit 17 */
	bool unusedBit_10_17 : 1 {};
	/**
	offset 24 bit 18 */
	bool unusedBit_10_18 : 1 {};
	/**
	offset 24 bit 19 */
	bool unusedBit_10_19 : 1 {};
	/**
	offset 24 bit 20 */
	bool unusedBit_10_20 : 1 {};
	/**
	offset 24 bit 21 */
	bool unusedBit_10_21 : 1 {};
	/**
	offset 24 bit 22 */
	bool unusedBit_10_22 : 1 {};
	/**
	offset 24 bit 23 */
	bool unusedBit_10_23 : 1 {};
	/**
	offset 24 bit 24 */
	bool unusedBit_10_24 : 1 {};
	/**
	offset 24 bit 25 */
	bool unusedBit_10_25 : 1 {};
	/**
	offset 24 bit 26 */
	bool unusedBit_10_26 : 1 {};
	/**
	offset 24 bit 27 */
	bool unusedBit_10_27 : 1 {};
	/**
	offset 24 bit 28 */
	bool unusedBit_10_28 : 1 {};
	/**
	offset 24 bit 29 */
	bool unusedBit_10_29 : 1 {};
	/**
	offset 24 bit 30 */
	bool unusedBit_10_30 : 1 {};
	/**
	offset 24 bit 31 */
	bool unusedBit_10_31 : 1 {};
	/**
	 * offset 28
	 */
	int accelDeltaLOADPersist = (int)0;
	/**
	 * offset 32
	 */
	uint32_t accelDeltaCicleThriger = (uint32_t)0;
	/**
	 * Ign: Dwell voltage correction
	 * offset 36
	 */
	float dwellVoltageCorrection = (float)0;
	/**
	 * Ign: Lua timing add
	 * units: deg
	 * offset 40
	 */
	float luaTimingAdd = (float)0;
	/**
	 * Ign: Lua timing mult
	 * units: deg
	 * offset 44
	 */
	float luaTimingMult = (float)0;
	/**
	 * Ign: Lua Spark Skip
	offset 48 bit 0 */
	bool luaIgnitionSkip : 1 {};
	/**
	offset 48 bit 1 */
	bool unusedBit_47_1 : 1 {};
	/**
	offset 48 bit 2 */
	bool unusedBit_47_2 : 1 {};
	/**
	offset 48 bit 3 */
	bool unusedBit_47_3 : 1 {};
	/**
	offset 48 bit 4 */
	bool unusedBit_47_4 : 1 {};
	/**
	offset 48 bit 5 */
	bool unusedBit_47_5 : 1 {};
	/**
	offset 48 bit 6 */
	bool unusedBit_47_6 : 1 {};
	/**
	offset 48 bit 7 */
	bool unusedBit_47_7 : 1 {};
	/**
	offset 48 bit 8 */
	bool unusedBit_47_8 : 1 {};
	/**
	offset 48 bit 9 */
	bool unusedBit_47_9 : 1 {};
	/**
	offset 48 bit 10 */
	bool unusedBit_47_10 : 1 {};
	/**
	offset 48 bit 11 */
	bool unusedBit_47_11 : 1 {};
	/**
	offset 48 bit 12 */
	bool unusedBit_47_12 : 1 {};
	/**
	offset 48 bit 13 */
	bool unusedBit_47_13 : 1 {};
	/**
	offset 48 bit 14 */
	bool unusedBit_47_14 : 1 {};
	/**
	offset 48 bit 15 */
	bool unusedBit_47_15 : 1 {};
	/**
	offset 48 bit 16 */
	bool unusedBit_47_16 : 1 {};
	/**
	offset 48 bit 17 */
	bool unusedBit_47_17 : 1 {};
	/**
	offset 48 bit 18 */
	bool unusedBit_47_18 : 1 {};
	/**
	offset 48 bit 19 */
	bool unusedBit_47_19 : 1 {};
	/**
	offset 48 bit 20 */
	bool unusedBit_47_20 : 1 {};
	/**
	offset 48 bit 21 */
	bool unusedBit_47_21 : 1 {};
	/**
	offset 48 bit 22 */
	bool unusedBit_47_22 : 1 {};
	/**
	offset 48 bit 23 */
	bool unusedBit_47_23 : 1 {};
	/**
	offset 48 bit 24 */
	bool unusedBit_47_24 : 1 {};
	/**
	offset 48 bit 25 */
	bool unusedBit_47_25 : 1 {};
	/**
	offset 48 bit 26 */
	bool unusedBit_47_26 : 1 {};
	/**
	offset 48 bit 27 */
	bool unusedBit_47_27 : 1 {};
	/**
	offset 48 bit 28 */
	bool unusedBit_47_28 : 1 {};
	/**
	offset 48 bit 29 */
	bool unusedBit_47_29 : 1 {};
	/**
	offset 48 bit 30 */
	bool unusedBit_47_30 : 1 {};
	/**
	offset 48 bit 31 */
	bool unusedBit_47_31 : 1 {};
};
static_assert(sizeof(ignition_state_s) == 52);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/ignition_state.txt
