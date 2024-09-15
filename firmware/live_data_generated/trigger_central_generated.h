// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_central.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
#define HW_EVENT_TYPES 6
// start of trigger_central_s
struct trigger_central_s {
	/**
	 * Hardware events since boot
	 * offset 0
	 */
	uint32_t hwEventCounters[HW_EVENT_TYPES];
	/**
	 * Sync: total cams front counter
	 * offset 24
	 */
	uint32_t vvtCamCounter = (uint32_t)0;
	/**
	 * InstantMAP at readout angle
	 * units: kPa
	 * offset 28
	 */
	float mapVvt_MAP_AT_SPECIAL_POINT = (float)0;
	/**
	 * InstantMAP delta @ readout angles
	 * units: kPa
	 * offset 32
	 */
	float mapVvt_MAP_AT_DIFF = (float)0;
	/**
	 * units: distance
	 * offset 36
	 */
	int8_t mapVvt_MAP_AT_CYCLE_COUNT = (int8_t)0;
	/**
	 * Instant MAP sync counter
	 * units: counter
	 * offset 37
	 */
	uint8_t mapVvt_sync_counter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 38
	 */
	uint8_t alignmentFill_at_38[2];
	/**
	 * Sync: Engine Phase
	 * units: deg
	 * offset 40
	 */
	float currentEngineDecodedPhase = (float)0;
	/**
	 * Sync: trigger angle error
	 * units: deg
	 * offset 44
	 */
	float triggerToothAngleError = (float)0;
	/**
	 * offset 48
	 */
	uint8_t triggerIgnoredToothCount = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 49
	 */
	uint8_t alignmentFill_at_49[3];
	/**
	 * Sync: MAP: prev angle
	 * units: deg
	 * offset 52
	 */
	angle_t mapCamPrevToothAngle = (angle_t)0;
	/**
	offset 56 bit 0 */
	bool isDecodingMapCam : 1 {};
	/**
	offset 56 bit 1 */
	bool unusedBit_13_1 : 1 {};
	/**
	offset 56 bit 2 */
	bool unusedBit_13_2 : 1 {};
	/**
	offset 56 bit 3 */
	bool unusedBit_13_3 : 1 {};
	/**
	offset 56 bit 4 */
	bool unusedBit_13_4 : 1 {};
	/**
	offset 56 bit 5 */
	bool unusedBit_13_5 : 1 {};
	/**
	offset 56 bit 6 */
	bool unusedBit_13_6 : 1 {};
	/**
	offset 56 bit 7 */
	bool unusedBit_13_7 : 1 {};
	/**
	offset 56 bit 8 */
	bool unusedBit_13_8 : 1 {};
	/**
	offset 56 bit 9 */
	bool unusedBit_13_9 : 1 {};
	/**
	offset 56 bit 10 */
	bool unusedBit_13_10 : 1 {};
	/**
	offset 56 bit 11 */
	bool unusedBit_13_11 : 1 {};
	/**
	offset 56 bit 12 */
	bool unusedBit_13_12 : 1 {};
	/**
	offset 56 bit 13 */
	bool unusedBit_13_13 : 1 {};
	/**
	offset 56 bit 14 */
	bool unusedBit_13_14 : 1 {};
	/**
	offset 56 bit 15 */
	bool unusedBit_13_15 : 1 {};
	/**
	offset 56 bit 16 */
	bool unusedBit_13_16 : 1 {};
	/**
	offset 56 bit 17 */
	bool unusedBit_13_17 : 1 {};
	/**
	offset 56 bit 18 */
	bool unusedBit_13_18 : 1 {};
	/**
	offset 56 bit 19 */
	bool unusedBit_13_19 : 1 {};
	/**
	offset 56 bit 20 */
	bool unusedBit_13_20 : 1 {};
	/**
	offset 56 bit 21 */
	bool unusedBit_13_21 : 1 {};
	/**
	offset 56 bit 22 */
	bool unusedBit_13_22 : 1 {};
	/**
	offset 56 bit 23 */
	bool unusedBit_13_23 : 1 {};
	/**
	offset 56 bit 24 */
	bool unusedBit_13_24 : 1 {};
	/**
	offset 56 bit 25 */
	bool unusedBit_13_25 : 1 {};
	/**
	offset 56 bit 26 */
	bool unusedBit_13_26 : 1 {};
	/**
	offset 56 bit 27 */
	bool unusedBit_13_27 : 1 {};
	/**
	offset 56 bit 28 */
	bool unusedBit_13_28 : 1 {};
	/**
	offset 56 bit 29 */
	bool unusedBit_13_29 : 1 {};
	/**
	offset 56 bit 30 */
	bool unusedBit_13_30 : 1 {};
	/**
	offset 56 bit 31 */
	bool unusedBit_13_31 : 1 {};
	/**
	 * offset 60
	 */
	uint32_t triggerElapsedUs = (uint32_t)0;
};
static_assert(sizeof(trigger_central_s) == 64);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_central.txt
