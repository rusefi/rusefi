#pragma once
#include "rusefi_types.h"
struct pid_status_s {
	// offset 0
	float pTerm = (float)0;

	// offset 4
	scaled_channel<int16_t, 100, 1> iTerm = (int16_t)0;

	// offset 6
	scaled_channel<int16_t, 100, 1> dTerm = (int16_t)0;

	// offset 8
	scaled_channel<int16_t, 100, 1> output = (int16_t)0;

	// offset 10
	scaled_channel<int16_t, 100, 1> error = (int16_t)0;

	// offset 12
	uint32_t resetCounter = (uint32_t)0;

};
static_assert(sizeof(pid_status_s) == 16);

struct output_channels_s {
	// offset 0 bit 0
	bool sd_present : 1 {};

	// offset 0 bit 1
	bool sd_logging_internal : 1 {};

	// offset 0 bit 2
	bool triggerScopeReady : 1 {};

	// offset 0 bit 3
	bool antilagTriggered : 1 {};

	// radiator fan
	// offset 0 bit 4
	bool isFanOn : 1 {};

	// offset 0 bit 5
	bool isO2HeaterOn : 1 {};

	// offset 0 bit 6
	bool checkEngine : 1 {};

	// offset 0 bit 7
	bool needBurn : 1 {};

	// offset 0 bit 8
	bool sd_msd : 1 {};

	// offset 0 bit 9
	bool isFan2On : 1 {};

	// offset 0 bit 10
	bool acrActive : 1 {};

	// offset 0 bit 11
	bool toothLogReady : 1 {};

	// offset 0 bit 12
	bool isTpsError : 1 {};

	// offset 0 bit 13
	bool isCltError : 1 {};

	// offset 0 bit 14
	bool isMapError : 1 {};

	// offset 0 bit 15
	bool isIatError : 1 {};

	// offset 0 bit 16
	bool isTriggerError : 1 {};

	// offset 0 bit 17
	bool hasCriticalError : 1 {};

	// offset 0 bit 18
	bool isWarnNow : 1 {};

	// offset 0 bit 19
	bool isPedalError : 1 {};

	// offset 0 bit 20
	bool launchTriggered : 1 {};

	// offset 0 bit 21
	bool isTps2Error : 1 {};

	// offset 0 bit 22
	bool isIdleClosedLoop : 1 {};

	// offset 0 bit 23
	bool injectorFault : 1 {};

	// offset 0 bit 24
	bool ignitionFault : 1 {};

	// offset 0 bit 25
	bool unusedBit_25_25 : 1 {};

	// offset 0 bit 26
	bool unusedBit_25_26 : 1 {};

	// offset 0 bit 27
	bool unusedBit_25_27 : 1 {};

	// offset 0 bit 28
	bool unusedBit_25_28 : 1 {};

	// offset 0 bit 29
	bool unusedBit_25_29 : 1 {};

	// offset 0 bit 30
	bool unusedBit_25_30 : 1 {};

	// offset 0 bit 31
	bool unusedBit_25_31 : 1 {};

	// @@GAUGE_NAME_RPM@@
	// RPM
	// offset 4
	uint16_t RPMValue = (uint16_t)0;

	// dRPM
	// RPM/s
	// offset 6
	uint16_t rpmAcceleration = (uint16_t)0;

	// @@GAUGE_NAME_GEAR_RATIO@@
	// value
	// offset 8
	scaled_channel<uint16_t, 100, 1> speedToRpmRatio = (uint16_t)0;

	// @@GAUGE_NAME_VVS@@
	// kph
	// offset 10
	uint8_t vehicleSpeedKph = (uint8_t)0;

	// @@GAUGE_NAME_ECU_TEMPERATURE@@
	// deg C
	// offset 11
	int8_t internalMcuTemperature = (int8_t)0;

	// @@GAUGE_NAME_CLT@@
	// deg C
	// offset 12
	scaled_channel<int16_t, 100, 1> coolant = (int16_t)0;

	// @@GAUGE_NAME_IAT@@
	// deg C
	// offset 14
	scaled_channel<int16_t, 100, 1> intake = (int16_t)0;

	// deg C
	// offset 16
	scaled_channel<int16_t, 100, 1> auxTemp1 = (int16_t)0;

