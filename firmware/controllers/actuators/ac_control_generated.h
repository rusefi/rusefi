// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/actuators/ac_control.txt Wed Jan 03 20:30:10 UTC 2024
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of ac_control_s
struct ac_control_s {
	/**
	 * @@INDICATOR_NAME_AC_SWITCH@@
	 * offset 0
	 */
	int8_t acButtonState = (int8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	 * AC enabled
	offset 4 bit 0 */
	bool m_acEnabled : 1 {};
	/**
	 * AC engine too slow
	offset 4 bit 1 */
	bool engineTooSlow : 1 {};
	/**
	 * AC engine too fast
	offset 4 bit 2 */
	bool engineTooFast : 1 {};
	/**
	 * AC no CLT
	offset 4 bit 3 */
	bool noClt : 1 {};
	/**
	 * AC engine too hot
	offset 4 bit 4 */
	bool engineTooHot : 1 {};
	/**
	 * AC tps too high
	offset 4 bit 5 */
	bool tpsTooHigh : 1 {};
	/**
	 * AC disabled by Lua
	offset 4 bit 6 */
	bool isDisabledByLua : 1 {};
	/**
	 * AC compressor on
	offset 4 bit 7 */
	bool acCompressorState : 1 {};
	/**
	offset 4 bit 8 */
	bool unusedBit_10_8 : 1 {};
	/**
	offset 4 bit 9 */
	bool unusedBit_10_9 : 1 {};
	/**
	offset 4 bit 10 */
	bool unusedBit_10_10 : 1 {};
	/**
	offset 4 bit 11 */
	bool unusedBit_10_11 : 1 {};
	/**
	offset 4 bit 12 */
	bool unusedBit_10_12 : 1 {};
	/**
	offset 4 bit 13 */
	bool unusedBit_10_13 : 1 {};
	/**
	offset 4 bit 14 */
	bool unusedBit_10_14 : 1 {};
	/**
	offset 4 bit 15 */
	bool unusedBit_10_15 : 1 {};
	/**
	offset 4 bit 16 */
	bool unusedBit_10_16 : 1 {};
	/**
	offset 4 bit 17 */
	bool unusedBit_10_17 : 1 {};
	/**
	offset 4 bit 18 */
	bool unusedBit_10_18 : 1 {};
	/**
	offset 4 bit 19 */
	bool unusedBit_10_19 : 1 {};
	/**
	offset 4 bit 20 */
	bool unusedBit_10_20 : 1 {};
	/**
	offset 4 bit 21 */
	bool unusedBit_10_21 : 1 {};
	/**
	offset 4 bit 22 */
	bool unusedBit_10_22 : 1 {};
	/**
	offset 4 bit 23 */
	bool unusedBit_10_23 : 1 {};
	/**
	offset 4 bit 24 */
	bool unusedBit_10_24 : 1 {};
	/**
	offset 4 bit 25 */
	bool unusedBit_10_25 : 1 {};
	/**
	offset 4 bit 26 */
	bool unusedBit_10_26 : 1 {};
	/**
	offset 4 bit 27 */
	bool unusedBit_10_27 : 1 {};
	/**
	offset 4 bit 28 */
	bool unusedBit_10_28 : 1 {};
	/**
	offset 4 bit 29 */
	bool unusedBit_10_29 : 1 {};
	/**
	offset 4 bit 30 */
	bool unusedBit_10_30 : 1 {};
	/**
	offset 4 bit 31 */
	bool unusedBit_10_31 : 1 {};
	/**
	 * AC latest activity
	 * offset 8
	 */
	int latest_usage_ac_control = (int)0;
	/**
	 * offset 12
	 */
	int acSwitchLastChangeTimeMs = (int)0;
};
static_assert(sizeof(ac_control_s) == 16);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/actuators/ac_control.txt Wed Jan 03 20:30:10 UTC 2024
