#pragma once
#include "rusefi_types.h"
struct ignition_state_s {
	// ms
	// offset 0
	float baseDwell = (float)0;
	// @@GAUGE_COIL_DWELL_TIME@@
	// ms
	// offset 4
	floatms_t sparkDwell = (floatms_t)0;
	// ignition dwell duration
	// as crankshaft angle
	// NAN if engine is stopped
	// See also sparkDwell
	// deg
	// offset 8
	angle_t dwellAngle = (angle_t)0;
	// Ign: CLT correction
	// deg
	// offset 12
	scaled_channel<int16_t, 100, 1> cltTimingCorrection = (int16_t)0;
	// Ign: IAT correction
	// deg
	// offset 14
	scaled_channel<int16_t, 100, 1> timingIatCorrection = (int16_t)0;
	// Idle: Timing adjustment
	// deg
	// offset 16
	scaled_channel<int16_t, 100, 1> timingPidCorrection = (int16_t)0;
	// offset 18
	uint8_t alignmentFill_at_18[2];
	// Ign: Dwell voltage correction
	// offset 20
	float dwellVoltageCorrection = (float)0;
	// Ign: Lua timing add
	// deg
	// offset 24
	float luaTimingAdd = (float)0;
	// Ign: Lua timing mult
	// deg
	// offset 28
	float luaTimingMult = (float)0;
};
static_assert(sizeof(ignition_state_s) == 32);