	// deg C
	// offset 18
	scaled_channel<int16_t, 100, 1> auxTemp2 = (int16_t)0;

	// @@GAUGE_NAME_TPS@@
	// %
	// offset 20
	scaled_channel<int16_t, 100, 1> TPSValue = (int16_t)0;

	// @@GAUGE_NAME_THROTTLE_PEDAL@@
	// %
	// offset 22
	scaled_channel<int16_t, 100, 1> throttlePedalPosition = (int16_t)0;

	// ADC
	// offset 24
	uint16_t tpsADC = (uint16_t)0;

	// V
	// offset 26
	scaled_channel<uint16_t, 1000, 1> rawMaf = (uint16_t)0;

	// @@GAUGE_NAME_AIR_FLOW_MEASURED@@
	// kg/h
	// offset 28
	scaled_channel<uint16_t, 10, 1> mafMeasured = (uint16_t)0;

	// @@GAUGE_NAME_MAP@@
	// kPa
	// offset 30
	scaled_channel<uint16_t, 30, 1> MAPValue = (uint16_t)0;

	// kPa
	// offset 32
	scaled_channel<uint16_t, 30, 1> baroPressure = (uint16_t)0;

	// @@GAUGE_NAME_LAMBDA@@
	// offset 34
	scaled_channel<uint16_t, 10000, 1> lambdaValue = (uint16_t)0;

	// @@GAUGE_NAME_KNOCK_RETARD@@
	// deg
	// offset 36
	scaled_channel<uint8_t, 10, 1> knockRetard = (uint8_t)0;

	// %
	// offset 37
	scaled_channel<uint8_t, 2, 1> idleCurrentPosition = (uint8_t)0;

	// @@GAUGE_NAME_VBAT@@
	// V
	// offset 38
	scaled_channel<uint16_t, 1000, 1> VBatt = (uint16_t)0;

	// @@GAUGE_NAME_OIL_PRESSURE@@
	// kPa
	// offset 40
	scaled_channel<uint16_t, 30, 1> oilPressure = (uint16_t)0;

	// @@GAUGE_NAME_VVT_B1I@@
	// deg
	// offset 42
	scaled_channel<int16_t, 50, 1> vvtPositionB1I = (int16_t)0;

	// @@GAUGE_NAME_AIR_MASS@@
	// g
	// offset 44
	scaled_channel<uint16_t, 1000, 1> chargeAirMass = (uint16_t)0;

	// crankingFuelMs
	// airmass in mg, 0-65 grams
	// ms
	// offset 46
	scaled_channel<uint16_t, 100, 1> crankingFuelMs = (uint16_t)0;

	// @@GAUGE_NAME_FUEL_BASE@@
	// This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	// mg
	// offset 48
	scaled_channel<uint16_t, 100, 1> baseFuel = (uint16_t)0;

	// @@GAUGE_NAME_FUEL_RUNNING@@
	// Total fuel with CLT IAT and TPS acceleration without injector lag corrections per cycle, as pulse per cycle
	// mg
	// offset 50
	scaled_channel<uint16_t, 100, 1> fuelRunning = (uint16_t)0;

	// @@GAUGE_NAME_FUEL_LAST_INJECTION@@
	// Actual last injection time - including all compensation and injection mode
	// ms
	// offset 52
	scaled_channel<uint16_t, 300, 1> actualLastInjection = (uint16_t)0;

	// @@GAUGE_NAME_FUEL_INJ_DUTY@@
	// %
	// offset 54
	scaled_channel<uint8_t, 2, 1> injectorDutyCycle = (uint8_t)0;

	// @@GAUGE_NAME_FUEL_VE@@
	// ratio
	// offset 55
	scaled_channel<uint8_t, 2, 1> veValue = (uint8_t)0;

	// deg
	// offset 56
	int16_t injectionOffset = (int16_t)0;

	// deg C
	// offset 58
	scaled_channel<int16_t, 100, 1> tCharge = (int16_t)0;

	// @@GAUGE_NAME_INJECTOR_LAG@@
	// ms
	// offset 60
	scaled_channel<uint16_t, 300, 1> injectorLagMs = (uint16_t)0;

	// @@GAUGE_NAME_ENGINE_CRC16@@
	// crc16
	// offset 62
	uint16_t engineMakeCodeNameCrc16 = (uint16_t)0;

