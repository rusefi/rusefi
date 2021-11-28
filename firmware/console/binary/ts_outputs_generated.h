// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Sun Nov 28 11:42:10 PST 2021
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of ts_outputs_s
struct ts_outputs_s {
	/**
	offset 0 bit 0 */
	bool sd_present : 1 {};
	/**
	offset 0 bit 1 */
	bool isIgnitionEnabledIndicator : 1 {};
	/**
	offset 0 bit 2 */
	bool isInjectionEnabledIndicator : 1 {};
	/**
	offset 0 bit 3 */
	bool sd_logging_internal : 1 {};
	/**
	offset 0 bit 4 */
	bool isCylinderCleanupActivated : 1 {};
	/**
	offset 0 bit 5 */
	bool isFuelPumpOn : 1 {};
	/**
	offset 0 bit 6 */
	bool isFanOn : 1 {};
	/**
	offset 0 bit 7 */
	bool isO2HeaterOn : 1 {};
	/**
	offset 0 bit 8 */
	bool checkEngine : 1 {};
	/**
	offset 0 bit 9 */
	bool needBurn : 1 {};
	/**
	offset 0 bit 10 */
	bool sd_msd : 1 {};
	/**
	offset 0 bit 11 */
	bool clutchUpState : 1 {};
	/**
	offset 0 bit 12 */
	bool clutchDownState : 1 {};
	/**
	offset 0 bit 13 */
	bool isFan2On : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedb14 : 1 {};
	/**
	offset 0 bit 15 */
	bool brakePedalState : 1 {};
	/**
	offset 0 bit 16 */
	bool toothLogReady : 1 {};
	/**
	offset 0 bit 17 */
	bool acSwitchState : 1 {};
	/**
	offset 0 bit 18 */
	bool isTpsError : 1 {};
	/**
	offset 0 bit 19 */
	bool isCltError : 1 {};
	/**
	offset 0 bit 20 */
	bool isMapError : 1 {};
	/**
	offset 0 bit 21 */
	bool isIatError : 1 {};
	/**
	offset 0 bit 22 */
	bool acState : 1 {};
	/**
	offset 0 bit 23 */
	bool isTriggerError : 1 {};
	/**
	offset 0 bit 24 */
	bool hasCriticalError : 1 {};
	/**
	offset 0 bit 25 */
	bool isWarnNow : 1 {};
	/**
	offset 0 bit 26 */
	bool isPedalError : 1 {};
	/**
	offset 0 bit 27 */
	bool isKnockChipOk : 1 {};
	/**
	offset 0 bit 28 */
	bool launchTriggered : 1 {};
	/**
	offset 0 bit 29 */
	bool isTps2Error : 1 {};
	/**
	offset 0 bit 30 */
	bool isIdleClosedLoop : 1 {};
	/**
	offset 0 bit 31 */
	bool isIdleCoasting : 1 {};
	/**
	RPM
	 * offset 4
	 */
	scaled_channel<uint16_t, 1, 1> RPMValue = (uint16_t)0;
	/**
	RPM/s
	 * offset 6
	 */
	uint16_t rpmAcceleration = (uint16_t)0;
	/**
	value
	 * offset 8
	 */
	scaled_channel<uint16_t, 100, 1> speedToRpmRatio = (uint16_t)0;
	/**
	kph
	 * offset 10
	 */
	scaled_channel<uint8_t, 1, 1> vehicleSpeedKph = (uint8_t)0;
	/**
	deg C
	 * offset 11
	 */
	scaled_channel<uint8_t, 1, 1> internalMcuTemperature = (uint8_t)0;
	/**
	deg C
	 * offset 12
	 */
	scaled_channel<uint16_t, 100, 1> coolant = (uint16_t)0;
	/**
	deg C
	 * offset 14
	 */
	scaled_channel<uint16_t, 100, 1> intake = (uint16_t)0;
	/**
	deg C
	 * offset 16
	 */
	scaled_channel<uint16_t, 100, 1> auxTemp1 = (uint16_t)0;
	/**
	deg C
	 * offset 18
	 */
	scaled_channel<uint16_t, 100, 1> auxTemp2 = (uint16_t)0;
	/**
	%
	 * offset 20
	 */
	scaled_channel<uint16_t, 100, 1> TPSValue = (uint16_t)0;
	/**
	%
	 * offset 22
	 */
	scaled_channel<uint16_t, 100, 1> throttlePedalPosition = (uint16_t)0;
	/**
	ADC
	 * offset 24
	 */
	uint16_t tpsADC = (uint16_t)0;
	/**
	V
	 * offset 26
	 */
	scaled_channel<uint16_t, 1000, 1> MAFValue = (uint16_t)0;
	/**
	Kg/h
	 * offset 28
	 */
	scaled_channel<uint16_t, 10, 1> massAirFlowValue = (uint16_t)0;
	/**
	kPa
	 * offset 30
	 */
	scaled_channel<uint16_t, 30, 1> MAPValue = (uint16_t)0;
	/**
	kPa
	 * offset 32
	 */
	scaled_channel<uint16_t, 30, 1> baroPressure = (uint16_t)0;
	/**
	 * offset 34
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue = (uint16_t)0;
	/**
	deg
	 * offset 36
	 */
	scaled_channel<uint8_t, 10, 1> knockRetard = (uint8_t)0;
	/**
	 * offset 37
	 */
	uint8_t unused37 = (uint8_t)0;
	/**
	V
	 * offset 38
	 */
	scaled_channel<uint16_t, 1000, 1> VBatt = (uint16_t)0;
	/**
	kPa
	 * offset 40
	 */
	scaled_channel<uint16_t, 30, 1> oilPressure = (uint16_t)0;
	/**
	deg
	 * offset 42
	 */
	scaled_channel<uint16_t, 50, 1> vvtPositionB1I = (uint16_t)0;
	/**
	g
	 * offset 44
	 */
	scaled_channel<uint16_t, 1000, 1> chargeAirMass = (uint16_t)0;
	/**
	 * airmass in mg, 0-65 grams
	ms
	 * offset 46
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelMs = (uint16_t)0;
	/**
	ratio
	 * offset 48
	 */
	scaled_channel<uint16_t, 1000, 1> currentTargetAfr = (uint16_t)0;
	/**
	 * This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	mg
	 * offset 50
	 */
	scaled_channel<uint16_t, 100, 1> baseFuel = (uint16_t)0;
	/**
	 * Total fuel with CLT IAT and TPS acceleration without injector lag corrections per cycle, as pulse per cycle
	mg
	 * offset 52
	 */
	scaled_channel<uint16_t, 100, 1> fuelRunning = (uint16_t)0;
	/**
	 * Actual last injection time - including all compensation and injection mode
	ms
	 * offset 54
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjection = (uint16_t)0;
	/**
	%
	 * offset 56
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycle = (uint8_t)0;
	/**
	ratio
	 * offset 57
	 */
	scaled_channel<uint8_t, 2, 1> veValue = (uint8_t)0;
	/**
	deg
	 * offset 58
	 */
	scaled_channel<uint16_t, 50, 1> injectionOffset = (uint16_t)0;
	/**
	deg C
	 * offset 60
	 */
	scaled_channel<uint16_t, 100, 1> tCharge = (uint16_t)0;
	/**
	ms
	 * offset 62
	 */
	scaled_channel<uint16_t, 300, 1> injectorLagMs = (uint16_t)0;
	/**
	%
	 * offset 64
	 */
	scaled_channel<uint16_t, 100, 1> iatCorrection = (uint16_t)0;
	/**
	%
	 * offset 66
	 */
	scaled_channel<uint16_t, 100, 1> cltCorrection = (uint16_t)0;
	/**
	%
	 * offset 68
	 */
	scaled_channel<uint16_t, 100, 1> baroCorrection = (uint16_t)0;
	/**
	deg
	 * offset 70
	 */
	scaled_channel<uint16_t, 1, 1> currentEnginePhase = (uint16_t)0;
	/**
	mg
	 * offset 72
	 */
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;
	/**
	mg
	 * offset 74
	 */
	scaled_channel<int16_t, 100, 1> wallFuelCorrection = (int16_t)0;
	/**
	 * offset 76
	 */
	uint16_t unused76 = (uint16_t)0;
	/**
	 * TPS acceleration enrichment
	ratio
	 * offset 78
	 */
	scaled_channel<uint16_t, 100, 1> deltaTps = (uint16_t)0;
	/**
	 * offset 80
	 */
	uint16_t unused80 = (uint16_t)0;
	/**
	ms
	 * offset 82
	 */
	scaled_channel<uint16_t, 300, 1> tpsAccelFuel = (uint16_t)0;
	/**
	deg
	 * offset 84
	 */
	scaled_channel<uint16_t, 50, 1> ignitionAdvance = (uint16_t)0;
	/**
	ms
	 * offset 86
	 */
	scaled_channel<uint16_t, 300, 1> sparkDwellValue = (uint16_t)0;
	/**
	%
	 * offset 88
	 */
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;
	/**
	%
	 * offset 90
	 */
	scaled_channel<uint16_t, 100, 1> idleAirValvePosition = (uint16_t)0;
	/**
	%
	 * offset 92
	 */
	scaled_channel<uint16_t, 100, 1> etbTarget = (uint16_t)0;
	/**
	%
	 * offset 94
	 */
	scaled_channel<uint16_t, 100, 1> etb1DutyCycle = (uint16_t)0;
	/**
	%
	 * offset 96
	 */
	scaled_channel<uint16_t, 100, 1> etb1Error = (uint16_t)0;
	/**
	%
	 * offset 98
	 */
	scaled_channel<uint16_t, 100, 1> fuelTankLevel = (uint16_t)0;
	/**
	grams
	 * offset 100
	 */
	scaled_channel<uint16_t, 1, 1> totalFuelConsumption = (uint16_t)0;
	/**
	gram/s
	 * offset 102
	 */
	scaled_channel<uint16_t, 200, 1> fuelFlowRate = (uint16_t)0;
	/**
	%
	 * offset 104
	 */
	scaled_channel<uint16_t, 100, 1> veTableYAxis = (uint16_t)0;
	/**
	%
	 * offset 106
	 */
	scaled_channel<uint16_t, 100, 1> afrTableYAxis = (uint16_t)0;
	/**
	Volts
	 * offset 108
	 */
	scaled_channel<float, 1, 1> knockLevel = (float)0;
	/**
	sec
	 * offset 112
	 */
	scaled_channel<uint32_t, 1, 1> seconds = (uint32_t)0;
	/**
	em
	 * offset 116
	 */
	scaled_channel<uint32_t, 1, 1> engineMode = (uint32_t)0;
	/**
	version_f
	 * offset 120
	 */
	scaled_channel<uint32_t, 1, 1> firmwareVersion = (uint32_t)0;
	/**
	 * offset 124
	 */
	scaled_channel<uint32_t, 1, 1> tsConfigVersion = (uint32_t)0;
	/**
	 * offset 128
	 */
	scaled_channel<float, 1, 1> calibrationValue = (float)0;
	/**
	 * offset 132
	 */
	scaled_channel<uint8_t, 1, 1> calibrationMode = (uint8_t)0;
	/**
	 * offset 133
	 */
	scaled_channel<uint8_t, 1, 1> padding = (uint8_t)0;
	/**
	%
	 * offset 134
	 */
	scaled_channel<uint16_t, 100, 1> fuelingLoad = (uint16_t)0;
	/**
	%
	 * offset 136
	 */
	scaled_channel<uint16_t, 100, 1> ignitionLoad = (uint16_t)0;
	/**
	crc16
	 * offset 138
	 */
	scaled_channel<uint16_t, 1, 1> engineMakeCodeNameCrc16 = (uint16_t)0;
	/**
	counter
	 * offset 140
	 */
	scaled_channel<uint32_t, 1, 1> totalTriggerErrorCounter = (uint32_t)0;
	/**
	 * offset 144
	 */
	uint32_t orderingErrorCounter = (uint32_t)0;
	/**
	count
	 * offset 148
	 */
	scaled_channel<uint16_t, 1, 1> warningCounter = (uint16_t)0;
	/**
	error
	 * offset 150
	 */
	scaled_channel<uint16_t, 1, 1> lastErrorCode = (uint16_t)0;
	/**
	error
	 * offset 152
	 */
	uint16_t recentErrorCode[8];
	/**
	val
	 * offset 168
	 */
	scaled_channel<float, 1, 1> debugFloatField1 = (float)0;
	/**
	val
	 * offset 172
	 */
	scaled_channel<float, 1, 1> debugFloatField2 = (float)0;
	/**
	val
	 * offset 176
	 */
	scaled_channel<float, 1, 1> debugFloatField3 = (float)0;
	/**
	val
	 * offset 180
	 */
	scaled_channel<float, 1, 1> debugFloatField4 = (float)0;
	/**
	val
	 * offset 184
	 */
	scaled_channel<float, 1, 1> debugFloatField5 = (float)0;
	/**
	val
	 * offset 188
	 */
	scaled_channel<float, 1, 1> debugFloatField6 = (float)0;
	/**
	val
	 * offset 192
	 */
	scaled_channel<float, 1, 1> debugFloatField7 = (float)0;
	/**
	val
	 * offset 196
	 */
	scaled_channel<uint32_t, 1, 1> debugIntField1 = (uint32_t)0;
	/**
	val
	 * offset 200
	 */
	scaled_channel<uint32_t, 1, 1> debugIntField2 = (uint32_t)0;
	/**
	val
	 * offset 204
	 */
	scaled_channel<uint32_t, 1, 1> debugIntField3 = (uint32_t)0;
	/**
	val
	 * offset 208
	 */
	scaled_channel<uint16_t, 1, 1> debugIntField4 = (uint16_t)0;
	/**
	val
	 * offset 210
	 */
	scaled_channel<uint16_t, 1, 1> debugIntField5 = (uint16_t)0;
	/**
	G
	 * offset 212
	 */
	scaled_channel<uint16_t, 100, 1> accelerationX = (uint16_t)0;
	/**
	G
	 * offset 214
	 */
	scaled_channel<uint16_t, 100, 1> accelerationY = (uint16_t)0;
	/**
	deg C
	 * offset 216
	 */
	uint16_t egt[EGT_CHANNEL_COUNT];
	/**
	%
	 * offset 232
	 */
	scaled_channel<uint16_t, 100, 1> TPS2Value = (uint16_t)0;
	/**
	V
	 * offset 234
	 */
	scaled_channel<uint16_t, 1000, 1> rawTps1Primary = (uint16_t)0;
	/**
	V
	 * offset 236
	 */
	scaled_channel<uint16_t, 1000, 1> rawPpsPrimary = (uint16_t)0;
	/**
	V
	 * offset 238
	 */
	scaled_channel<uint16_t, 1000, 1> rawClt = (uint16_t)0;
	/**
	V
	 * offset 240
	 */
	scaled_channel<uint16_t, 1000, 1> rawIat = (uint16_t)0;
	/**
	V
	 * offset 242
	 */
	scaled_channel<uint16_t, 1000, 1> rawOilPressure = (uint16_t)0;
	/**
	crc16
	 * offset 244
	 */
	scaled_channel<uint16_t, 1, 1> tuneCrc16 = (uint16_t)0;
	/**
	 * offset 246
	 */
	scaled_channel<uint8_t, 1, 1> unusedAt246 = (uint8_t)0;
	/**
	gear
	 * offset 247
	 */
	scaled_channel<uint8_t, 1, 1> tcuCurrentGear = (uint8_t)0;
	/**
	V
	 * offset 248
	 */
	scaled_channel<uint16_t, 1000, 1> rawPpsSecondary = (uint16_t)0;
	/**
	 * offset 250
	 */
	scaled_channel<uint8_t, 1, 1> knock[12];
	/**
	gear
	 * offset 262
	 */
	scaled_channel<uint8_t, 1, 1> tcuDesiredGear = (uint8_t)0;
	/**
	%
	 * offset 263
	 */
	scaled_channel<uint8_t, 2, 1> flexPercent = (uint8_t)0;
	/**
	V
	 * offset 264
	 */
	scaled_channel<uint16_t, 1000, 1> rawIdlePositionSensor = (uint16_t)0;
	/**
	V
	 * offset 266
	 */
	scaled_channel<uint16_t, 1000, 1> rawWastegatePosition = (uint16_t)0;
	/**
	%
	 * offset 268
	 */
	scaled_channel<uint16_t, 100, 1> wastegatePositionSensor = (uint16_t)0;
	/**
	%
	 * offset 270
	 */
	scaled_channel<uint16_t, 100, 1> idlePositionSensor = (uint16_t)0;
	/**
	V
	 * offset 272
	 */
	scaled_channel<uint16_t, 1000, 1> rawLowFuelPressure = (uint16_t)0;
	/**
	V
	 * offset 274
	 */
	scaled_channel<uint16_t, 1000, 1> rawHighFuelPressure = (uint16_t)0;
	/**
	kpa
	 * offset 276
	 */
	scaled_channel<uint16_t, 30, 1> lowFuelPressure = (uint16_t)0;
	/**
	bar
	 * offset 278
	 */
	scaled_channel<uint16_t, 10, 1> highFuelPressure = (uint16_t)0;
	/**
	 * offset 280
	 */
	scaled_channel<uint16_t, 10000, 1> targetLambda = (uint16_t)0;
	/**
	AFR
	 * offset 282
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue = (uint16_t)0;
	/**
	m/s2
	 * offset 284
	 */
	scaled_channel<uint16_t, 300, 1> VssAcceleration = (uint16_t)0;
	/**
	 * offset 286
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue2 = (uint16_t)0;
	/**
	AFR
	 * offset 288
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue2 = (uint16_t)0;
	/**
	deg
	 * offset 290
	 */
	scaled_channel<uint16_t, 50, 1> vvtPositionB1E = (uint16_t)0;
	/**
	deg
	 * offset 292
	 */
	scaled_channel<uint16_t, 50, 1> vvtPositionB2I = (uint16_t)0;
	/**
	deg
	 * offset 294
	 */
	scaled_channel<uint16_t, 50, 1> vvtPositionB2E = (uint16_t)0;
	/**
	%
	 * offset 296
	 */
	scaled_channel<uint16_t, 100, 1> fuelPidCorrection[2];
	/**
	V
	 * offset 300
	 */
	scaled_channel<uint16_t, 1000, 1> rawTps1Secondary = (uint16_t)0;
	/**
	V
	 * offset 302
	 */
	scaled_channel<uint16_t, 1000, 1> rawTps2Primary = (uint16_t)0;
	/**
	V
	 * offset 304
	 */
	scaled_channel<uint16_t, 1000, 1> rawTps2Secondary = (uint16_t)0;
	/**
	 * offset 306
	 */
	uint16_t knockCount = (uint16_t)0;
	/**
	G
	 * offset 308
	 */
	scaled_channel<uint16_t, 100, 1> accelerationZ = (uint16_t)0;
	/**
	 * offset 310
	 */
	scaled_channel<uint16_t, 100, 1> accelerationRoll = (uint16_t)0;
	/**
	 * offset 312
	 */
	scaled_channel<uint16_t, 100, 1> accelerationYaw = (uint16_t)0;
	/**
	deg
	 * offset 314
	 */
	scaled_channel<uint8_t, 1, 1> vvtTargets[4];
	/**
	hz
	 * offset 318
	 */
	uint16_t turboSpeed = (uint16_t)0;
	/**
	 * offset 320
	 */
	uint8_t unusedAtTheEnd[20];
	/** total size 340*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Sun Nov 28 11:42:10 PST 2021
