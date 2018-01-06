/*
 * @file	tunerstudio_configuration.h
 * @brief	Tuner Studio connectivity configuration
 *
 * In this file the configuration of TunerStudio is defined
 *
 * @date Oct 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TUNERSTUDIO_CONFIGURATION_H_
#define TUNERSTUDIO_CONFIGURATION_H_

#include "rusefi_types.h"

#define PAGE_COUNT 1

typedef struct {
	uint16_t values[EGT_CHANNEL_COUNT];
} egt_values_s;

/**
 * please be aware that 'float' (F32) type requires TunerStudio version 2.6 and later
 */
typedef struct {
	/* see also [OutputChannels] in rusefi.input */
	// primary instrument cluster gauges
	int rpm; // size 4, offset 0
	/**
	 * This value is in Celcius - UI would convert into F if needed
	 */
	float coolantTemperature; // size 4, offset 4
	float intakeAirTemperature; // size 4, offset 8
	float throttlePositon; // size 4, offset 12
	float massAirFlowVoltage; // size 4, offset 16
	float airFuelRatio; // size 4, offset 20
	float engineLoad; // size 4, offset 24
	float vBatt; // size 4, offset 28
	short int tpsADC; // size 2, offset 32
	short int alignment; // size 2, offset 34
	float baroPressure; // size 4, offset 36
	float manifoldAirPressure; // size 4, offset 40
	float crankingFuelMs; // offset 44
	/**
	 * This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	 */
	float fuelBase; // 48
	float tCharge; // 52
	float ignitionAdvance; // 56
	float sparkDwell; // 60
	/**
	 * this one contains total resulting fuel squirt time, per event
	 * With all corrections and injector lag. See also baseFuel
	 */
	float actualLastInjection; // 64
	float debugFloatField1;	// 68
	/**
	 * Yes, I do not really enjoy packing bits into integers but we simply have too many boolean flags and I cannot
	 * water 4 bytes per traffic - I want gauges to work as fast as possible
	 */
	unsigned int hasSdCard : 1; // bit 0, 72
	unsigned int isIgnitionEnabled : 1; // bit 1
	unsigned int isInjectionEnabled : 1; // bit 2
	unsigned int isCylinderCleanupEnabled : 1; // bit 3
	unsigned int isCylinderCleanupActivated : 1; // bit 4
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
	unsigned int brakePedalState : 1; // bit 15. 0 - not pressed, 1 = pressed
	unsigned int toothLogReady : 1; // bit 16
	float vehicleSpeedKph; // 76
	unsigned int isTpsError : 1; // bit 0, 80
	unsigned int isCltError : 1; // bit 1
	unsigned int isMapError : 1; // bit 2
	unsigned int isIatError : 1; // bit 3
	unsigned int isAcSwitchEngaged : 1; // bit 4
	unsigned int isTriggerError : 1; // bit 5
	unsigned int hasFatalError : 1; // bit 6
	unsigned int isWarnNow : 1; // bit 7
	unsigned int isCltBroken : 1; // bit 8
	int tsConfigVersion; // 84
	egt_values_s egtValues; // 88
	float unusedOffset104; // 104
	float massAirFlow; // 108
	/**
	 * Current volumetric efficiency
	 */
	float veValue; // offset 112
	/**
	 * TPS value delta within specified number of cycles
	 * See tpsAccelFuel
	 */
	float deltaTps; // offset 116
	int triggerErrorsCounter; // offset 120
	/**
	 * Engine load delta
	 */
	float engineLoadAccelExtra; // offset 124
	float tpsAccelFuel; // offset 128
	float baroCorrection; // 132
	float pedalPosition; // 138
	/**
	 * @see coilDutyCycle
	 */
	float injectorDutyCycle; // 140
	int knockCount; // 144
	float fuelTankGauge; // 148
	float knockLevel; // 152
	int totalTriggerErrorCounter; // 156
	float wallFuelAmount; // 160
	/**
	 * multiplier, 1 means no correction, 1.20 means 20% extra
	 */
	float iatCorrection; // 164
	floatms_t wallFuelCorrection; // 168
	float idlePosition; // 172
	float currentTargetAfr; // 176
	float chargeAirMass; // 180
	/**
	 * multiplier, 1 means no correction, 1.20 means 20% extra
	 */
	float cltCorrection; // 184
	/**
	 * Total fuel with CLT, IAT and TPS acceleration without injector lag corrections per cycle,
	 * as squirt duration.
	 *
	 * @see actualLastInjection
	 */
	float fuelRunning; // 188
	int debugIntField1;	// 192
	float injectorLagMs; // 196
	float debugFloatField2; // 200
	float debugFloatField3; // 204
	float debugFloatField4; // 208
	float debugFloatField5; // 212
	int debugIntField2; // 216
	int debugIntField3; // 220
	int timeSeconds; // 224
	float engineLoadDelta; // 228
	float speedToRpmRatio; // 232
	int warningCounter; // 236
	int lastErrorCode; // 240
	/**
	 * Microcontroller own internal temperature, C
	 */
	float internalMcuTemperature; // 244
	float vvtPosition; // 248
	int engineMode; // 252
	float debugFloatField6; // 256
	float debugFloatField7; // 260
	int firmwareVersion; // 264
	float fuelPidCorrection; // 268
	/**
	 * @see injectorDutyCycle
	 */
	float coilDutyCycle; // 272
	int16_t accelerationX; // 276
	int16_t accelerationY; // 278
	float oilPressure;	// 280
	float fuelConsumptionPerHour; // 284
	int unused3[17];
	/* see also [OutputChannels] in rusefi.input */
} TunerStudioOutputChannels;

#endif /* TUNERSTUDIO_CONFIGURATION_H_ */