	// @@GAUGE_NAME_FUEL_WALL_AMOUNT@@
	// mg
	// offset 64
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;

	// @@GAUGE_NAME_FUEL_WALL_CORRECTION@@
	// mg
	// offset 66
	scaled_channel<int16_t, 100, 1> wallFuelCorrectionValue = (int16_t)0;

	// offset 68
	uint16_t revolutionCounterSinceStart = (uint16_t)0;

	// @@GAUGE_NAME_FUEL_TPS_ROC@@
	// ratio
	// offset 70
	scaled_channel<int16_t, 100, 1> deltaTpsValue = (int16_t)0;

	// offset 72
	uint16_t canReadCounter = (uint16_t)0;

	// @@GAUGE_NAME_FUEL_TPS_EXTRA@@
	// ms
	// offset 74
	scaled_channel<int16_t, 300, 1> tpsAccelFuel = (int16_t)0;

	// @@GAUGE_NAME_TIMING_ADVANCE@@
	// deg
	// offset 76
	scaled_channel<int16_t, 50, 1> ignitionAdvance = (int16_t)0;

	// offset 78
	uint16_t unusedDwellWasHere = (uint16_t)0;

	// @@GAUGE_NAME_DWELL_DUTY@@
	// %
	// offset 80
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;

	// @@GAUGE_NAME_IAC@@
	// %
	// offset 82
	scaled_channel<uint16_t, 100, 1> idleAirValvePosition = (uint16_t)0;

	// ETB Target
	// %
	// offset 84
	scaled_channel<int16_t, 100, 1> etbTarget = (int16_t)0;

	// @@GAUGE_NAME_ETB_DUTY@@
	// %
	// offset 86
	scaled_channel<int16_t, 100, 1> etb1DutyCycle = (int16_t)0;

	// Fuel level
	// %
	// offset 88
	scaled_channel<int16_t, 100, 1> fuelTankLevel = (int16_t)0;

	// @@GAUGE_NAME_FUEL_CONSUMPTION@@
	// grams
	// offset 90
	uint16_t totalFuelConsumption = (uint16_t)0;

	// @@GAUGE_NAME_FUEL_FLOW@@
	// gram/s
	// offset 92
	scaled_channel<uint16_t, 200, 1> fuelFlowRate = (uint16_t)0;

	// @@GAUGE_NAME_TPS2@@
	// %
	// offset 94
	scaled_channel<int16_t, 100, 1> TPS2Value = (int16_t)0;

	// @@GAUGE_NAME_TUNE_CRC16@@
	// crc16
	// offset 96
	uint16_t tuneCrc16 = (uint16_t)0;

	// need 4 byte alignment
	// units
	// offset 98
	uint8_t alignmentFill_at_98[2];

	// @@GAUGE_NAME_KNOCK_LEVEL@@
	// Volts
	// offset 100
	float knockLevel = (float)0;

	// @@GAUGE_NAME_UPTIME@@
	// sec
	// offset 104
	uint32_t seconds = (uint32_t)0;

	// Engine Mode
	// em
	// offset 108
	uint32_t engineMode = (uint32_t)0;

	// @@GAUGE_NAME_VERSION@@
	// version_f
	// offset 112
	uint32_t firmwareVersion = (uint32_t)0;

	// V
	// offset 116
	scaled_channel<int16_t, 1000, 1> rawWastegatePosition = (int16_t)0;

	// @@GAUGE_NAME_ACCEL_X@@
	// G
	// offset 118
	scaled_channel<int16_t, 100, 1> accelerationX = (int16_t)0;

	// @@GAUGE_NAME_ACCEL_Y@@
	// G
	// offset 120
	scaled_channel<int16_t, 100, 1> accelerationY = (int16_t)0;

	// need 4 byte alignment
	// units
	// offset 122
	uint8_t alignmentFill_at_122[2];

	// offset 124
	uint32_t tsConfigVersion = (uint32_t)0;

	// offset 128
	float calibrationValue = (float)0;

	// offset 132
	uint8_t calibrationMode = (uint8_t)0;

	// offset 133
	uint8_t idleTargetPosition = (uint8_t)0;

	// @@GAUGE_NAME_DETECTED_GEAR@@
	// offset 134
	uint8_t detectedGear = (uint8_t)0;

