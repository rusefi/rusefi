// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/ignition_state.txt Sun Mar 05 16:51:01 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of ignition_state_s
struct ignition_state_s {
	/**
	 * "Ignition: base dwell"
	ms
	 * offset 0
	 */
	float baseDwell = (float)0;
	/**
	 * @@GAUGE_COIL_DWELL_TIME@@
	ms
	 * offset 4
	 */
	floatms_t sparkDwell = (floatms_t)0;
	/**
	 * Ignition: dwell duration
	 * as crankshaft angle
	 * NAN if engine is stopped
	 * See also sparkDwell
	deg
	 * offset 8
	 */
	angle_t dwellAngle = (angle_t)0;
	/**
	 * "Ignition: CLT correction"
	deg
	 * offset 12
	 */
	scaled_channel<int16_t, 100, 1> cltTimingCorrection = (int16_t)0;
	/**
	 * "Ignition: IAT correction"
	deg
	 * offset 14
	 */
	scaled_channel<int16_t, 100, 1> timingIatCorrection = (int16_t)0;
	/**
	 * "Ignition: PID correction"
	deg
	 * offset 16
	 */
	scaled_channel<int16_t, 100, 1> timingPidCorrection = (int16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 18
	 */
	uint8_t alignmentFill_at_18[2];
	/**
	 * "Ignition: dwell corr"
	 * offset 20
	 */
	float dwellVoltageCorrection = (float)0;
	/**
	 * "Ignition: Lua add"
	deg
	 * offset 24
	 */
	float luaTimingAdd = (float)0;
	/**
	 * "Ignition: Lua mult"
	deg
	 * offset 28
	 */
	float luaTimingMult = (float)0;
};
static_assert(sizeof(ignition_state_s) == 32);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/ignition_state.txt Sun Mar 05 16:51:01 UTC 2023
