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
	 * DFCO: Timing retard
	 * units: deg
	 * offset 18
	 */
	scaled_channel<int16_t, 100, 1> dfcoTimingRetard = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TIMING_ADVANCE@@
	 * units: deg
	 * offset 20
	 */
	scaled_channel<int16_t, 50, 1> baseIgnitionAdvance = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ADJUSTED_TIMING@@
	 * units: deg
	 * offset 22
	 */
	scaled_channel<int16_t, 50, 1> correctedIgnitionAdvance = (int16_t)0;
	/**
	 * Traction: timing correction
	 * units: deg
	 * offset 24
	 */
	scaled_channel<int16_t, 50, 1> tractionAdvanceDrop = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 26
	 */
	uint8_t alignmentFill_at_26[2];
	/**
	 * Ign: Dwell voltage correction
	 * offset 28
	 */
	float dwellVoltageCorrection = (float)0;
	/**
	 * Ign: Lua timing add
	 * units: deg
	 * offset 32
	 */
	float luaTimingAdd = (float)0;
	/**
	 * Ign: Lua timing mult
	 * units: deg
	 * offset 36
	 */
	float luaTimingMult = (float)0;
	/**
	 * Ign: Lua Spark Skip
	offset 40 bit 0 */
	bool luaIgnitionSkip : 1 {};
	/**
	offset 40 bit 1 */
	bool accelThresholdThrigger : 1 {};
	/**
	offset 40 bit 2 */
	bool unusedBit_16_2 : 1 {};
	/**
	offset 40 bit 3 */
	bool unusedBit_16_3 : 1 {};
	/**
	offset 40 bit 4 */
	bool unusedBit_16_4 : 1 {};
	/**
	offset 40 bit 5 */
	bool unusedBit_16_5 : 1 {};
	/**
	offset 40 bit 6 */
	bool unusedBit_16_6 : 1 {};
	/**
	offset 40 bit 7 */
	bool unusedBit_16_7 : 1 {};
	/**
	offset 40 bit 8 */
	bool unusedBit_16_8 : 1 {};
	/**
	offset 40 bit 9 */
	bool unusedBit_16_9 : 1 {};
	/**
	offset 40 bit 10 */
	bool unusedBit_16_10 : 1 {};
	/**
	offset 40 bit 11 */
	bool unusedBit_16_11 : 1 {};
	/**
	offset 40 bit 12 */
	bool unusedBit_16_12 : 1 {};
	/**
	offset 40 bit 13 */
	bool unusedBit_16_13 : 1 {};
	/**
	offset 40 bit 14 */
	bool unusedBit_16_14 : 1 {};
	/**
	offset 40 bit 15 */
	bool unusedBit_16_15 : 1 {};
	/**
	offset 40 bit 16 */
	bool unusedBit_16_16 : 1 {};
	/**
	offset 40 bit 17 */
	bool unusedBit_16_17 : 1 {};
	/**
	offset 40 bit 18 */
	bool unusedBit_16_18 : 1 {};
	/**
	offset 40 bit 19 */
	bool unusedBit_16_19 : 1 {};
	/**
	offset 40 bit 20 */
	bool unusedBit_16_20 : 1 {};
	/**
	offset 40 bit 21 */
	bool unusedBit_16_21 : 1 {};
	/**
	offset 40 bit 22 */
	bool unusedBit_16_22 : 1 {};
	/**
	offset 40 bit 23 */
	bool unusedBit_16_23 : 1 {};
	/**
	offset 40 bit 24 */
	bool unusedBit_16_24 : 1 {};
	/**
	offset 40 bit 25 */
	bool unusedBit_16_25 : 1 {};
	/**
	offset 40 bit 26 */
	bool unusedBit_16_26 : 1 {};
	/**
	offset 40 bit 27 */
	bool unusedBit_16_27 : 1 {};
	/**
	offset 40 bit 28 */
	bool unusedBit_16_28 : 1 {};
	/**
	offset 40 bit 29 */
	bool unusedBit_16_29 : 1 {};
	/**
	offset 40 bit 30 */
	bool unusedBit_16_30 : 1 {};
	/**
	offset 40 bit 31 */
	bool unusedBit_16_31 : 1 {};
	/**
	 * offset 44
	 */
	int16_t accelDeltaLOADPersist = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 46
	 */
	uint8_t alignmentFill_at_46[2];
	/**
	 * offset 48
	 */
	uint32_t accelDeltaCycleThriger = (uint32_t)0;
	/**
	 * offset 52
	 */
	float oldLoadValue = (float)0;
};
static_assert(sizeof(ignition_state_s) == 56);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/ignition_state.txt