	// offset 135
	uint8_t maxTriggerReentrant = (uint8_t)0;

	// @@GAUGE_NAME_TRG_ERR@@
	// counter
	// offset 136
	uint32_t totalTriggerErrorCounter = (uint32_t)0;

	// offset 140
	uint32_t orderingErrorCounter = (uint32_t)0;

	// @@GAUGE_NAME_WARNING_COUNTER@@
	// count
	// offset 144
	uint16_t warningCounter = (uint16_t)0;

	// @@GAUGE_NAME_WARNING_LAST@@
	// error
	// offset 146
	uint16_t lastErrorCode = (uint16_t)0;

	// error
	// offset 148
	uint16_t recentErrorCode[8];

	// val
	// offset 164
	float debugFloatField1 = (float)0;

	// val
	// offset 168
	float debugFloatField2 = (float)0;

	// val
	// offset 172
	float debugFloatField3 = (float)0;

	// val
	// offset 176
	float debugFloatField4 = (float)0;

	// val
	// offset 180
	float debugFloatField5 = (float)0;

	// val
	// offset 184
	float debugFloatField6 = (float)0;

	// val
	// offset 188
	float debugFloatField7 = (float)0;

	// val
	// offset 192
	uint32_t debugIntField1 = (uint32_t)0;

	// val
	// offset 196
	uint32_t debugIntField2 = (uint32_t)0;

	// val
	// offset 200
	uint32_t debugIntField3 = (uint32_t)0;

	// val
	// offset 204
	int16_t debugIntField4 = (int16_t)0;

	// val
	// offset 206
	int16_t debugIntField5 = (int16_t)0;

	// deg C
	// offset 208
	uint16_t egt[EGT_CHANNEL_COUNT];

	// V
	// offset 224
	scaled_channel<int16_t, 1000, 1> rawTps1Primary = (int16_t)0;

	// V
	// offset 226
	scaled_channel<int16_t, 1000, 1> rawPpsPrimary = (int16_t)0;

	// V
	// offset 228
	scaled_channel<int16_t, 1000, 1> rawClt = (int16_t)0;

	// V
	// offset 230
	scaled_channel<int16_t, 1000, 1> rawIat = (int16_t)0;

	// V
	// offset 232
	scaled_channel<int16_t, 1000, 1> rawOilPressure = (int16_t)0;

	// offset 234
	uint8_t fuelClosedLoopBinIdx = (uint8_t)0;

	// @@GAUGE_NAME_CURRENT_GEAR@@
	// gear
	// offset 235
	int8_t tcuCurrentGear = (int8_t)0;

	// V
	// offset 236
	scaled_channel<int16_t, 1000, 1> rawPpsSecondary = (int16_t)0;

	// dBv
	// offset 238
	int8_t knock[12];

	// @@GAUGE_NAME_DESIRED_GEAR@@
	// gear
	// offset 250
	int8_t tcuDesiredGear = (int8_t)0;

	// @@GAUGE_NAME_FLEX@@
	// %
	// offset 251
	scaled_channel<uint8_t, 2, 1> flexPercent = (uint8_t)0;

	// @@GAUGE_NAME_WG_POSITION@@
	// %
	// offset 252
	scaled_channel<int16_t, 100, 1> wastegatePositionSensor = (int16_t)0;

	// V
	// offset 254
	scaled_channel<int16_t, 1000, 1> rawLowFuelPressure = (int16_t)0;

	// V
	// offset 256
	scaled_channel<int16_t, 1000, 1> rawHighFuelPressure = (int16_t)0;

	// @@GAUGE_NAME_FUEL_PRESSURE_LOW@@
	// kpa
	// offset 258
	scaled_channel<int16_t, 30, 1> lowFuelPressure = (int16_t)0;

	// @@GAUGE_NAME_FUEL_PRESSURE_HIGH@@
	// bar
	// offset 260
	scaled_channel<int16_t, 10, 1> highFuelPressure = (int16_t)0;

	// @@GAUGE_NAME_AFR@@
	// AFR
	// offset 262
	scaled_channel<uint16_t, 1000, 1> AFRValue = (uint16_t)0;

	// Vss Accel
	// m/s2
	// offset 264
	scaled_channel<uint16_t, 300, 1> VssAcceleration = (uint16_t)0;

