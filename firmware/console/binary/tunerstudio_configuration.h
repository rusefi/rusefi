/*
 * @file	tunerstudio_configuration.h
 * @brief	Tuner Studio connectivity configuration
 *
 * In this file the configuration of TunerStudio is defined
 *
 * @date Oct 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef TUNERSTUDIO_CONFIGURATION_H_
#define TUNERSTUDIO_CONFIGURATION_H_

#include "rusefi_types.h"

/**
 * this is used to confirm that firmware and TunerStudio are using the same rusefi.ini version
 */
#define TS_FILE_VERSION 20150314

#define PAGE_COUNT 2

#define PAGE_0_SIZE 14008
#define PAGE_1_SIZE 1152

#define TS_OUTPUT_SIZE 196

typedef struct {
	uint16_t values[EGT_CHANNEL_COUNT];
} egt_values_s;

/**
 * please be aware that 'float' (F32) type requires TunerStudio version 2.6 and later
 */
typedef struct {
	// primary instrument cluster gauges
	int rpm; // size 4, offset 0
	float coolant_temperature; // size 4, offset 4
	float intakeAirTemperature; // size 4, offset 8
	float throttlePositon; // size 4, offset 12
	float massAirFlowVoltage; // size 4, offset 16
	float airFuelRatio; // size 4, offset 20
	float engineLoad; // size 4, offset 24
	float v_batt; // size 4, offset 28
	short int tpsADC; // size 2, offset 32
	short int alignment; // size 2, offset 34
	float atmospherePressure; // size 4, offset 36
	float manifold_air_pressure; // size 4, offset 40
	float crankingFuelMs;
	/**
	 * This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	 */
	float baseFuel; // 48
	float tCharge; // 52
	float inj_adv; // 56
	float sparkDwell; // 60
	/**
	 * this one contains fuel with all corrections. See also baseFuel
	 */
	float pulseWidthMs; // 64
	float warmUpEnrich;	// 68
	/**
	 * Yes, I do not really enjoy packing bits into integers but we simply have too many boolean flags and I cannot
	 * water 4 bytes per traffic - I want gauges to work as fast as possible
	 */
	unsigned int hasSdCard : 1; // bit 0
	unsigned int isIgnitionEnabled : 1; // bit 1
	unsigned int injection_enabled : 1; // bit 2
	unsigned int cylinder_cleanup_enabled : 1; // bit 3
	unsigned int cylinder_cleanup_activated : 1; // bit 4
	unsigned int isFuelPumpOn : 1; // bit 5
	unsigned int isFanOn : 1; // bit 6
	unsigned int isO2HeaterOn : 1; // bit 7
	unsigned int checkEngine : 1; // bit 8
	unsigned int needBurn : 1; // bit 9
	unsigned int secondTriggerChannelEnabled : 1; // bit 10
	unsigned int clutchUpState : 1; // bit 11
	unsigned int clutchDownState : 1; // bit 12
	unsigned int knockEverIndicator : 1; // bit 13
	unsigned int knockNowIndicator : 1; // bit 14
	float vehicleSpeedKph;
	unsigned int isTpsError : 1; // bit 0
	unsigned int isCltError : 1; // bit 1
	unsigned int isMapError : 1; // bit 2
	unsigned int isIatError : 1; // bit 3
	unsigned int isAcSwitchEngaged : 1; // bit 4
	int tsConfigVersion;
	egt_values_s egtValues;
	float rpmAcceleration;
	float massAirFlowValue;
	float veValue;
	float maxDelta;
	float minDelta;
	int unused3[18];
} TunerStudioOutputChannels;

#endif /* TUNERSTUDIO_CONFIGURATION_H_ */
