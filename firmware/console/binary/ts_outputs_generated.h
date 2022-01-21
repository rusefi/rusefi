// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Fri Jan 21 20:56:35 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of pid_status_s
struct pid_status_s {
	/**
	 * offset 0
	 */
	float iTerm = (float)0;
	/**
	 * offset 4
	 */
	float dTerm = (float)0;
	/**
	 * offset 8
	 */
	float output = (float)0;
	/**
	 * offset 12
	 */
	float error = (float)0;
	/**
	 * offset 16
	 */
	int resetCounter = (int)0;
	/** total size 20*/
};

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
	 * "radiator fan"
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
	 * @@INDICATOR_NAME_CLUTCH_UP@@
	offset 0 bit 11 */
	bool clutchUpState : 1 {};
	/**
	 * @@INDICATOR_NAME_CLUTCH_DOWN@@
	offset 0 bit 12 */
	bool clutchDownState : 1 {};
	/**
	offset 0 bit 13 */
	bool isFan2On : 1 {};
	/**
	offset 0 bit 14 */
	bool alternatorOnOff : 1 {};
	/**
	 * @@INDICATOR_NAME_BRAKE_DOWN@@
	offset 0 bit 15 */
	bool brakePedalState : 1 {};
	/**
	offset 0 bit 16 */
	bool toothLogReady : 1 {};
	/**
	 * @@INDICATOR_NAME_AC_SWITCH@@
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
	deg
	 * offset 36
	 */
	scaled_channel<uint8_t, 10, 1> knockRetard = (uint8_t)0;
	/**
	 * offset 37
	 */
	uint8_t idleCurrentPosition = (uint8_t)0;
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
	 * @@GAUGE_NAME_FUEL_IAT_CORR@@
	%
	 * offset 64
	 */
	scaled_channel<uint16_t, 100, 1> iatCorrection = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_CLT_CORR@@
	%
	 * offset 66
	 */
	scaled_channel<uint16_t, 100, 1> cltCorrection = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_BARO_CORR@@
	%
	 * offset 68
	 */
	scaled_channel<uint16_t, 100, 1> baroCorrection = (uint16_t)0;
	/**
	 * "Engine Phase"
	deg
	 * offset 70
	 */
	scaled_channel<uint16_t, 1, 1> currentEnginePhase = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_CORRECTION@@
	mg
	 * offset 72
	 */
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_AMOUNT@@
	mg
	 * offset 74
	 */
	scaled_channel<int16_t, 100, 1> wallFuelCorrection = (int16_t)0;
	/**
	 * offset 76
	 */
	uint16_t revolutionCounterSinceStart = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_TPS_ROC@@
	ratio
	 * offset 78
	 */
	scaled_channel<int16_t, 100, 1> deltaTps = (int16_t)0;
	/**
	 * offset 80
	 */
	uint16_t canReadCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_TPS_EXTRA@@
	ms
	 * offset 82
	 */
	scaled_channel<int16_t, 300, 1> tpsAccelFuel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TIMING_ADVANCE@@
	deg
	 * offset 84
	 */
	scaled_channel<int16_t, 50, 1> ignitionAdvance = (int16_t)0;
	/**
	 * @@GAUGE_COIL_DWELL_TIME@@
	ms
	 * offset 86
	 */
	scaled_channel<uint16_t, 300, 1> sparkDwellValue = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_DWELL_DUTY@@
	%
	 * offset 88
	 */
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_IAC@@
	%
	 * offset 90
	 */
	scaled_channel<uint16_t, 100, 1> idleAirValvePosition = (uint16_t)0;
	/**
	 * "ETB Target"
	%
	 * offset 92
	 */
	scaled_channel<int16_t, 100, 1> etbTarget = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ETB_DUTY@@
	%
	 * offset 94
	 */
	scaled_channel<int16_t, 100, 1> etb1DutyCycle = (int16_t)0;
	/**
	 * "ETB Error"
	%
	 * offset 96
	 */
	scaled_channel<int16_t, 100, 1> etb1Error = (int16_t)0;
	/**
	 * "Fuel level"
	%
	 * offset 98
	 */
	scaled_channel<int16_t, 100, 1> fuelTankLevel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_CONSUMPTION@@
	grams
	 * offset 100
	 */
	scaled_channel<uint16_t, 1, 1> totalFuelConsumption = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_FLOW@@
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
	 * "Engine Mode"
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
	 * @@GAUGE_NAME_FUEL_LOAD@@
	%
	 * offset 134
	 */
	scaled_channel<uint16_t, 100, 1> fuelingLoad = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_IGNITION_LOAD@@
	%
	 * offset 136
	 */
	scaled_channel<uint16_t, 100, 1> ignitionLoad = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ENGINE_CRC16@@
	crc16
	 * offset 138
	 */
	scaled_channel<uint16_t, 1, 1> engineMakeCodeNameCrc16 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TRG_ERR@@
	counter
	 * offset 140
	 */
	scaled_channel<uint32_t, 1, 1> totalTriggerErrorCounter = (uint32_t)0;
	/**
	 * offset 144
	 */
	uint32_t orderingErrorCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_COUNTER@@
	count
	 * offset 148
	 */
	scaled_channel<uint16_t, 1, 1> warningCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_LAST@@
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
	scaled_channel<int16_t, 1, 1> debugIntField4 = (int16_t)0;
	/**
	val
	 * offset 210
	 */
	scaled_channel<int16_t, 1, 1> debugIntField5 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_X@@
	G
	 * offset 212
	 */
	scaled_channel<int16_t, 100, 1> accelerationX = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_Y@@
	G
	 * offset 214
	 */
	scaled_channel<int16_t, 100, 1> accelerationY = (int16_t)0;
	/**
	deg C
	 * offset 216
	 */
	uint16_t egt[EGT_CHANNEL_COUNT];
	/**
	 * @@GAUGE_NAME_TPS2@@
	%
	 * offset 232
	 */
	scaled_channel<int16_t, 100, 1> TPS2Value = (int16_t)0;
	/**
	V
	 * offset 234
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Primary = (int16_t)0;
	/**
	V
	 * offset 236
	 */
	scaled_channel<int16_t, 1000, 1> rawPpsPrimary = (int16_t)0;
	/**
	V
	 * offset 238
	 */
	scaled_channel<int16_t, 1000, 1> rawClt = (int16_t)0;
	/**
	V
	 * offset 240
	 */
	scaled_channel<int16_t, 1000, 1> rawIat = (int16_t)0;
	/**
	V
	 * offset 242
	 */
	scaled_channel<int16_t, 1000, 1> rawOilPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TUNE_CRC16@@
	crc16
	 * offset 244
	 */
	scaled_channel<uint16_t, 1, 1> tuneCrc16 = (uint16_t)0;
	/**
	 * offset 246
	 */
	scaled_channel<uint8_t, 1, 1> fuelClosedLoopBinIdx = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_CURRENT_GEAR@@
	gear
	 * offset 247
	 */
	scaled_channel<uint8_t, 1, 1> tcuCurrentGear = (uint8_t)0;
	/**
	V
	 * offset 248
	 */
	scaled_channel<int16_t, 1000, 1> rawPpsSecondary = (int16_t)0;
	/**
	 * offset 250
	 */
	scaled_channel<int8_t, 1, 1> knock[12];
	/**
	 * @@GAUGE_NAME_DESIRED_GEAR@@
	gear
	 * offset 262
	 */
	scaled_channel<uint8_t, 1, 1> tcuDesiredGear = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_FLEX@@
	%
	 * offset 263
	 */
	scaled_channel<uint8_t, 2, 1> flexPercent = (uint8_t)0;
	/**
	V
	 * offset 264
	 */
	scaled_channel<int16_t, 1000, 1> rawIdlePositionSensor = (int16_t)0;
	/**
	V
	 * offset 266
	 */
	scaled_channel<int16_t, 1000, 1> rawWastegatePosition = (int16_t)0;
	/**
	 * @@GAUGE_NAME_WG_POSITION@@
	%
	 * offset 268
	 */
	scaled_channel<int16_t, 100, 1> wastegatePositionSensor = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IDLE_POSITION@@
	%
	 * offset 270
	 */
	scaled_channel<int16_t, 100, 1> idlePositionSensor = (int16_t)0;
	/**
	V
	 * offset 272
	 */
	scaled_channel<int16_t, 1000, 1> rawLowFuelPressure = (int16_t)0;
	/**
	V
	 * offset 274
	 */
	scaled_channel<int16_t, 1000, 1> rawHighFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_LOW@@
	kpa
	 * offset 276
	 */
	scaled_channel<int16_t, 30, 1> lowFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_HIGH@@
	bar
	 * offset 278
	 */
	scaled_channel<int16_t, 10, 1> highFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_LAMBDA@@
	 * offset 280
	 */
	scaled_channel<uint16_t, 10000, 1> targetLambda = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR@@
	AFR
	 * offset 282
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue = (uint16_t)0;
	/**
	 * "Vss Accel"
	m/s2
	 * offset 284
	 */
	scaled_channel<uint16_t, 300, 1> VssAcceleration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA2@@
	 * offset 286
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR2@@
	AFR
	 * offset 288
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1E@@
	deg
	 * offset 290
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1E = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2I@@
	deg
	 * offset 292
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2E@@
	deg
	 * offset 294
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2E = (int16_t)0;
	/**
	%
	 * offset 296
	 */
	scaled_channel<int16_t, 100, 1> fuelPidCorrection[2];
	/**
	V
	 * offset 300
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Secondary = (int16_t)0;
	/**
	V
	 * offset 302
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Primary = (int16_t)0;
	/**
	V
	 * offset 304
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Secondary = (int16_t)0;
	/**
	 * "knock: count"
	 * offset 306
	 */
	uint16_t knockCount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_Z@@
	G
	 * offset 308
	 */
	scaled_channel<int16_t, 100, 1> accelerationZ = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_ROLL@@
	 * offset 310
	 */
	scaled_channel<int16_t, 100, 1> accelerationRoll = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_YAW@@
	 * offset 312
	 */
	scaled_channel<int16_t, 100, 1> accelerationYaw = (int16_t)0;
	/**
	deg
	 * offset 314
	 */
	scaled_channel<int8_t, 1, 1> vvtTargets[4];
	/**
	 * @@GAUGE_NAME_TURBO_SPEED@@
	hz
	 * offset 318
	 */
	uint16_t turboSpeed = (uint16_t)0;
	/**
	deg C
	 * offset 320
	 */
	uint16_t wbTemperature[2];
	/**
	%
	 * offset 324
	 */
	uint8_t wbHeaterDuty[2];
	/**
	%
	 * offset 326
	 */
	scaled_channel<int16_t, 100, 1> tps1Split = (int16_t)0;
	/**
	%
	 * offset 328
	 */
	scaled_channel<int16_t, 100, 1> tps2Split = (int16_t)0;
	/**
	%
	 * offset 330
	 */
	scaled_channel<int16_t, 100, 1> tps12Split = (int16_t)0;
	/**
	%
	 * offset 332
	 */
	scaled_channel<int16_t, 100, 1> accPedalSplit = (int16_t)0;
	/**
	 * "DI: Pump Angle"
	deg
	 * offset 334
	 */
	scaled_channel<int16_t, 50, 1> m_requested_pump = (int16_t)0;
	/**
	 * "Pump Angle"
	deg
	 * offset 336
	 */
	scaled_channel<int16_t, 30, 1> boostControlTarget = (int16_t)0;
	/**
	 * "Spark Cut Code"
	code
	 * offset 338
	 */
	int8_t sparkCutReason = (int8_t)0;
	/**
	 * "Fuel Cut Code"
	code
	 * offset 339
	 */
	int8_t fuelCutReason = (int8_t)0;
	/**
	 * "DI: fuel_requested_percent"
	v
	 * offset 340
	 */
	float fuel_requested_percent = (float)0;
	/**
	 * "DI: fuel_requested_percent_pi"
	v
	 * offset 344
	 */
	float fuel_requested_percent_pi = (float)0;
	/**
	 * "DI: m_I_sum_percent"
	v
	 * offset 348
	 */
	float m_I_sum_percent = (float)0;
	/**
	 * "DI: m_pressureTarget_kPa"
	v
	 * offset 352
	 */
	float m_pressureTarget_kPa = (float)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_ESTIMATE@@
	kg/h
	 * offset 356
	 */
	scaled_channel<uint16_t, 10, 1> mafEstimate = (uint16_t)0;
	/**
	rpm
	 * offset 358
	 */
	uint16_t instantRpm = (uint16_t)0;
	/**
	 * "DI: next start"
	v
	 * offset 360
	 */
	float di_nextStart = (float)0;
	/**
	counter
	 * offset 364
	 */
	uint16_t systemEventReuse = (uint16_t)0;
	/**
	V
	 * offset 366
	 */
	scaled_channel<uint16_t, 1000, 1> rawMap = (uint16_t)0;
	/**
	V
	 * offset 368
	 */
	scaled_channel<uint16_t, 1000, 1> rawAfr = (uint16_t)0;
	/**
	 * offset 370
	 */
	uint8_t unusedAtTheEnd[98];
	/**
	offset 468 bit 0 */
	bool launchSpeedCondition : 1 {};
	/**
	offset 468 bit 1 */
	bool launchRpmCondition : 1 {};
	/**
	offset 468 bit 2 */
	bool launchTpsCondition : 1 {};
	/**
	offset 468 bit 3 */
	bool launchActivateSwitchCondition : 1 {};
	/**
	offset 468 bit 4 */
	bool launchIsLaunchCondition : 1 {};
	/**
	offset 468 bit 5 */
	bool launchCombinedConditions : 1 {};
	/**
	offset 468 bit 6 */
	bool launchActivatePinState : 1 {};
	/**
	offset 468 bit 7 */
	bool unusedBit_185_7 : 1 {};
	/**
	offset 468 bit 8 */
	bool unusedBit_185_8 : 1 {};
	/**
	offset 468 bit 9 */
	bool unusedBit_185_9 : 1 {};
	/**
	offset 468 bit 10 */
	bool unusedBit_185_10 : 1 {};
	/**
	offset 468 bit 11 */
	bool unusedBit_185_11 : 1 {};
	/**
	offset 468 bit 12 */
	bool unusedBit_185_12 : 1 {};
	/**
	offset 468 bit 13 */
	bool unusedBit_185_13 : 1 {};
	/**
	offset 468 bit 14 */
	bool unusedBit_185_14 : 1 {};
	/**
	offset 468 bit 15 */
	bool unusedBit_185_15 : 1 {};
	/**
	offset 468 bit 16 */
	bool unusedBit_185_16 : 1 {};
	/**
	offset 468 bit 17 */
	bool unusedBit_185_17 : 1 {};
	/**
	offset 468 bit 18 */
	bool unusedBit_185_18 : 1 {};
	/**
	offset 468 bit 19 */
	bool unusedBit_185_19 : 1 {};
	/**
	offset 468 bit 20 */
	bool unusedBit_185_20 : 1 {};
	/**
	offset 468 bit 21 */
	bool unusedBit_185_21 : 1 {};
	/**
	offset 468 bit 22 */
	bool unusedBit_185_22 : 1 {};
	/**
	offset 468 bit 23 */
	bool unusedBit_185_23 : 1 {};
	/**
	offset 468 bit 24 */
	bool unusedBit_185_24 : 1 {};
	/**
	offset 468 bit 25 */
	bool unusedBit_185_25 : 1 {};
	/**
	offset 468 bit 26 */
	bool unusedBit_185_26 : 1 {};
	/**
	offset 468 bit 27 */
	bool unusedBit_185_27 : 1 {};
	/**
	offset 468 bit 28 */
	bool unusedBit_185_28 : 1 {};
	/**
	offset 468 bit 29 */
	bool unusedBit_185_29 : 1 {};
	/**
	offset 468 bit 30 */
	bool unusedBit_185_30 : 1 {};
	/**
	offset 468 bit 31 */
	bool unusedBit_185_31 : 1 {};
	/**
	 * offset 472
	 */
	uint8_t TEMPLOG_MAP_AT_CYCLE_COUNT = (uint8_t)0;
	/**
	 * offset 473
	 */
	uint8_t boostControllerOutput = (uint8_t)0;
	/**
	 * offset 474
	 */
	uint8_t boostControllerClosedLoopPart = (uint8_t)0;
	/**
	 * offset 475
	 */
	uint8_t boostControllerOpenLoopPart = (uint8_t)0;
	/**
	 * offset 476
	 */
	scaled_channel<float, 1, 1> TEMPLOG_MAP_AT_SPECIAL_POINT = (float)0;
	/**
	 * offset 480
	 */
	scaled_channel<float, 1, 1> TEMPLOG_MAP_AT_DIFF = (float)0;
	/**
	 * offset 484
	 */
	scaled_channel<float, 1, 1> vvtSyncGapRatio = (float)0;
	/**
	 * offset 488
	 */
	scaled_channel<float, 1, 1> vvtCurrentPosition = (float)0;
	/**
	 * offset 492
	 */
	scaled_channel<float, 1, 1> triggerSyncGapRatio = (float)0;
	/**
	 * offset 496
	 */
	uint8_t triggerStateIndex = (uint8_t)0;
	/**
	 * offset 497
	 */
	uint8_t vvtCounter = (uint8_t)0;
	/**
	 * offset 498
	 */
	uint8_t vvtSyncCounter = (uint8_t)0;
	/**
	 * offset 499
	 */
	uint8_t vvtStateIndex = (uint8_t)0;
	/**
	 * offset 500
	 */
	scaled_channel<float, 1, 1> TEMPLOG_MAP_INSTANT_AVERAGE = (float)0;
	/**
	 * offset 504
	 */
	uint8_t fallbackMap = (uint8_t)0;
	/**
	 * offset 505
	 */
	uint8_t TEMPLOG_map_peak = (uint8_t)0;
	/**
	 * offset 506
	 */
	uint8_t TEMPLOG_map_length = (uint8_t)0;
	/**
	 * offset 507
	 */
	uint8_t TEMPLOG_mapIndex = (uint8_t)0;
	/**
	%
	 * offset 508
	 */
	scaled_channel<uint16_t, 100, 1> timingCltCorrection = (uint16_t)0;
	/**
	%
	 * offset 510
	 */
	scaled_channel<uint16_t, 100, 1> timingIatCorrection = (uint16_t)0;
	/**
	%
	 * offset 512
	 */
	scaled_channel<uint16_t, 100, 1> timingPidCorrection = (uint16_t)0;
	/**
	 * Instant MAP
	kPa
	 * offset 514
	 */
	scaled_channel<uint16_t, 30, 1> instantMAPValue = (uint16_t)0;
	/**
	 * offset 516
	 */
	uint16_t mostRecentTimeBetweenSparkEvents = (uint16_t)0;
	/**
	 * offset 518
	 */
	uint16_t mostRecentTimeBetweenIgnitionEvents = (uint16_t)0;
	/**
	 * offset 520
	 */
	uint16_t maxLockedDuration = (uint16_t)0;
	/**
	 * offset 522
	 */
	uint16_t maxTriggerReentrant = (uint16_t)0;
	/**
	 * offset 524
	 */
	uint16_t canWriteOk = (uint16_t)0;
	/**
	 * offset 526
	 */
	uint16_t canWriteNotOk = (uint16_t)0;
	/**
	 * offset 528
	 */
	int triggerPrimaryFall = (int)0;
	/**
	 * offset 532
	 */
	int triggerPrimaryRise = (int)0;
	/**
	 * offset 536
	 */
	int triggerSecondaryFall = (int)0;
	/**
	 * offset 540
	 */
	int triggerSecondaryRise = (int)0;
	/**
	 * offset 544
	 */
	int triggerVvtFall = (int)0;
	/**
	 * offset 548
	 */
	int triggerVvtRise = (int)0;
	/**
	 * offset 552
	 */
	uint8_t startStopStateToggleCounter = (uint8_t)0;
	/**
	 * offset 553
	 */
	uint8_t starterState = (uint8_t)0;
	/**
	 * offset 554
	 */
	uint8_t starterRelayDisable = (uint8_t)0;
	/**
	 * offset 555
	 */
	uint8_t multiSparkCounter = (uint8_t)0;
	/**
	 * offset 556
	 */
	int idleState = (int)0;
	/**
	 * offset 560
	 */
	float injectorFlowPressureRatio = (float)0;
	/**
	 * offset 564
	 */
	float injectorFlowPressureDelta = (float)0;
	/**
	 * offset 568
	 */
	float etbFeedForward = (float)0;
	/**
	 * offset 572
	 */
	float etbIntegralError = (float)0;
	/**
	 * offset 576
	 */
	float etbCurrentTarget = (float)0;
	/**
	 * offset 580
	 */
	pid_status_s alternatorStatus;
	/**
	 * offset 600
	 */
	pid_status_s idleStatus;
	/**
	 * offset 620
	 */
	pid_status_s etbStatus;
	/** total size 640*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Fri Jan 21 20:56:35 UTC 2022