	// @@GAUGE_NAME_LAMBDA2@@
	// offset 266
	scaled_channel<uint16_t, 10000, 1> lambdaValue2 = (uint16_t)0;

	// @@GAUGE_NAME_AFR2@@
	// AFR
	// offset 268
	scaled_channel<uint16_t, 1000, 1> AFRValue2 = (uint16_t)0;

	// @@GAUGE_NAME_VVT_B1E@@
	// deg
	// offset 270
	scaled_channel<int16_t, 50, 1> vvtPositionB1E = (int16_t)0;

	// @@GAUGE_NAME_VVT_B2I@@
	// deg
	// offset 272
	scaled_channel<int16_t, 50, 1> vvtPositionB2I = (int16_t)0;

	// @@GAUGE_NAME_VVT_B2E@@
	// deg
	// offset 274
	scaled_channel<int16_t, 50, 1> vvtPositionB2E = (int16_t)0;

	// %
	// offset 276
	scaled_channel<int16_t, 100, 1> fuelPidCorrection[2];

	// V
	// offset 280
	scaled_channel<int16_t, 1000, 1> rawTps1Secondary = (int16_t)0;

	// V
	// offset 282
	scaled_channel<int16_t, 1000, 1> rawTps2Primary = (int16_t)0;

	// V
	// offset 284
	scaled_channel<int16_t, 1000, 1> rawTps2Secondary = (int16_t)0;

	// @@GAUGE_NAME_KNOCK_COUNTER@@
	// offset 286
	uint16_t knockCount = (uint16_t)0;

	// @@GAUGE_NAME_ACCEL_Z@@
	// G
	// offset 288
	scaled_channel<int16_t, 100, 1> accelerationZ = (int16_t)0;

	// @@GAUGE_NAME_ACCEL_ROLL@@
	// G
	// offset 290
	scaled_channel<int16_t, 100, 1> accelerationRoll = (int16_t)0;

	// @@GAUGE_NAME_ACCEL_YAW@@
	// G
	// offset 292
	scaled_channel<int16_t, 100, 1> accelerationYaw = (int16_t)0;

	// deg
	// offset 294
	int8_t vvtTargets[4];

	// @@GAUGE_NAME_TURBO_SPEED@@
	// hz
	// offset 298
	uint16_t turboSpeed = (uint16_t)0;

	// %
	// offset 300
	scaled_channel<int16_t, 100, 1> tps1Split = (int16_t)0;

	// %
	// offset 302
	scaled_channel<int16_t, 100, 1> tps2Split = (int16_t)0;

	// %
	// offset 304
	scaled_channel<int16_t, 100, 1> tps12Split = (int16_t)0;

	// %
	// offset 306
	scaled_channel<int16_t, 100, 1> accPedalSplit = (int16_t)0;

	// Spark Cut Code
	// code
	// offset 308
	int8_t sparkCutReason = (int8_t)0;

	// Fuel Cut Code
	// code
	// offset 309
	int8_t fuelCutReason = (int8_t)0;

	// @@GAUGE_NAME_AIR_FLOW_ESTIMATE@@
	// kg/h
	// offset 310
	scaled_channel<uint16_t, 10, 1> mafEstimate = (uint16_t)0;

	// rpm
	// offset 312
	uint16_t instantRpm = (uint16_t)0;

	// counter
	// offset 314
	uint16_t systemEventReuse = (uint16_t)0;

	// V
	// offset 316
	scaled_channel<uint16_t, 1000, 1> rawMap = (uint16_t)0;

	// V
	// offset 318
	scaled_channel<uint16_t, 1000, 1> rawAfr = (uint16_t)0;

	// %
	// offset 320
	uint8_t tpsAccelFrom = (uint8_t)0;

	// %
	// offset 321
	uint8_t tpsAccelTo = (uint8_t)0;

	// need 4 byte alignment
	// units
	// offset 322
	uint8_t alignmentFill_at_322[2];

	// offset 324
	float calibrationValue2 = (float)0;

	// offset 328 bit 0
	bool isMainRelayOn : 1 {};

	// isUsbConnected
	// Original reason for this is to check if USB is connected from Lua
	// offset 328 bit 1
	bool isUsbConnected : 1 {};

	// offset 328 bit 2
	bool unusedBit_165_2 : 1 {};

