// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_central.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of trigger_central_s
struct trigger_central_s {
	/**
	 * Hardware events since boot
	 * offset 0
	 */
	uint16_t hwEventCounters[HW_EVENT_TYPES] = {};
	/**
	 * offset 8
	 */
	uint16_t vvtEventRiseCounter[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 16
	 */
	uint16_t vvtEventFallCounter[CAM_INPUTS_COUNT] = {};
	/**
	 * Sync: total cams front counter
	 * offset 24
	 */
	uint16_t vvtCamCounter = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 26
	 */
	uint8_t alignmentFill_at_26[2] = {};
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
	 * offset 38
	 */
	uint8_t mapVvt_min_point_counter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 39
	 */
	uint8_t alignmentFill_at_39[1] = {};
	/**
	 * offset 40
	 */
	uint32_t temp_mapVvt_index = (uint32_t)0;
	/**
	 * offset 44
	 */
	float mapVvt_CycleDelta = (float)0;
	/**
	 * Sync: Engine Phase
	 * units: deg
	 * offset 48
	 */
	float currentEngineDecodedPhase = (float)0;
	/**
	 * Sync: trigger angle error
	 * units: deg
	 * offset 52
	 */
	float triggerToothAngleError = (float)0;
	/**
	 * offset 56
	 */
	uint8_t triggerIgnoredToothCount = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 57
	 */
	uint8_t alignmentFill_at_57[3] = {};
	/**
	 * Sync: MAP: prev angle
	 * units: deg
	 * offset 60
	 */
	angle_t mapCamPrevToothAngle = (angle_t)0;
	/**
	offset 64 bit 0 */
	bool isDecodingMapCam : 1 {};
	/**
	offset 64 bit 1 */
	bool unusedBit_19_1 : 1 {};
	/**
	offset 64 bit 2 */
	bool unusedBit_19_2 : 1 {};
	/**
	offset 64 bit 3 */
	bool unusedBit_19_3 : 1 {};
	/**
	offset 64 bit 4 */
	bool unusedBit_19_4 : 1 {};
	/**
	offset 64 bit 5 */
	bool unusedBit_19_5 : 1 {};
	/**
	offset 64 bit 6 */
	bool unusedBit_19_6 : 1 {};
	/**
	offset 64 bit 7 */
	bool unusedBit_19_7 : 1 {};
	/**
	offset 64 bit 8 */
	bool unusedBit_19_8 : 1 {};
	/**
	offset 64 bit 9 */
	bool unusedBit_19_9 : 1 {};
	/**
	offset 64 bit 10 */
	bool unusedBit_19_10 : 1 {};
	/**
	offset 64 bit 11 */
	bool unusedBit_19_11 : 1 {};
	/**
	offset 64 bit 12 */
	bool unusedBit_19_12 : 1 {};
	/**
	offset 64 bit 13 */
	bool unusedBit_19_13 : 1 {};
	/**
	offset 64 bit 14 */
	bool unusedBit_19_14 : 1 {};
	/**
	offset 64 bit 15 */
	bool unusedBit_19_15 : 1 {};
	/**
	offset 64 bit 16 */
	bool unusedBit_19_16 : 1 {};
	/**
	offset 64 bit 17 */
	bool unusedBit_19_17 : 1 {};
	/**
	offset 64 bit 18 */
	bool unusedBit_19_18 : 1 {};
	/**
	offset 64 bit 19 */
	bool unusedBit_19_19 : 1 {};
	/**
	offset 64 bit 20 */
	bool unusedBit_19_20 : 1 {};
	/**
	offset 64 bit 21 */
	bool unusedBit_19_21 : 1 {};
	/**
	offset 64 bit 22 */
	bool unusedBit_19_22 : 1 {};
	/**
	offset 64 bit 23 */
	bool unusedBit_19_23 : 1 {};
	/**
	offset 64 bit 24 */
	bool unusedBit_19_24 : 1 {};
	/**
	offset 64 bit 25 */
	bool unusedBit_19_25 : 1 {};
	/**
	offset 64 bit 26 */
	bool unusedBit_19_26 : 1 {};
	/**
	offset 64 bit 27 */
	bool unusedBit_19_27 : 1 {};
	/**
	offset 64 bit 28 */
	bool unusedBit_19_28 : 1 {};
	/**
	offset 64 bit 29 */
	bool unusedBit_19_29 : 1 {};
	/**
	offset 64 bit 30 */
	bool unusedBit_19_30 : 1 {};
	/**
	offset 64 bit 31 */
	bool unusedBit_19_31 : 1 {};
	/**
	 * offset 68
	 */
	uint32_t triggerElapsedUs = (uint32_t)0;
};
static_assert(sizeof(trigger_central_s) == 72);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_central.txt
