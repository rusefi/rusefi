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
	 * PWM mode active
	offset 0 bit 10 */
	bool pwmActive : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_11_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_11_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_11_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_11_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_11_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_11_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_11_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_11_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_11_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_11_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_11_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_11_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_11_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_11_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_11_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_11_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_11_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_11_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_11_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_11_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_11_31 : 1 {};
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
};
static_assert(sizeof(fan_control_s) == 20);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/modules/fan_control/fan_control.txt