	// offset 328 bit 3
	bool unusedBit_165_3 : 1 {};

	// offset 328 bit 4
	bool unusedBit_165_4 : 1 {};

	// offset 328 bit 5
	bool unusedBit_165_5 : 1 {};

	// offset 328 bit 6
	bool unusedBit_165_6 : 1 {};

	// offset 328 bit 7
	bool unusedBit_165_7 : 1 {};

	// offset 328 bit 8
	bool unusedBit_165_8 : 1 {};

	// offset 328 bit 9
	bool unusedBit_165_9 : 1 {};

	// offset 328 bit 10
	bool unusedBit_165_10 : 1 {};

	// offset 328 bit 11
	bool unusedBit_165_11 : 1 {};

	// offset 328 bit 12
	bool unusedBit_165_12 : 1 {};

	// offset 328 bit 13
	bool unusedBit_165_13 : 1 {};

	// offset 328 bit 14
	bool unusedBit_165_14 : 1 {};

	// offset 328 bit 15
	bool unusedBit_165_15 : 1 {};

	// offset 328 bit 16
	bool unusedBit_165_16 : 1 {};

	// offset 328 bit 17
	bool unusedBit_165_17 : 1 {};

	// offset 328 bit 18
	bool unusedBit_165_18 : 1 {};

	// offset 328 bit 19
	bool unusedBit_165_19 : 1 {};

	// offset 328 bit 20
	bool unusedBit_165_20 : 1 {};

	// offset 328 bit 21
	bool unusedBit_165_21 : 1 {};

	// offset 328 bit 22
	bool unusedBit_165_22 : 1 {};

	// offset 328 bit 23
	bool unusedBit_165_23 : 1 {};

	// offset 328 bit 24
	bool unusedBit_165_24 : 1 {};

	// offset 328 bit 25
	bool unusedBit_165_25 : 1 {};

	// offset 328 bit 26
	bool unusedBit_165_26 : 1 {};

	// offset 328 bit 27
	bool unusedBit_165_27 : 1 {};

	// offset 328 bit 28
	bool unusedBit_165_28 : 1 {};

	// offset 328 bit 29
	bool unusedBit_165_29 : 1 {};

	// offset 328 bit 30
	bool unusedBit_165_30 : 1 {};

	// offset 328 bit 31
	bool unusedBit_165_31 : 1 {};

	// count
	// offset 332
	uint32_t luaInvocationCounter = (uint32_t)0;

	// nt
	// offset 336
	uint32_t luaLastCycleDuration = (uint32_t)0;

	// count
	// offset 340
	uint8_t testBenchIter = (uint8_t)0;

	// "Current Range"
	// offset 341
	uint8_t tcu_currentRange = (uint8_t)0;

	// @@GAUGE_NAME_TC_RATIO@@
	// value
	// offset 342
	scaled_channel<uint16_t, 100, 1> tcRatio = (uint16_t)0;

	// offset 344
	float lastShiftTime = (float)0;

	// offset 348
	uint32_t vssEdgeCounter = (uint32_t)0;

	// offset 352
	uint32_t issEdgeCounter = (uint32_t)0;

	// @@GAUGE_NAME_AUX_LINEAR_1@@
	// offset 356
	scaled_channel<int16_t, 100, 1> auxLinear1 = (int16_t)0;

	// @@GAUGE_NAME_AUX_LINEAR_2@@
	// offset 358
	scaled_channel<int16_t, 100, 1> auxLinear2 = (int16_t)0;

	// offset 360 bit 0
	bool dfcoActive : 1 {};

	// offset 360 bit 1
	bool tpsAccelActive : 1 {};

	// offset 360 bit 2
	bool unusedBit_207_2 : 1 {};

	// offset 360 bit 3
	bool unusedBit_207_3 : 1 {};

	// offset 360 bit 4
	bool unusedBit_207_4 : 1 {};

	// offset 360 bit 5
	bool unusedBit_207_5 : 1 {};

	// offset 360 bit 6
	bool unusedBit_207_6 : 1 {};

	// offset 360 bit 7
	bool unusedBit_207_7 : 1 {};

	// offset 360 bit 8
	bool unusedBit_207_8 : 1 {};

	// offset 360 bit 9
	bool unusedBit_207_9 : 1 {};

