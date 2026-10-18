// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/modules/fan_control/fan_control.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of fan_control_s
struct fan_control_s {
	/**
	offset 0 bit 0 */
	bool cranking : 1 {};
	/**
	offset 0 bit 1 */
	bool notRunning : 1 {};
	/**
	 * Engine stopped
	offset 0 bit 2 */
	bool disabledWhileEngineStopped : 1 {};
	/**
	 * Broken CLT
	offset 0 bit 3 */
	bool brokenClt : 1 {};
	/**
	 * Enable for AC
	offset 0 bit 4 */
	bool enabledForAc : 1 {};
	/**
	 * Enable for AC pressure
	offset 0 bit 5 */
	bool enabledForAcPressure : 1 {};
	/**
	 * Above hot threshold
	offset 0 bit 6 */
	bool hot : 1 {};
	/**
	 * Below cold threshold
	offset 0 bit 7 */
	bool cold : 1 {};
	/**
	offset 0 bit 8 */
	bool disabledBySpeed : 1 {};
	/**
	 * On
	offset 0 bit 9 */
	bool m_state : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_10_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_10_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_10_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_10_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_10_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_10_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_10_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_10_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_10_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_10_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_10_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_10_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_10_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_10_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_10_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_10_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_10_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_10_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_10_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_10_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_10_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_10_31 : 1 {};
	/**
	 * offset 4
	 */
	uint8_t radiatorFanStatus = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3] = {};
	/**
	 * Fan speed target
	 * 0% = fan off, 100% = full fan. Pulled from the PWM curve plus any A/C boost or inhibit override, before soft-start ramping.
	 * offset 8
	 */
	float fanSpeedTarget = (float)0;
	/**
	 * Fan speed applied
	 * 0% = fan off, 100% = full fan. Same as Fan speed target but after soft-start ramping - commanded demand, not a measured fan speed.
	 * offset 12
	 */
	float fanSpeedApplied = (float)0;
	/**
	 * PWM duty applied
	 * The raw PWM duty cycle sent to the pin, after off/full-speed duty mapping. May be inverted vs fan speed (min/max can be set either way to match hardware).
	 * offset 16
	 */
	float pwmAppliedPwm = (float)0;
	/**
	 * PWM mode active
	offset 20 bit 0 */
	bool pwmActive : 1 {};
	/**
	offset 20 bit 1 */
	bool unusedBit_38_1 : 1 {};
	/**
	offset 20 bit 2 */
	bool unusedBit_38_2 : 1 {};
	/**
	offset 20 bit 3 */
	bool unusedBit_38_3 : 1 {};
	/**
	offset 20 bit 4 */
	bool unusedBit_38_4 : 1 {};
	/**
	offset 20 bit 5 */
	bool unusedBit_38_5 : 1 {};
	/**
	offset 20 bit 6 */
	bool unusedBit_38_6 : 1 {};
	/**
	offset 20 bit 7 */
	bool unusedBit_38_7 : 1 {};
	/**
	offset 20 bit 8 */
	bool unusedBit_38_8 : 1 {};
	/**
	offset 20 bit 9 */
	bool unusedBit_38_9 : 1 {};
	/**
	offset 20 bit 10 */
	bool unusedBit_38_10 : 1 {};
	/**
	offset 20 bit 11 */
	bool unusedBit_38_11 : 1 {};
	/**
	offset 20 bit 12 */
	bool unusedBit_38_12 : 1 {};
	/**
	offset 20 bit 13 */
	bool unusedBit_38_13 : 1 {};
	/**
	offset 20 bit 14 */
	bool unusedBit_38_14 : 1 {};
	/**
	offset 20 bit 15 */
	bool unusedBit_38_15 : 1 {};
	/**
	offset 20 bit 16 */
	bool unusedBit_38_16 : 1 {};
	/**
	offset 20 bit 17 */
	bool unusedBit_38_17 : 1 {};
	/**
	offset 20 bit 18 */
	bool unusedBit_38_18 : 1 {};
	/**
	offset 20 bit 19 */
	bool unusedBit_38_19 : 1 {};
	/**
	offset 20 bit 20 */
	bool unusedBit_38_20 : 1 {};
	/**
	offset 20 bit 21 */
	bool unusedBit_38_21 : 1 {};
	/**
	offset 20 bit 22 */
	bool unusedBit_38_22 : 1 {};
	/**
	offset 20 bit 23 */
	bool unusedBit_38_23 : 1 {};
	/**
	offset 20 bit 24 */
	bool unusedBit_38_24 : 1 {};
	/**
	offset 20 bit 25 */
	bool unusedBit_38_25 : 1 {};
	/**
	offset 20 bit 26 */
	bool unusedBit_38_26 : 1 {};
	/**
	offset 20 bit 27 */
	bool unusedBit_38_27 : 1 {};
	/**
	offset 20 bit 28 */
	bool unusedBit_38_28 : 1 {};
	/**
	offset 20 bit 29 */
	bool unusedBit_38_29 : 1 {};
	/**
	offset 20 bit 30 */
	bool unusedBit_38_30 : 1 {};
	/**
	offset 20 bit 31 */
	bool unusedBit_38_31 : 1 {};
};
static_assert(sizeof(fan_control_s) == 24);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/modules/fan_control/fan_control.txt
