// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_central.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
#define HW_EVENT_TYPES 4
// start of trigger_central_s
struct trigger_central_s {
	/**
	 * Hardware events since boot
	 * offset 0
	 */
	uint16_t hwEventCounters[HW_EVENT_TYPES];
	/**
	 * Sync: total cams front counter
	 * offset 8
	 */
	uint16_t vvtCamCounter = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2];
	/**
	 * InstantMAP at readout angle
	 * units: kPa
	 * offset 12
	 */
	float mapVvt_MAP_AT_SPECIAL_POINT = (float)0;
	/**
	 * InstantMAP delta @ readout angles
	 * units: kPa
	 * offset 16
	 */
	float mapVvt_MAP_AT_DIFF = (float)0;
	/**
	 * units: distance
	 * offset 20
	 */
	int8_t mapVvt_MAP_AT_CYCLE_COUNT = (int8_t)0;
	/**
	 * Instant MAP sync counter
	 * units: counter
	 * offset 21
	 */
	uint8_t mapVvt_sync_counter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 22
	 */
	uint8_t alignmentFill_at_22[2];
	/**
	 * Sync: Engine Phase
	 * units: deg
	 * offset 24
	 */
	float currentEngineDecodedPhase = (float)0;
	/**
	 * Sync: trigger angle error
	 * units: deg
	 * offset 28
	 */
	float triggerToothAngleError = (float)0;
	/**
	 * offset 32
	 */
	uint8_t triggerIgnoredToothCount = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 33
	 */
	uint8_t alignmentFill_at_33[3];
	/**
	 * Sync: MAP: prev angle
	 * units: deg
	 * offset 36
	 */
	angle_t mapCamPrevToothAngle = (angle_t)0;
	/**
	offset 40 bit 0 */
	bool isDecodingMapCam : 1 {};
	/**
	offset 40 bit 1 */
	bool unusedBit_14_1 : 1 {};
	/**
	offset 40 bit 2 */
	bool unusedBit_14_2 : 1 {};
	/**
	offset 40 bit 3 */
	bool unusedBit_14_3 : 1 {};
	/**
	offset 40 bit 4 */
	bool unusedBit_14_4 : 1 {};
	/**
	offset 40 bit 5 */
	bool unusedBit_14_5 : 1 {};
	/**
	offset 40 bit 6 */
	bool unusedBit_14_6 : 1 {};
	/**
	offset 40 bit 7 */
	bool unusedBit_14_7 : 1 {};
	/**
	offset 40 bit 8 */
	bool unusedBit_14_8 : 1 {};
	/**
	offset 40 bit 9 */
	bool unusedBit_14_9 : 1 {};
	/**
	offset 40 bit 10 */
	bool unusedBit_14_10 : 1 {};
	/**
	offset 40 bit 11 */
	bool unusedBit_14_11 : 1 {};
	/**
	offset 40 bit 12 */
	bool unusedBit_14_12 : 1 {};
	/**
	offset 40 bit 13 */
	bool unusedBit_14_13 : 1 {};
	/**
	offset 40 bit 14 */
	bool unusedBit_14_14 : 1 {};
	/**
	offset 40 bit 15 */
	bool unusedBit_14_15 : 1 {};
	/**
	offset 40 bit 16 */
	bool unusedBit_14_16 : 1 {};
	/**
	offset 40 bit 17 */
	bool unusedBit_14_17 : 1 {};
	/**
	offset 40 bit 18 */
	bool unusedBit_14_18 : 1 {};
	/**
	offset 40 bit 19 */
	bool unusedBit_14_19 : 1 {};
	/**
	offset 40 bit 20 */
	bool unusedBit_14_20 : 1 {};
	/**
	offset 40 bit 21 */
	bool unusedBit_14_21 : 1 {};
	/**
	offset 40 bit 22 */
	bool unusedBit_14_22 : 1 {};
	/**
	offset 40 bit 23 */
	bool unusedBit_14_23 : 1 {};
	/**
	offset 40 bit 24 */
	bool unusedBit_14_24 : 1 {};
	/**
	offset 40 bit 25 */
	bool unusedBit_14_25 : 1 {};
	/**
	offset 40 bit 26 */
	bool unusedBit_14_26 : 1 {};
	/**
	offset 40 bit 27 */
	bool unusedBit_14_27 : 1 {};
	/**
	offset 40 bit 28 */
	bool unusedBit_14_28 : 1 {};
	/**
	offset 40 bit 29 */
	bool unusedBit_14_29 : 1 {};
	/**
	offset 40 bit 30 */
	bool unusedBit_14_30 : 1 {};
	/**
	offset 40 bit 31 */
	bool unusedBit_14_31 : 1 {};
	/**
	 * offset 44
	 */
	uint32_t triggerElapsedUs = (uint32_t)0;
};
static_assert(sizeof(trigger_central_s) == 48);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_central.txt