	// offset 360 bit 10
	bool unusedBit_207_10 : 1 {};

	// offset 360 bit 11
	bool unusedBit_207_11 : 1 {};

	// offset 360 bit 12
	bool unusedBit_207_12 : 1 {};

	// offset 360 bit 13
	bool unusedBit_207_13 : 1 {};

	// offset 360 bit 14
	bool unusedBit_207_14 : 1 {};

	// offset 360 bit 15
	bool unusedBit_207_15 : 1 {};

	// offset 360 bit 16
	bool unusedBit_207_16 : 1 {};

	// offset 360 bit 17
	bool unusedBit_207_17 : 1 {};

	// offset 360 bit 18
	bool unusedBit_207_18 : 1 {};

	// offset 360 bit 19
	bool unusedBit_207_19 : 1 {};

	// offset 360 bit 20
	bool unusedBit_207_20 : 1 {};

	// offset 360 bit 21
	bool unusedBit_207_21 : 1 {};

	// offset 360 bit 22
	bool unusedBit_207_22 : 1 {};

	// offset 360 bit 23
	bool unusedBit_207_23 : 1 {};

	// offset 360 bit 24
	bool unusedBit_207_24 : 1 {};

	// offset 360 bit 25
	bool unusedBit_207_25 : 1 {};

	// offset 360 bit 26
	bool unusedBit_207_26 : 1 {};

	// offset 360 bit 27
	bool unusedBit_207_27 : 1 {};

	// offset 360 bit 28
	bool unusedBit_207_28 : 1 {};

	// offset 360 bit 29
	bool unusedBit_207_29 : 1 {};

	// offset 360 bit 30
	bool unusedBit_207_30 : 1 {};

	// offset 360 bit 31
	bool unusedBit_207_31 : 1 {};

	// kPa
	// offset 364
	scaled_channel<uint16_t, 10, 1> fallbackMap = (uint16_t)0;

	// Instant MAP
	// kPa
	// offset 366
	scaled_channel<uint16_t, 30, 1> instantMAPValue = (uint16_t)0;

	// us
	// offset 368
	uint16_t maxLockedDuration = (uint16_t)0;

	// offset 370
	uint16_t canWriteOk = (uint16_t)0;

	// offset 372
	uint16_t canWriteNotOk = (uint16_t)0;

	// need 4 byte alignment
	// units
	// offset 374
	uint8_t alignmentFill_at_374[2];

	// offset 376
	uint32_t triggerPrimaryFall = (uint32_t)0;

	// offset 380
	uint32_t triggerPrimaryRise = (uint32_t)0;

	// offset 384
	uint32_t triggerSecondaryFall = (uint32_t)0;

	// offset 388
	uint32_t triggerSecondaryRise = (uint32_t)0;

	// offset 392
	uint32_t triggerVvtFall = (uint32_t)0;

	// offset 396
	uint32_t triggerVvtRise = (uint32_t)0;

	// offset 400
	uint8_t starterState = (uint8_t)0;

	// offset 401
	uint8_t starterRelayDisable = (uint8_t)0;

	// offset 402
	uint8_t multiSparkCounter = (uint8_t)0;

	// offset 403
	uint8_t extiOverflowCount = (uint8_t)0;

	// offset 404
	pid_status_s alternatorStatus;

	// offset 420
	pid_status_s idleStatus;

	// offset 436
	pid_status_s etbStatus;

	// offset 452
	pid_status_s boostStatus;

	// offset 468
	pid_status_s wastegateDcStatus;

	// offset 484
	pid_status_s vvtStatus[CAM_INPUTS_COUNT];

	// aux speed 1
	// s
	// offset 548
	uint16_t auxSpeed1 = (uint16_t)0;

	// aux speed 2
	// s
	// offset 550
	uint16_t auxSpeed2 = (uint16_t)0;

	// @@GAUGE_NAME_ISS@@
	// RPM
	// offset 552
	uint16_t ISSValue = (uint16_t)0;

	// V
	// offset 554
	scaled_channel<int16_t, 1000, 1> rawAnalogInput[LUA_ANALOG_INPUT_COUNT];

	// GPPWM Output
	// %
	// offset 570
	scaled_channel<uint8_t, 2, 1> gppwmOutput[4];

