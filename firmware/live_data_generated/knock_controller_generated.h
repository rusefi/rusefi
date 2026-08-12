// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of knock_controller_s
struct knock_controller_s {
	/**
	 * @@GAUGE_NAME_KNOCK_LEVEL@@
	 * units: Volts
	 * offset 0
	 */
	float m_knockLevel = (float)0;
	/**
	 * Knock: Cyl
	 * units: dBv
	 * offset 4
	 */
	int8_t m_knockCyl[MAX_CYLINDER_COUNT] = {};
	/**
	 * @@GAUGE_NAME_KNOCK_RETARD@@
	 * units: deg
	 * offset 16
	 */
	angle_t m_knockRetard = (angle_t)0;
	/**
	 * Knock: Threshold
	 * offset 20
	 */
	float m_knockThreshold = (float)0;
	/**
	 * @@GAUGE_NAME_KNOCK_COUNTER@@
	 * offset 24
	 */
	uint32_t m_knockCount = (uint32_t)0;
	/**
	 * Knock: Max retard
	 * offset 28
	 */
	float m_maximumRetard = (float)0;
	/**
	 * Knock: SpectrumData
	 * units: compressed data
	 * offset 32
	 */
	uint32_t m_knockSpectrum[16] = {};
	/**
	 * Knock: ChannelAndCylNumber
	 * units: compressed N + N
	 * offset 96
	 */
	uint16_t m_knockSpectrumChannelCyl = (uint16_t)0;
	/**
	 * Knock: Start Freq
	 * units: Hz
	 * offset 98
	 */
	uint16_t m_knockFrequencyStart = (uint16_t)0;
	/**
	 * Knock: Step Freq
	 * units: Hz
	 * offset 100
	 */
	float m_knockFrequencyStep = (float)0;
	/**
	 * Knock: Fuel trim when knock
	 * units: multiplier
	 * offset 104
	 */
	float m_knockFuelTrimMultiplier = (float)0;
	/**
	 * Knock: Detected recently
	offset 108 bit 0 */
	bool hasKnockRecently : 1 {};
	/**
	 * Knock: Retard active
	offset 108 bit 1 */
	bool hasKnockRetardNow : 1 {};
	/**
	offset 108 bit 2 */
	bool unusedBit_13_2 : 1 {};
	/**
	offset 108 bit 3 */
	bool unusedBit_13_3 : 1 {};
	/**
	offset 108 bit 4 */
	bool unusedBit_13_4 : 1 {};
	/**
	offset 108 bit 5 */
	bool unusedBit_13_5 : 1 {};
	/**
	offset 108 bit 6 */
	bool unusedBit_13_6 : 1 {};
	/**
	offset 108 bit 7 */
	bool unusedBit_13_7 : 1 {};
	/**
	offset 108 bit 8 */
	bool unusedBit_13_8 : 1 {};
	/**
	offset 108 bit 9 */
	bool unusedBit_13_9 : 1 {};
	/**
	offset 108 bit 10 */
	bool unusedBit_13_10 : 1 {};
	/**
	offset 108 bit 11 */
	bool unusedBit_13_11 : 1 {};
	/**
	offset 108 bit 12 */
	bool unusedBit_13_12 : 1 {};
	/**
	offset 108 bit 13 */
	bool unusedBit_13_13 : 1 {};
	/**
	offset 108 bit 14 */
	bool unusedBit_13_14 : 1 {};
	/**
	offset 108 bit 15 */
	bool unusedBit_13_15 : 1 {};
	/**
	offset 108 bit 16 */
	bool unusedBit_13_16 : 1 {};
	/**
	offset 108 bit 17 */
	bool unusedBit_13_17 : 1 {};
	/**
	offset 108 bit 18 */
	bool unusedBit_13_18 : 1 {};
	/**
	offset 108 bit 19 */
	bool unusedBit_13_19 : 1 {};
	/**
	offset 108 bit 20 */
	bool unusedBit_13_20 : 1 {};
	/**
	offset 108 bit 21 */
	bool unusedBit_13_21 : 1 {};
	/**
	offset 108 bit 22 */
	bool unusedBit_13_22 : 1 {};
	/**
	offset 108 bit 23 */
	bool unusedBit_13_23 : 1 {};
	/**
	offset 108 bit 24 */
	bool unusedBit_13_24 : 1 {};
	/**
	offset 108 bit 25 */
	bool unusedBit_13_25 : 1 {};
	/**
	offset 108 bit 26 */
	bool unusedBit_13_26 : 1 {};
	/**
	offset 108 bit 27 */
	bool unusedBit_13_27 : 1 {};
	/**
	offset 108 bit 28 */
	bool unusedBit_13_28 : 1 {};
	/**
	offset 108 bit 29 */
	bool unusedBit_13_29 : 1 {};
	/**
	offset 108 bit 30 */
	bool unusedBit_13_30 : 1 {};
	/**
	offset 108 bit 31 */
	bool unusedBit_13_31 : 1 {};
};
static_assert(sizeof(knock_controller_s) == 112);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt
