// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/vvt.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of vvt_s
struct vvt_s {
	/**
	 * units: deg
	 * offset 0
	 */
	scaled_channel<uint16_t, 10, 1> vvtTarget = (uint16_t)0;
	/**
	 * units: %
	 * offset 2
	 */
	scaled_channel<uint8_t, 2, 1> vvtOutput = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3
	 */
	uint8_t alignmentFill_at_3[1] = {};
};
static_assert(sizeof(vvt_s) == 4);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/vvt.txt