	// offset 574
	scaled_channel<int16_t, 1, 1> gppwmXAxis[4];

	// offset 582
	scaled_channel<int16_t, 10, 1> gppwmYAxis[4];

	// V
	// offset 590
	scaled_channel<int16_t, 1000, 1> rawBattery = (int16_t)0;

	// offset 592
	scaled_channel<int16_t, 10, 1> ignBlendParameter[IGN_BLEND_COUNT];

	// %
	// offset 600
	scaled_channel<uint8_t, 2, 1> ignBlendBias[IGN_BLEND_COUNT];

	// deg
	// offset 604
	scaled_channel<int16_t, 100, 1> ignBlendOutput[IGN_BLEND_COUNT];

	// offset 612
	scaled_channel<int16_t, 10, 1> veBlendParameter[VE_BLEND_COUNT];

	// %
	// offset 620
	scaled_channel<uint8_t, 2, 1> veBlendBias[VE_BLEND_COUNT];

	// %
	// offset 624
	scaled_channel<int16_t, 100, 1> veBlendOutput[VE_BLEND_COUNT];

	// offset 632 bit 0
	bool coilState1 : 1 {};

	// offset 632 bit 1
	bool coilState2 : 1 {};

	// offset 632 bit 2
	bool coilState3 : 1 {};

	// offset 632 bit 3
	bool coilState4 : 1 {};

	// offset 632 bit 4
	bool coilState5 : 1 {};

	// offset 632 bit 5
	bool coilState6 : 1 {};

	// offset 632 bit 6
	bool coilState7 : 1 {};

	// offset 632 bit 7
	bool coilState8 : 1 {};

	// offset 632 bit 8
	bool coilState9 : 1 {};

	// offset 632 bit 9
	bool coilState10 : 1 {};

	// offset 632 bit 10
	bool coilState11 : 1 {};

	// offset 632 bit 11
	bool coilState12 : 1 {};

	// offset 632 bit 12
	bool injectorState1 : 1 {};

	// offset 632 bit 13
	bool injectorState2 : 1 {};

	// offset 632 bit 14
	bool injectorState3 : 1 {};

	// offset 632 bit 15
	bool injectorState4 : 1 {};

	// offset 632 bit 16
	bool injectorState5 : 1 {};

	// offset 632 bit 17
	bool injectorState6 : 1 {};

	// offset 632 bit 18
	bool injectorState7 : 1 {};

	// offset 632 bit 19
	bool injectorState8 : 1 {};

	// offset 632 bit 20
	bool injectorState9 : 1 {};

	// offset 632 bit 21
	bool injectorState10 : 1 {};

	// offset 632 bit 22
	bool injectorState11 : 1 {};

	// offset 632 bit 23
	bool injectorState12 : 1 {};

	// offset 632 bit 24
	bool unusedBit_297_24 : 1 {};

	// offset 632 bit 25
	bool unusedBit_297_25 : 1 {};

	// offset 632 bit 26
	bool unusedBit_297_26 : 1 {};

	// offset 632 bit 27
	bool unusedBit_297_27 : 1 {};

	// offset 632 bit 28
	bool unusedBit_297_28 : 1 {};

	// offset 632 bit 29
	bool unusedBit_297_29 : 1 {};

	// offset 632 bit 30
	bool unusedBit_297_30 : 1 {};

	// offset 632 bit 31
	bool unusedBit_297_31 : 1 {};

	// offset 636
	uint32_t outputRequestPeriod = (uint32_t)0;

	// offset 640
	float mapFast = (float)0;

	// value
	// offset 644
	float luaGauges[LUA_GAUGE_COUNT];

	// V
	// offset 652
	scaled_channel<uint16_t, 1000, 1> rawMaf2 = (uint16_t)0;

	// @@GAUGE_NAME_AIR_FLOW_MEASURED_2@@
	// kg/h
	// offset 654
	scaled_channel<uint16_t, 10, 1> mafMeasured2 = (uint16_t)0;

	// offset 656
	uint16_t schedulingUsedCount = (uint16_t)0;

	// offset 658
	uint8_t unusedAtTheEnd[136];

	// need 4 byte alignment
	// units
	// offset 794
	uint8_t alignmentFill_at_794[2];

};
static_assert(sizeof(output_channels_s) == 796);

