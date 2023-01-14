// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/timing_state.txt Sat Jan 14 12:55:04 EST 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of timing_state_s
struct timing_state_s {
	/**
	 * ignition dwell duration in ms
	 * See also dwellAngle
	 * offset 0
	 */
	floatms_t sparkDwell = (floatms_t)0;
	/**
	 * ignition dwell duration
	 * as crankshaft angle
	 * NAN if engine is stopped
	 * See also sparkDwell
	 * offset 4
	 */
	angle_t dwellAngle = (angle_t)0;
	/**
	deg
	 * offset 8
	 */
	scaled_channel<int16_t, 100, 1> cltTimingCorrection = (int16_t)0;
	/**
	deg
	 * offset 10
	 */
	scaled_channel<int16_t, 100, 1> timingIatCorrection = (int16_t)0;
	/**
	deg
	 * offset 12
	 */
	scaled_channel<int16_t, 100, 1> timingPidCorrection = (int16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 14
	 */
	uint8_t alignmentFill_at_14[2];
};
static_assert(sizeof(timing_state_s) == 16);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/timing_state.txt Sat Jan 14 12:55:04 EST 2023
