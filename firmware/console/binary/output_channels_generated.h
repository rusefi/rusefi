// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Sat Sep 03 21:41:58 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of pid_status_s
struct pid_status_s {
	/**
	 * offset 0
	 */
	scaled_channel<float, 1, 1> pTerm = (float)0;
	/**
	 * offset 4
	 */
	scaled_channel<int16_t, 100, 1> iTerm = (int16_t)0;
	/**
	 * offset 6
	 */
	scaled_channel<int16_t, 100, 1> dTerm = (int16_t)0;
	/**
	 * offset 8
	 */
	scaled_channel<int16_t, 100, 1> output = (int16_t)0;
	/**
	 * offset 10
	 */
	scaled_channel<int16_t, 100, 1> error = (int16_t)0;
	/**
	 * offset 12
	 */
	uint32_t resetCounter = (uint32_t)0;
};
static_assert(sizeof(pid_status_s) == 16);

// start of output_channels_s
struct output_channels_s {
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
	bool unusedb4 : 1 {};
	/**
	offset 0 bit 5 */
	bool unusedb5 : 1 {};
	/**
	 * radiator fan
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
	bool isFan2On : 1 {};
	/**
	offset 0 bit 12 */
	bool alternatorOnOff : 1 {};
	/**
	offset 0 bit 13 */
	bool toothLogReady : 1 {};
	/**
	offset 0 bit 14 */
	bool unused3123 : 1 {};
	/**
	offset 0 bit 15 */
	bool isTpsError : 1 {};
	/**
	offset 0 bit 16 */
	bool isCltError : 1 {};
	/**
	offset 0 bit 17 */
	bool isMapError : 1 {};
	/**
	offset 0 bit 18 */
	bool isIatError : 1 {};
	/**
	offset 0 bit 19 */
	bool unused131313 : 1 {};
	/**
	offset 0 bit 20 */
	bool isTriggerError : 1 {};
	/**
	offset 0 bit 21 */
	bool hasCriticalError : 1 {};
	/**
	offset 0 bit 22 */
	bool isWarnNow : 1 {};
	/**
	offset 0 bit 23 */
	bool isPedalError : 1 {};
	/**
	offset 0 bit 24 */
	bool isKnockChipOk : 1 {};
	/**
	offset 0 bit 25 */
	bool launchTriggered : 1 {};
	/**
	offset 0 bit 26 */
	bool isTps2Error : 1 {};
	/**
	offset 0 bit 27 */
	bool isIdleClosedLoop : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_28_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_28_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_28_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_28_31 : 1 {};
	/**
	 * @@GAUGE_NAME_RPM@@
	RPM
	 * offset 4
	 */
	scaled_channel<uint16_t, 1, 1> RPMValue = (uint16_t)0;
	/**
	 * dRPM
	RPM/s
	 * offset 6
	 */
	uint16_t rpmAcceleration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_GEAR_RATIO@@
	value
	 * offset 8
	 */
	scaled_channel<uint16_t, 100, 1> speedToRpmRatio = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVS@@
	kph
	 * offset 10
	 */
	scaled_channel<uint8_t, 1, 1> vehicleSpeedKph = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_CPU_TEMP@@
	deg C
	 * offset 11
	 */
	scaled_channel<int8_t, 1, 1> internalMcuTemperature = (int8_t)0;
	/**
	 * @@GAUGE_NAME_CLT@@
	deg C
	 * offset 12
	 */
	scaled_channel<int16_t, 100, 1> coolant = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IAT@@
	deg C
	 * offset 14
	 */
	scaled_channel<int16_t, 100, 1> intake = (int16_t)0;
	/**
	deg C
	 * offset 16
	 */
	scaled_channel<int16_t, 100, 1> auxTemp1 = (int16_t)0;
	/**
	deg C
	 * offset 18
	 */
	scaled_channel<int16_t, 100, 1> auxTemp2 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TPS@@
	%
	 * offset 20
	 */
	scaled_channel<int16_t, 100, 1> TPSValue = (int16_t)0;
	/**
	 * @@GAUGE_NAME_THROTTLE_PEDAL@@
	%
	 * offset 22
	 */
	scaled_channel<int16_t, 100, 1> throttlePedalPosition = (int16_t)0;
	/**
	ADC
	 * offset 24
	 */
	uint16_t tpsADC = (uint16_t)0;
	/**
	V
	 * offset 26
	 */
	scaled_channel<uint16_t, 1000, 1> rawMaf = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_MEASURED@@
	kg/h
	 * offset 28
	 */
	scaled_channel<uint16_t, 10, 1> mafMeasured = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_MAP@@
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
	 * @@GAUGE_NAME_LAMBDA@@
	 * offset 34
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_KNOCK_RETARD@@
	deg
	 * offset 36
	 */
	scaled_channel<uint8_t, 10, 1> knockRetard = (uint8_t)0;
	/**
	%
	 * offset 37
	 */
	scaled_channel<uint8_t, 2, 1> idleCurrentPosition = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_VBAT@@
	V
	 * offset 38
	 */
	scaled_channel<uint16_t, 1000, 1> VBatt = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_OIL_PRESSURE@@
	kPa
	 * offset 40
	 */
	scaled_channel<uint16_t, 30, 1> oilPressure = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1I@@
	deg
	 * offset 42
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_MASS@@
	g
	 * offset 44
	 */
	scaled_channel<uint16_t, 1000, 1> chargeAirMass = (uint16_t)0;
	/**
	 * crankingFuelMs
	 * airmass in mg, 0-65 grams
	ms
	 * offset 46
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelMs = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_AFR@@
	ratio
	 * offset 48
	 */
	scaled_channel<uint16_t, 1000, 1> currentTargetAfr = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_BASE@@
	 * This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	mg
	 * offset 50
	 */
	scaled_channel<uint16_t, 100, 1> baseFuel = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_RUNNING@@
	 * Total fuel with CLT IAT and TPS acceleration without injector lag corrections per cycle, as pulse per cycle
	mg
	 * offset 52
	 */
	scaled_channel<uint16_t, 100, 1> fuelRunning = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION@@
	 * Actual last injection time - including all compensation and injection mode
	ms
	 * offset 54
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjection = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_INJ_DUTY@@
	%
	 * offset 56
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycle = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_VE@@
	ratio
	 * offset 57
	 */
	scaled_channel<uint8_t, 2, 1> veValue = (uint8_t)0;
	/**
	deg
	 * offset 58
	 */
	scaled_channel<int16_t, 1, 1> injectionOffset = (int16_t)0;
	/**
	deg C
	 * offset 60
	 */
	scaled_channel<int16_t, 100, 1> tCharge = (int16_t)0;
	/**
	 * @@GAUGE_NAME_INJECTOR_LAG@@
	ms
	 * offset 62
	 */
	scaled_channel<uint16_t, 300, 1> injectorLagMs = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_LOAD@@
	%
	 * offset 64
	 */
	scaled_channel<uint16_t, 100, 1> fuelingLoad = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_IGNITION_LOAD@@
	%
	 * offset 66
	 */
	scaled_channel<uint16_t, 100, 1> ignitionLoad = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ENGINE_CRC16@@
	crc16
	 * offset 68
	 */
	scaled_channel<uint16_t, 1, 1> engineMakeCodeNameCrc16 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_AMOUNT@@
	mg
	 * offset 70
	 */
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_CORRECTION@@
	mg
	 * offset 72
	 */
	scaled_channel<int16_t, 100, 1> wallFuelCorrection = (int16_t)0;
	/**
	 * offset 74
	 */
	uint16_t revolutionCounterSinceStart = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_TPS_ROC@@
	ratio
	 * offset 76
	 */
	scaled_channel<int16_t, 100, 1> deltaTps = (int16_t)0;
	/**
	 * offset 78
	 */
	uint16_t canReadCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_TPS_EXTRA@@
	ms
	 * offset 80
	 */
	scaled_channel<int16_t, 300, 1> tpsAccelFuel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TIMING_ADVANCE@@
	deg
	 * offset 82
	 */
	scaled_channel<int16_t, 50, 1> ignitionAdvance = (int16_t)0;
	/**
	 * @@GAUGE_COIL_DWELL_TIME@@
	ms
	 * offset 84
	 */
	scaled_channel<uint16_t, 300, 1> sparkDwellValue = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_DWELL_DUTY@@
	%
	 * offset 86
	 */
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_IAC@@
	%
	 * offset 88
	 */
	scaled_channel<uint16_t, 100, 1> idleAirValvePosition = (uint16_t)0;
	/**
	 * ETB Target
	%
	 * offset 90
	 */
	scaled_channel<int16_t, 100, 1> etbTarget = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ETB_DUTY@@
	%
	 * offset 92
	 */
	scaled_channel<int16_t, 100, 1> etb1DutyCycle = (int16_t)0;
	/**
	 * ETB Error
	%
	 * offset 94
	 */
	scaled_channel<int16_t, 100, 1> etb1Error = (int16_t)0;
	/**
	 * Fuel level
	%
	 * offset 96
	 */
	scaled_channel<int16_t, 100, 1> fuelTankLevel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_CONSUMPTION@@
	grams
	 * offset 98
	 */
	scaled_channel<uint16_t, 1, 1> totalFuelConsumption = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_FLOW@@
	gram/s
	 * offset 100
	 */
	scaled_channel<uint16_t, 200, 1> fuelFlowRate = (uint16_t)0;
	/**
	%
	 * offset 102
	 */
	scaled_channel<uint16_t, 100, 1> veTableYAxis = (uint16_t)0;
	/**
	%
	 * offset 104
	 */
	scaled_channel<uint16_t, 100, 1> afrTableYAxis = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 106
	 */
	uint8_t alignmentFill_at_106[2];
	/**
	 * @@GAUGE_NAME_KNOCK_LEVEL@@
	Volts
	 * offset 108
	 */
	scaled_channel<float, 1, 1> knockLevel = (float)0;
	/**
	 * @@GAUGE_NAME_UPTIME@@
	sec
	 * offset 112
	 */
	scaled_channel<uint32_t, 1, 1> seconds = (uint32_t)0;
	/**
	 * Engine Mode
	em
	 * offset 116
	 */
	scaled_channel<uint32_t, 1, 1> engineMode = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_VERSION@@
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
	scaled_channel<uint8_t, 1, 1> idleTargetPosition = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 134
	 */
	uint8_t alignmentFill_at_134[2];
	/**
	 * @@GAUGE_NAME_TRG_ERR@@
	counter
	 * offset 136
	 */
	scaled_channel<uint32_t, 1, 1> totalTriggerErrorCounter = (uint32_t)0;
	/**
	 * offset 140
	 */
	uint32_t orderingErrorCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_COUNTER@@
	count
	 * offset 144
	 */
	scaled_channel<uint16_t, 1, 1> warningCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_LAST@@
	error
	 * offset 146
	 */
	scaled_channel<uint16_t, 1, 1> lastErrorCode = (uint16_t)0;
	/**
	error
	 * offset 148
	 */
	uint16_t recentErrorCode[8];
	/**
	val
	 * offset 164
	 */
	scaled_channel<float, 1, 1> debugFloatField1 = (float)0;
	/**
	val
	 * offset 168
	 */
	scaled_channel<float, 1, 1> debugFloatField2 = (float)0;
	/**
	val
	 * offset 172
	 */
	scaled_channel<float, 1, 1> debugFloatField3 = (float)0;
	/**
	val
	 * offset 176
	 */
	scaled_channel<float, 1, 1> debugFloatField4 = (float)0;
	/**
	val
	 * offset 180
	 */
	scaled_channel<float, 1, 1> debugFloatField5 = (float)0;
	/**
	val
	 * offset 184
	 */
	scaled_channel<float, 1, 1> debugFloatField6 = (float)0;
	/**
	val
	 * offset 188
	 */
	scaled_channel<float, 1, 1> debugFloatField7 = (float)0;
	/**
	val
	 * offset 192
	 */
	scaled_channel<uint32_t, 1, 1> debugIntField1 = (uint32_t)0;
	/**
	val
	 * offset 196
	 */
	scaled_channel<uint32_t, 1, 1> debugIntField2 = (uint32_t)0;
	/**
	val
	 * offset 200
	 */
	scaled_channel<uint32_t, 1, 1> debugIntField3 = (uint32_t)0;
	/**
	val
	 * offset 204
	 */
	scaled_channel<int16_t, 1, 1> debugIntField4 = (int16_t)0;
	/**
	val
	 * offset 206
	 */
	scaled_channel<int16_t, 1, 1> debugIntField5 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_X@@
	G
	 * offset 208
	 */
	scaled_channel<int16_t, 100, 1> accelerationX = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_Y@@
	G
	 * offset 210
	 */
	scaled_channel<int16_t, 100, 1> accelerationY = (int16_t)0;
	/**
	deg C
	 * offset 212
	 */
	uint16_t egt[EGT_CHANNEL_COUNT];
	/**
	 * @@GAUGE_NAME_TPS2@@
	%
	 * offset 228
	 */
	scaled_channel<int16_t, 100, 1> TPS2Value = (int16_t)0;
	/**
	V
	 * offset 230
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Primary = (int16_t)0;
	/**
	V
	 * offset 232
	 */
	scaled_channel<int16_t, 1000, 1> rawPpsPrimary = (int16_t)0;
	/**
	V
	 * offset 234
	 */
	scaled_channel<int16_t, 1000, 1> rawClt = (int16_t)0;
	/**
	V
	 * offset 236
	 */
	scaled_channel<int16_t, 1000, 1> rawIat = (int16_t)0;
	/**
	V
	 * offset 238
	 */
	scaled_channel<int16_t, 1000, 1> rawOilPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TUNE_CRC16@@
	crc16
	 * offset 240
	 */
	scaled_channel<uint16_t, 1, 1> tuneCrc16 = (uint16_t)0;
	/**
	 * offset 242
	 */
	scaled_channel<uint8_t, 1, 1> fuelClosedLoopBinIdx = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_CURRENT_GEAR@@
	gear
	 * offset 243
	 */
	scaled_channel<int8_t, 1, 1> tcuCurrentGear = (int8_t)0;
	/**
	V
	 * offset 244
	 */
	scaled_channel<int16_t, 1000, 1> rawPpsSecondary = (int16_t)0;
	/**
	dBv
	 * offset 246
	 */
	scaled_channel<int8_t, 1, 1> knock[12];
	/**
	 * @@GAUGE_NAME_DESIRED_GEAR@@
	gear
	 * offset 258
	 */
	scaled_channel<int8_t, 1, 1> tcuDesiredGear = (int8_t)0;
	/**
	 * @@GAUGE_NAME_FLEX@@
	%
	 * offset 259
	 */
	scaled_channel<uint8_t, 2, 1> flexPercent = (uint8_t)0;
	/**
	V
	 * offset 260
	 */
	scaled_channel<int16_t, 1000, 1> rawIdlePositionSensor = (int16_t)0;
	/**
	V
	 * offset 262
	 */
	scaled_channel<int16_t, 1000, 1> rawWastegatePosition = (int16_t)0;
	/**
	 * @@GAUGE_NAME_WG_POSITION@@
	%
	 * offset 264
	 */
	scaled_channel<int16_t, 100, 1> wastegatePositionSensor = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IDLE_POSITION@@
	%
	 * offset 266
	 */
	scaled_channel<int16_t, 100, 1> idlePositionSensor = (int16_t)0;
	/**
	V
	 * offset 268
	 */
	scaled_channel<int16_t, 1000, 1> rawLowFuelPressure = (int16_t)0;
	/**
	V
	 * offset 270
	 */
	scaled_channel<int16_t, 1000, 1> rawHighFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_LOW@@
	kpa
	 * offset 272
	 */
	scaled_channel<int16_t, 30, 1> lowFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_HIGH@@
	bar
	 * offset 274
	 */
	scaled_channel<int16_t, 10, 1> highFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_LAMBDA@@
	 * offset 276
	 */
	scaled_channel<uint16_t, 10000, 1> targetLambda = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR@@
	AFR
	 * offset 278
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue = (uint16_t)0;
	/**
	 * Vss Accel
	m/s2
	 * offset 280
	 */
	scaled_channel<uint16_t, 300, 1> VssAcceleration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA2@@
	 * offset 282
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR2@@
	AFR
	 * offset 284
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1E@@
	deg
	 * offset 286
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1E = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2I@@
	deg
	 * offset 288
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2E@@
	deg
	 * offset 290
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2E = (int16_t)0;
	/**
	%
	 * offset 292
	 */
	scaled_channel<int16_t, 100, 1> fuelPidCorrection[2];
	/**
	V
	 * offset 296
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Secondary = (int16_t)0;
	/**
	V
	 * offset 298
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Primary = (int16_t)0;
	/**
	V
	 * offset 300
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Secondary = (int16_t)0;
	/**
	 * @@GAUGE_NAME_KNOCK_COUNTER@@
	 * offset 302
	 */
	scaled_channel<uint16_t, 1, 1> knockCount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_Z@@
	G
	 * offset 304
	 */
	scaled_channel<int16_t, 100, 1> accelerationZ = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_ROLL@@
	G
	 * offset 306
	 */
	scaled_channel<int16_t, 100, 1> accelerationRoll = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_YAW@@
	G
	 * offset 308
	 */
	scaled_channel<int16_t, 100, 1> accelerationYaw = (int16_t)0;
	/**
	deg
	 * offset 310
	 */
	scaled_channel<int8_t, 1, 1> vvtTargets[4];
	/**
	 * @@GAUGE_NAME_TURBO_SPEED@@
	hz
	 * offset 314
	 */
	uint16_t turboSpeed = (uint16_t)0;
	/**
	%
	 * offset 316
	 */
	scaled_channel<int16_t, 100, 1> tps1Split = (int16_t)0;
	/**
	%
	 * offset 318
	 */
	scaled_channel<int16_t, 100, 1> tps2Split = (int16_t)0;
	/**
	%
	 * offset 320
	 */
	scaled_channel<int16_t, 100, 1> tps12Split = (int16_t)0;
	/**
	%
	 * offset 322
	 */
	scaled_channel<int16_t, 100, 1> accPedalSplit = (int16_t)0;
	/**
	 * Spark Cut Code
	code
	 * offset 324
	 */
	scaled_channel<int8_t, 1, 1> sparkCutReason = (int8_t)0;
	/**
	 * Fuel Cut Code
	code
	 * offset 325
	 */
	scaled_channel<int8_t, 1, 1> fuelCutReason = (int8_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_ESTIMATE@@
	kg/h
	 * offset 326
	 */
	scaled_channel<uint16_t, 10, 1> mafEstimate = (uint16_t)0;
	/**
	rpm
	 * offset 328
	 */
	uint16_t instantRpm = (uint16_t)0;
	/**
	counter
	 * offset 330
	 */
	uint16_t systemEventReuse = (uint16_t)0;
	/**
	V
	 * offset 332
	 */
	scaled_channel<uint16_t, 1000, 1> rawMap = (uint16_t)0;
	/**
	V
	 * offset 334
	 */
	scaled_channel<uint16_t, 1000, 1> rawAfr = (uint16_t)0;
	/**
	%
	 * offset 336
	 */
	uint8_t tpsAccelFrom = (uint8_t)0;
	/**
	%
	 * offset 337
	 */
	uint8_t tpsAccelTo = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 338
	 */
	uint8_t alignmentFill_at_338[2];
	/**
	 * offset 340
	 */
	scaled_channel<float, 1, 1> calibrationValue2 = (float)0;
	/**
	offset 344 bit 0 */
	bool isMainRelayOn : 1 {};
	/**
	 * Original reason for this is to check if USB is connected from Lua
	offset 344 bit 1 */
	bool isUsbConnected : 1 {};
	/**
	offset 344 bit 2 */
	bool unusedBit_172_2 : 1 {};
	/**
	offset 344 bit 3 */
	bool unusedBit_172_3 : 1 {};
	/**
	offset 344 bit 4 */
	bool unusedBit_172_4 : 1 {};
	/**
	offset 344 bit 5 */
	bool unusedBit_172_5 : 1 {};
	/**
	offset 344 bit 6 */
	bool unusedBit_172_6 : 1 {};
	/**
	offset 344 bit 7 */
	bool unusedBit_172_7 : 1 {};
	/**
	offset 344 bit 8 */
	bool unusedBit_172_8 : 1 {};
	/**
	offset 344 bit 9 */
	bool unusedBit_172_9 : 1 {};
	/**
	offset 344 bit 10 */
	bool unusedBit_172_10 : 1 {};
	/**
	offset 344 bit 11 */
	bool unusedBit_172_11 : 1 {};
	/**
	offset 344 bit 12 */
	bool unusedBit_172_12 : 1 {};
	/**
	offset 344 bit 13 */
	bool unusedBit_172_13 : 1 {};
	/**
	offset 344 bit 14 */
	bool unusedBit_172_14 : 1 {};
	/**
	offset 344 bit 15 */
	bool unusedBit_172_15 : 1 {};
	/**
	offset 344 bit 16 */
	bool unusedBit_172_16 : 1 {};
	/**
	offset 344 bit 17 */
	bool unusedBit_172_17 : 1 {};
	/**
	offset 344 bit 18 */
	bool unusedBit_172_18 : 1 {};
	/**
	offset 344 bit 19 */
	bool unusedBit_172_19 : 1 {};
	/**
	offset 344 bit 20 */
	bool unusedBit_172_20 : 1 {};
	/**
	offset 344 bit 21 */
	bool unusedBit_172_21 : 1 {};
	/**
	offset 344 bit 22 */
	bool unusedBit_172_22 : 1 {};
	/**
	offset 344 bit 23 */
	bool unusedBit_172_23 : 1 {};
	/**
	offset 344 bit 24 */
	bool unusedBit_172_24 : 1 {};
	/**
	offset 344 bit 25 */
	bool unusedBit_172_25 : 1 {};
	/**
	offset 344 bit 26 */
	bool unusedBit_172_26 : 1 {};
	/**
	offset 344 bit 27 */
	bool unusedBit_172_27 : 1 {};
	/**
	offset 344 bit 28 */
	bool unusedBit_172_28 : 1 {};
	/**
	offset 344 bit 29 */
	bool unusedBit_172_29 : 1 {};
	/**
	offset 344 bit 30 */
	bool unusedBit_172_30 : 1 {};
	/**
	offset 344 bit 31 */
	bool unusedBit_172_31 : 1 {};
	/**
	count
	 * offset 348
	 */
	int luaInvocationCounter = (int)0;
	/**
	nt
	 * offset 352
	 */
	int luaLastCycleDuration = (int)0;
	/**
	count
	 * offset 356
	 */
	uint8_t testBenchIter = (uint8_t)0;
	/**
	 * "Current Range"
	 * offset 357
	 */
	uint8_t tcu_currentRange = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_TC_RATIO@@
	value
	 * offset 358
	 */
	scaled_channel<uint16_t, 100, 1> tcRatio = (uint16_t)0;
	/**
	 * "S"
	 * offset 360
	 */
	float lastShiftTime = (float)0;
	/**
	 * offset 364
	 */
	int vssEdgeCounter = (int)0;
	/**
	 * offset 368
	 */
	int issEdgeCounter = (int)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_1@@
	 * offset 372
	 */
	scaled_channel<int16_t, 100, 1> auxLinear1 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_2@@
	 * offset 374
	 */
	scaled_channel<int16_t, 100, 1> auxLinear2 = (int16_t)0;
	/**
	offset 376 bit 0 */
	bool dfcoActive : 1 {};
	/**
	offset 376 bit 1 */
	bool tpsAccelActive : 1 {};
	/**
	offset 376 bit 2 */
	bool unusedBit_214_2 : 1 {};
	/**
	offset 376 bit 3 */
	bool unusedBit_214_3 : 1 {};
	/**
	offset 376 bit 4 */
	bool unusedBit_214_4 : 1 {};
	/**
	offset 376 bit 5 */
	bool unusedBit_214_5 : 1 {};
	/**
	offset 376 bit 6 */
	bool unusedBit_214_6 : 1 {};
	/**
	offset 376 bit 7 */
	bool unusedBit_214_7 : 1 {};
	/**
	offset 376 bit 8 */
	bool unusedBit_214_8 : 1 {};
	/**
	offset 376 bit 9 */
	bool unusedBit_214_9 : 1 {};
	/**
	offset 376 bit 10 */
	bool unusedBit_214_10 : 1 {};
	/**
	offset 376 bit 11 */
	bool unusedBit_214_11 : 1 {};
	/**
	offset 376 bit 12 */
	bool unusedBit_214_12 : 1 {};
	/**
	offset 376 bit 13 */
	bool unusedBit_214_13 : 1 {};
	/**
	offset 376 bit 14 */
	bool unusedBit_214_14 : 1 {};
	/**
	offset 376 bit 15 */
	bool unusedBit_214_15 : 1 {};
	/**
	offset 376 bit 16 */
	bool unusedBit_214_16 : 1 {};
	/**
	offset 376 bit 17 */
	bool unusedBit_214_17 : 1 {};
	/**
	offset 376 bit 18 */
	bool unusedBit_214_18 : 1 {};
	/**
	offset 376 bit 19 */
	bool unusedBit_214_19 : 1 {};
	/**
	offset 376 bit 20 */
	bool unusedBit_214_20 : 1 {};
	/**
	offset 376 bit 21 */
	bool unusedBit_214_21 : 1 {};
	/**
	offset 376 bit 22 */
	bool unusedBit_214_22 : 1 {};
	/**
	offset 376 bit 23 */
	bool unusedBit_214_23 : 1 {};
	/**
	offset 376 bit 24 */
	bool unusedBit_214_24 : 1 {};
	/**
	offset 376 bit 25 */
	bool unusedBit_214_25 : 1 {};
	/**
	offset 376 bit 26 */
	bool unusedBit_214_26 : 1 {};
	/**
	offset 376 bit 27 */
	bool unusedBit_214_27 : 1 {};
	/**
	offset 376 bit 28 */
	bool unusedBit_214_28 : 1 {};
	/**
	offset 376 bit 29 */
	bool unusedBit_214_29 : 1 {};
	/**
	offset 376 bit 30 */
	bool unusedBit_214_30 : 1 {};
	/**
	offset 376 bit 31 */
	bool unusedBit_214_31 : 1 {};
	/**
	 * @@GAUGE_NAME_BOOST_OUTPUT@@
	%
	 * offset 380
	 */
	scaled_channel<uint8_t, 2, 1> boostControllerOutput = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_BOOST_OPEN_LOOP@@
	%
	 * offset 381
	 */
	scaled_channel<uint8_t, 2, 1> boostControllerOpenLoopPart = (uint8_t)0;
	/**
	kPa
	 * offset 382
	 */
	scaled_channel<uint16_t, 10, 1> fallbackMap = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_DETECTED_GEAR@@
	 * offset 384
	 */
	uint8_t detectedGear = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 385
	 */
	uint8_t alignmentFill_at_385[1];
	/**
	 * Instant MAP
	kPa
	 * offset 386
	 */
	scaled_channel<uint16_t, 30, 1> instantMAPValue = (uint16_t)0;
	/**
	 * offset 388
	 */
	uint16_t mostRecentTimeBetweenSparkEvents = (uint16_t)0;
	/**
	 * offset 390
	 */
	uint16_t mostRecentTimeBetweenIgnitionEvents = (uint16_t)0;
	/**
	 * offset 392
	 */
	uint16_t maxLockedDuration = (uint16_t)0;
	/**
	 * offset 394
	 */
	uint16_t maxTriggerReentrant = (uint16_t)0;
	/**
	 * offset 396
	 */
	uint16_t canWriteOk = (uint16_t)0;
	/**
	 * offset 398
	 */
	uint16_t canWriteNotOk = (uint16_t)0;
	/**
	 * offset 400
	 */
	int triggerPrimaryFall = (int)0;
	/**
	 * offset 404
	 */
	int triggerPrimaryRise = (int)0;
	/**
	 * offset 408
	 */
	int triggerSecondaryFall = (int)0;
	/**
	 * offset 412
	 */
	int triggerSecondaryRise = (int)0;
	/**
	 * offset 416
	 */
	int triggerVvtFall = (int)0;
	/**
	 * offset 420
	 */
	int triggerVvtRise = (int)0;
	/**
	 * offset 424
	 */
	uint8_t startStopStateToggleCounter = (uint8_t)0;
	/**
	 * offset 425
	 */
	uint8_t starterState = (uint8_t)0;
	/**
	 * offset 426
	 */
	uint8_t starterRelayDisable = (uint8_t)0;
	/**
	 * offset 427
	 */
	uint8_t multiSparkCounter = (uint8_t)0;
	/**
	 * offset 428
	 */
	pid_status_s alternatorStatus;
	/**
	 * offset 444
	 */
	pid_status_s idleStatus;
	/**
	 * offset 460
	 */
	pid_status_s etbStatus;
	/**
	 * offset 476
	 */
	pid_status_s boostStatus;
	/**
	 * offset 492
	 */
	pid_status_s vvtStatus[CAM_INPUTS_COUNT];
	/**
	 * aux speed 1
	s
	 * offset 556
	 */
	uint16_t auxSpeed1 = (uint16_t)0;
	/**
	 * aux speed 2
	s
	 * offset 558
	 */
	uint16_t auxSpeed2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ISS@@
	RPM
	 * offset 560
	 */
	scaled_channel<uint16_t, 1, 1> ISSValue = (uint16_t)0;
	/**
	V
	 * offset 562
	 */
	scaled_channel<int16_t, 1000, 1> rawAnalogInput[AUX_ANALOG_INPUT_COUNT];
	/**
	 * GPPWM Output
	%
	 * offset 578
	 */
	scaled_channel<uint8_t, 2, 1> gppwmOutput[4];
	/**
	V
	 * offset 582
	 */
	scaled_channel<int16_t, 1000, 1> rawBattery = (int16_t)0;
	/**
	 * offset 584
	 */
	uint8_t extiOverflowCount = (uint8_t)0;
	/**
	 * offset 585
	 */
	uint8_t unusedAtTheEnd[159];
};
static_assert(sizeof(output_channels_s) == 744);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Sat Sep 03 21:41:58 UTC 2022
