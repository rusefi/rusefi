// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/tcu/tcu_controller.txt Wed Jan 03 03:40:23 UTC 2024
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of tcu_controller_s
struct tcu_controller_s {
	/**
	 * @@GAUGE_NAME_CURRENT_GEAR@@
	gear
	 * offset 0
	 */
	int8_t tcuCurrentGear = (int8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[1];
	/**
	 * @@GAUGE_NAME_TC_RATIO@@
	value
	 * offset 2
	 */
	scaled_channel<uint16_t, 100, 1> tcRatio = (uint16_t)0;
	/**
	 * offset 4
	 */
	float lastShiftTime = (float)0;
	/**
	 * "TCU: Current Range"
	 * offset 8
	 */
	uint8_t tcu_currentRange = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 9
	 */
	uint8_t alignmentFill_at_9[3];
};
static_assert(sizeof(tcu_controller_s) == 12);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/tcu/tcu_controller.txt Wed Jan 03 03:40:23 UTC 2024
