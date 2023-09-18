// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) console/binary/output_channels.txt Mon Sep 18 12:56:47 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of pid_status_s
struct pid_status_s {
	/**
	 * offset 0
	 */
	float pTerm = (float)0;
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
	 * SD: Present
	offset 0 bit 0 */
	bool sd_present : 1 {};
	/**
	 * SD: Logging
	offset 0 bit 1 */
	bool sd_logging_internal : 1 {};
	/**
	offset 0 bit 2 */
	bool triggerScopeReady : 1 {};
	/**
	offset 0 bit 3 */
	bool antilagTriggered : 1 {};
	/**
	 * Radiator Fan
	offset 0 bit 4 */
	bool isFanOn : 1 {};
	/**
	offset 0 bit 5 */
	bool isO2HeaterOn : 1 {};
	/**
	offset 0 bit 6 */
	bool checkEngine : 1 {};
	/**
	offset 0 bit 7 */
	bool needBurn : 1 {};
	/**
	 * SD: MSD
	offset 0 bit 8 */
	bool sd_msd : 1 {};
	/**
	 * Radiator Fan 2
	offset 0 bit 9 */
	bool isFan2On : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBitLAtwfs : 1 {};
	/**
	 * Tooth Logger Ready
	offset 0 bit 11 */
	bool toothLogReady : 1 {};
	/**
	 * Error: TPS
	offset 0 bit 12 */
	bool isTpsError : 1 {};
	/**
	 * Error: CLT
	offset 0 bit 13 */
	bool isCltError : 1 {};
	/**
	 * Error: MAP
	offset 0 bit 14 */
	bool isMapError : 1 {};
	/**
	 * Error: IAT
	offset 0 bit 15 */
	bool isIatError : 1 {};
	/**
	 * Error: Trigger
	offset 0 bit 16 */
	bool isTriggerError : 1 {};
	/**
	 * Error: Active
	offset 0 bit 17 */
	bool hasCriticalError : 1 {};
	/**
	 * Warning: Active
	offset 0 bit 18 */
	bool isWarnNow : 1 {};
	/**
	 * Error: Pedal
	offset 0 bit 19 */
	bool isPedalError : 1 {};
	/**
	offset 0 bit 20 */
	bool isKnockChipOk : 1 {};
	/**
	 * Launch Control Triggered
	offset 0 bit 21 */
	bool launchTriggered : 1 {};
	/**
	 * Error: TPS2
	offset 0 bit 22 */
	bool isTps2Error : 1 {};
	/**
	 * Idle: Closed Loop
	offset 0 bit 23 */
	bool isIdleClosedLoop : 1 {};
	/**
	 * Injector Fault
	offset 0 bit 24 */
	bool injectorFault : 1 {};
	/**
	 * Ignition Fault
	offset 0 bit 25 */
	bool ignitionFault : 1 {};
	/**
	offset 0 bit 26 */
	bool isMainRelayOn : 1 {};
	/**
	 * isUsbConnected
	 * Original reason for this is to check if USB is connected from Lua
	offset 0 bit 27 */
	bool isUsbConnected : 1 {};
	/**
	offset 0 bit 28 */
	bool dfcoActive : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_29_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_29_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_29_31 : 1 {};
	/**
	 * @@GAUGE_NAME_RPM@@
	RPM
	 * offset 4
	 */
	uint16_t RPMValue = (uint16_t)0;
	/**
	 * dRPM
	RPM acceleration
	 * offset 6
	 */
	int16_t rpmAcceleration = (int16_t)0;
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
	uint8_t unusedVehicleSpeedKph = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_CPU_TEMP@@
	deg C
	 * offset 11
	 */
	int8_t internalMcuTemperature = (int8_t)0;
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
	 * need 4 byte alignment
	units
	 * offset 37
	 */
	uint8_t alignmentFill_at_37[1];
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
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION@@
	 * Actual last injection time - including all compensation and injection mode
	ms
	 * offset 44
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjection = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_INJ_DUTY@@
	%
	 * offset 46
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycle = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_VE@@
	ratio
	 * offset 47
	 */
	scaled_channel<uint8_t, 2, 1> veValue = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_INJECTION_TIMING@@
	deg
	 * offset 48
	 */
	int16_t injectionOffset = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ENGINE_CRC16@@
	crc16
	 * offset 50
	 */
	uint16_t engineMakeCodeNameCrc16 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_AMOUNT@@
	mg
	 * offset 52
	 */
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_CORRECTION@@
	mg
	 * offset 54
	 */
	scaled_channel<int16_t, 100, 1> wallFuelCorrectionValue = (int16_t)0;
	/**
	 * offset 56
	 */
	uint16_t revolutionCounterSinceStart = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_READ_OK@@
	 * offset 58
	 */
	uint16_t canReadCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_TPS_EXTRA@@
	ms
	 * offset 60
	 */
	scaled_channel<int16_t, 300, 1> tpsAccelFuel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IGNITION_MODE@@
	 * offset 62
	 */
	uint8_t currentIgnitionMode = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_INJECTION_MODE@@
	 * offset 63
	 */
	uint8_t currentInjectionMode = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_DWELL_DUTY@@
	%
	 * offset 64
	 */
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_IAC@@
	%
	 * offset 66
	 */
	scaled_channel<uint16_t, 100, 1> idleAirValvePosition = (uint16_t)0;
	/**
	 * ETB Target
	%
	 * offset 68
	 */
	scaled_channel<int16_t, 100, 1> etbTarget = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ETB_DUTY@@
	%
	 * offset 70
	 */
	scaled_channel<int16_t, 100, 1> etb1DutyCycle = (int16_t)0;
	/**
	 * Fuel level
	%
	 * offset 72
	 */
	scaled_channel<int16_t, 100, 1> fuelTankLevel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_CONSUMPTION@@
	grams
	 * offset 74
	 */
	uint16_t totalFuelConsumption = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_FLOW@@
	gram/s
	 * offset 76
	 */
	scaled_channel<uint16_t, 200, 1> fuelFlowRate = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TPS2@@
	%
	 * offset 78
	 */
	scaled_channel<int16_t, 100, 1> TPS2Value = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TUNE_CRC16@@
	crc16
	 * offset 80
	 */
	uint16_t tuneCrc16 = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 82
	 */
	uint8_t alignmentFill_at_82[2];
	/**
	 * @@GAUGE_NAME_KNOCK_LEVEL@@
	Volts
	 * offset 84
	 */
	float knockLevel = (float)0;
	/**
	 * @@GAUGE_NAME_UPTIME@@
	sec
	 * offset 88
	 */
	uint32_t seconds = (uint32_t)0;
	/**
	 * Engine Mode
	em
	 * offset 92
	 */
	uint32_t engineMode = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_VERSION@@
	version_f
	 * offset 96
	 */
	uint32_t firmwareVersion = (uint32_t)0;
	/**
	V
	 * offset 100
	 */
	scaled_channel<int16_t, 1000, 1> rawIdlePositionSensor = (int16_t)0;
	/**
	V
	 * offset 102
	 */
	scaled_channel<int16_t, 1000, 1> rawWastegatePosition = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_X@@
	G
	 * offset 104
	 */
	scaled_channel<int16_t, 100, 1> accelerationX = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_Y@@
	G
	 * offset 106
	 */
	scaled_channel<int16_t, 100, 1> accelerationY = (int16_t)0;
	/**
	 * @@GAUGE_NAME_DETECTED_GEAR@@
	 * offset 108
	 */
	uint8_t detectedGear = (uint8_t)0;
	/**
	 * offset 109
	 */
	uint8_t maxTriggerReentrant = (uint8_t)0;
	/**
	V
	 * offset 110
	 */
	scaled_channel<int16_t, 1000, 1> rawLowFuelPressure = (int16_t)0;
	/**
	V
	 * offset 112
	 */
	scaled_channel<int16_t, 1000, 1> rawHighFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_LOW@@
	kpa
	 * offset 114
	 */
	scaled_channel<int16_t, 30, 1> lowFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_DESIRED_GEAR@@
	gear
	 * offset 116
	 */
	int8_t tcuDesiredGear = (int8_t)0;
	/**
	 * @@GAUGE_NAME_FLEX@@
	%
	 * offset 117
	 */
	scaled_channel<uint8_t, 2, 1> flexPercent = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_WG_POSITION@@
	%
	 * offset 118
	 */
	scaled_channel<int16_t, 100, 1> wastegatePositionSensor = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_HIGH@@
	bar
	 * offset 120
	 */
	scaled_channel<int16_t, 10, 1> highFuelPressure = (int16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 122
	 */
	uint8_t alignmentFill_at_122[2];
	/**
	 * offset 124
	 */
	uint32_t tsConfigVersion = (uint32_t)0;
	/**
	 * offset 128
	 */
	float calibrationValue = (float)0;
	/**
	 * offset 132
	 */
	uint8_t calibrationMode = (uint8_t)0;
	/**
	 * Idle: Stepper target position
	 * offset 133
	 */
	uint8_t idleTargetPosition = (uint8_t)0;
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
	uint32_t totalTriggerErrorCounter = (uint32_t)0;
	/**
	 * offset 140
	 */
	uint32_t orderingErrorCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_COUNTER@@
	count
	 * offset 144
	 */
	uint16_t warningCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_LAST@@
	error
	 * offset 146
	 */
	uint16_t lastErrorCode = (uint16_t)0;
	/**
	error
	 * offset 148
	 */
	uint16_t recentErrorCode[8];
	/**
	val
	 * offset 164
	 */
	float debugFloatField1 = (float)0;
	/**
	val
	 * offset 168
	 */
	float debugFloatField2 = (float)0;
	/**
	val
	 * offset 172
	 */
	float debugFloatField3 = (float)0;
	/**
	val
	 * offset 176
	 */
	float debugFloatField4 = (float)0;
	/**
	val
	 * offset 180
	 */
	float debugFloatField5 = (float)0;
	/**
	val
	 * offset 184
	 */
	float debugFloatField6 = (float)0;
	/**
	val
	 * offset 188
	 */
	float debugFloatField7 = (float)0;
	/**
	val
	 * offset 192
	 */
	uint32_t debugIntField1 = (uint32_t)0;
	/**
	val
	 * offset 196
	 */
	uint32_t debugIntField2 = (uint32_t)0;
	/**
	val
	 * offset 200
	 */
	uint32_t debugIntField3 = (uint32_t)0;
	/**
	val
	 * offset 204
	 */
	int16_t debugIntField4 = (int16_t)0;
	/**
	val
	 * offset 206
	 */
	int16_t debugIntField5 = (int16_t)0;
	/**
	 * EGT
	deg C
	 * offset 208
	 */
	uint16_t egt[EGT_CHANNEL_COUNT];
	/**
	V
	 * offset 224
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Primary = (int16_t)0;
	/**
	V
	 * offset 226
	 */
	scaled_channel<int16_t, 1000, 1> rawPpsPrimary = (int16_t)0;
	/**
	V
	 * offset 228
	 */
	scaled_channel<int16_t, 1000, 1> rawClt = (int16_t)0;
	/**
	V
	 * offset 230
	 */
	scaled_channel<int16_t, 1000, 1> rawIat = (int16_t)0;
	/**
	V
	 * offset 232
	 */
	scaled_channel<int16_t, 1000, 1> rawOilPressure = (int16_t)0;
	/**
	 * offset 234
	 */
	uint8_t fuelClosedLoopBinIdx = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_CURRENT_GEAR@@
	gear
	 * offset 235
	 */
	int8_t tcuCurrentGear = (int8_t)0;
	/**
	V
	 * offset 236
	 */
	scaled_channel<int16_t, 1000, 1> rawPpsSecondary = (int16_t)0;
	/**
	 * Knock: Cyl
	dBv
	 * offset 238
	 */
	int8_t knock[12];
	/**
	 * @@GAUGE_NAME_IDLE_POSITION@@
	%
	 * offset 250
	 */
	scaled_channel<int16_t, 100, 1> idlePositionSensor = (int16_t)0;
	/**
	 * @@GAUGE_NAME_AFR@@
	AFR
	 * offset 252
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue = (uint16_t)0;
	/**
	 * Vss Accel
	m/s2
	 * offset 254
	 */
	scaled_channel<uint16_t, 300, 1> VssAcceleration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA2@@
	 * offset 256
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR2@@
	AFR
	 * offset 258
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1E@@
	deg
	 * offset 260
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1E = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2I@@
	deg
	 * offset 262
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2E@@
	deg
	 * offset 264
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2E = (int16_t)0;
	/**
	 * Fuel: Trim bank
	%
	 * offset 266
	 */
	scaled_channel<int16_t, 100, 1> fuelPidCorrection[2];
	/**
	V
	 * offset 270
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Secondary = (int16_t)0;
	/**
	V
	 * offset 272
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Primary = (int16_t)0;
	/**
	V
	 * offset 274
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Secondary = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_Z@@
	G
	 * offset 276
	 */
	scaled_channel<int16_t, 100, 1> accelerationZ = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_ROLL@@
	G
	 * offset 278
	 */
	scaled_channel<int16_t, 100, 1> accelerationRoll = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_YAW@@
	G
	 * offset 280
	 */
	scaled_channel<int16_t, 100, 1> accelerationYaw = (int16_t)0;
	/**
	deg
	 * offset 282
	 */
	int8_t vvtTargets[4];
	/**
	 * @@GAUGE_NAME_TURBO_SPEED@@
	hz
	 * offset 286
	 */
	uint16_t turboSpeed = (uint16_t)0;
	/**
	 * Ign: Timing Cyl
	deg
	 * offset 288
	 */
	scaled_channel<int16_t, 50, 1> ignitionAdvanceCyl[MAX_CYLINDER_COUNT];
	/**
	%
	 * offset 312
	 */
	scaled_channel<int16_t, 100, 1> tps1Split = (int16_t)0;
	/**
	%
	 * offset 314
	 */
	scaled_channel<int16_t, 100, 1> tps2Split = (int16_t)0;
	/**
	%
	 * offset 316
	 */
	scaled_channel<int16_t, 100, 1> tps12Split = (int16_t)0;
	/**
	%
	 * offset 318
	 */
	scaled_channel<int16_t, 100, 1> accPedalSplit = (int16_t)0;
	/**
	 * Ign: Cut Code
	code
	 * offset 320
	 */
	int8_t sparkCutReason = (int8_t)0;
	/**
	 * Fuel: Cut Code
	code
	 * offset 321
	 */
	int8_t fuelCutReason = (int8_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_ESTIMATE@@
	kg/h
	 * offset 322
	 */
	scaled_channel<uint16_t, 10, 1> mafEstimate = (uint16_t)0;
	/**
	 * sync: instant RPM
	rpm
	 * offset 324
	 */
	uint16_t instantRpm = (uint16_t)0;
	/**
	V
	 * offset 326
	 */
	scaled_channel<uint16_t, 1000, 1> rawMap = (uint16_t)0;
	/**
	V
	 * offset 328
	 */
	scaled_channel<uint16_t, 1000, 1> rawAfr = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 330
	 */
	uint8_t alignmentFill_at_330[2];
	/**
	 * offset 332
	 */
	float calibrationValue2 = (float)0;
	/**
	 * Lua: Tick counter
	count
	 * offset 336
	 */
	uint32_t luaInvocationCounter = (uint32_t)0;
	/**
	 * Lua: Last tick duration
	nt
	 * offset 340
	 */
	uint32_t luaLastCycleDuration = (uint32_t)0;
	/**
	 * "TCU: Current Range"
	 * offset 344
	 */
	uint8_t tcu_currentRange = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 345
	 */
	uint8_t alignmentFill_at_345[1];
	/**
	 * @@GAUGE_NAME_TC_RATIO@@
	value
	 * offset 346
	 */
	scaled_channel<uint16_t, 100, 1> tcRatio = (uint16_t)0;
	/**
	 * offset 348
	 */
	float lastShiftTime = (float)0;
	/**
	 * offset 352
	 */
	uint32_t vssEdgeCounter = (uint32_t)0;
	/**
	 * offset 356
	 */
	uint32_t issEdgeCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_1@@
	 * offset 360
	 */
	scaled_channel<int16_t, 100, 1> auxLinear1 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_2@@
	 * offset 362
	 */
	scaled_channel<int16_t, 100, 1> auxLinear2 = (int16_t)0;
	/**
	kPa
	 * offset 364
	 */
	scaled_channel<uint16_t, 10, 1> fallbackMap = (uint16_t)0;
	/**
	 * Instant MAP
	kPa
	 * offset 366
	 */
	scaled_channel<uint16_t, 30, 1> instantMAPValue = (uint16_t)0;
	/**
	us
	 * offset 368
	 */
	uint16_t maxLockedDuration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_WRITE_OK@@
	 * offset 370
	 */
	uint16_t canWriteOk = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_WRITE_ERR@@
	 * offset 372
	 */
	uint16_t canWriteNotOk = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 374
	 */
	uint8_t alignmentFill_at_374[2];
	/**
	 * offset 376
	 */
	uint32_t triggerPrimaryFall = (uint32_t)0;
	/**
	 * offset 380
	 */
	uint32_t triggerPrimaryRise = (uint32_t)0;
	/**
	 * offset 384
	 */
	uint32_t triggerSecondaryFall = (uint32_t)0;
	/**
	 * offset 388
	 */
	uint32_t triggerSecondaryRise = (uint32_t)0;
	/**
	 * offset 392
	 */
	uint16_t triggerVvtFall = (uint16_t)0;
	/**
	 * offset 394
	 */
	uint16_t triggerVvtRise = (uint16_t)0;
	/**
	 * offset 396
	 */
	uint16_t triggerVvt2Fall = (uint16_t)0;
	/**
	 * offset 398
	 */
	uint16_t triggerVvt2Rise = (uint16_t)0;
	/**
	 * offset 400
	 */
	uint8_t starterState = (uint8_t)0;
	/**
	 * offset 401
	 */
	uint8_t starterRelayDisable = (uint8_t)0;
	/**
	 * Ign: Multispark count
	 * offset 402
	 */
	uint8_t multiSparkCounter = (uint8_t)0;
	/**
	 * offset 403
	 */
	uint8_t extiOverflowCount = (uint8_t)0;
	/**
	 * offset 404
	 */
	pid_status_s alternatorStatus;
	/**
	 * offset 420
	 */
	pid_status_s idleStatus;
	/**
	 * offset 436
	 */
	pid_status_s etbStatus;
	/**
	 * offset 452
	 */
	pid_status_s boostStatus;
	/**
	 * offset 468
	 */
	pid_status_s wastegateDcStatus;
	/**
	 * offset 484
	 */
	pid_status_s vvtStatus[CAM_INPUTS_COUNT];
	/**
	 * Aux speed 1
	s
	 * offset 548
	 */
	uint16_t auxSpeed1 = (uint16_t)0;
	/**
	 * Aux speed 2
	s
	 * offset 550
	 */
	uint16_t auxSpeed2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ISS@@
	RPM
	 * offset 552
	 */
	uint16_t ISSValue = (uint16_t)0;
	/**
	V
	 * offset 554
	 */
	scaled_channel<int16_t, 1000, 1> rawAnalogInput[LUA_ANALOG_INPUT_COUNT];
	/**
	 * GPPWM Output
	%
	 * offset 570
	 */
	scaled_channel<uint8_t, 2, 1> gppwmOutput[4];
	/**
	 * offset 574
	 */
	scaled_channel<int16_t, 1, 1> gppwmXAxis[4];
	/**
	 * offset 582
	 */
	scaled_channel<int16_t, 10, 1> gppwmYAxis[4];
	/**
	V
	 * offset 590
	 */
	scaled_channel<int16_t, 1000, 1> rawBattery = (int16_t)0;
	/**
	 * offset 592
	 */
	scaled_channel<int16_t, 10, 1> ignBlendParameter[IGN_BLEND_COUNT];
	/**
	%
	 * offset 600
	 */
	scaled_channel<uint8_t, 2, 1> ignBlendBias[IGN_BLEND_COUNT];
	/**
	deg
	 * offset 604
	 */
	scaled_channel<int16_t, 100, 1> ignBlendOutput[IGN_BLEND_COUNT];
	/**
	 * offset 612
	 */
	scaled_channel<int16_t, 10, 1> veBlendParameter[VE_BLEND_COUNT];
	/**
	%
	 * offset 620
	 */
	scaled_channel<uint8_t, 2, 1> veBlendBias[VE_BLEND_COUNT];
	/**
	%
	 * offset 624
	 */
	scaled_channel<int16_t, 100, 1> veBlendOutput[VE_BLEND_COUNT];
	/**
	 * offset 632
	 */
	scaled_channel<int16_t, 10, 1> boostOpenLoopBlendParameter[BOOST_BLEND_COUNT];
	/**
	%
	 * offset 636
	 */
	scaled_channel<uint8_t, 2, 1> boostOpenLoopBlendBias[BOOST_BLEND_COUNT];
	/**
	%
	 * offset 638
	 */
	scaled_channel<int8_t, 1, 1> boostOpenLoopBlendOutput[BOOST_BLEND_COUNT];
	/**
	 * offset 640
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendParameter[BOOST_BLEND_COUNT];
	/**
	%
	 * offset 644
	 */
	scaled_channel<uint8_t, 2, 1> boostClosedLoopBlendBias[BOOST_BLEND_COUNT];
	/**
	%
	 * offset 646
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendOutput[BOOST_BLEND_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 650
	 */
	uint8_t alignmentFill_at_650[2];
	/**
	offset 652 bit 0 */
	bool coilState1 : 1 {};
	/**
	offset 652 bit 1 */
	bool coilState2 : 1 {};
	/**
	offset 652 bit 2 */
	bool coilState3 : 1 {};
	/**
	offset 652 bit 3 */
	bool coilState4 : 1 {};
	/**
	offset 652 bit 4 */
	bool coilState5 : 1 {};
	/**
	offset 652 bit 5 */
	bool coilState6 : 1 {};
	/**
	offset 652 bit 6 */
	bool coilState7 : 1 {};
	/**
	offset 652 bit 7 */
	bool coilState8 : 1 {};
	/**
	offset 652 bit 8 */
	bool coilState9 : 1 {};
	/**
	offset 652 bit 9 */
	bool coilState10 : 1 {};
	/**
	offset 652 bit 10 */
	bool coilState11 : 1 {};
	/**
	offset 652 bit 11 */
	bool coilState12 : 1 {};
	/**
	offset 652 bit 12 */
	bool injectorState1 : 1 {};
	/**
	offset 652 bit 13 */
	bool injectorState2 : 1 {};
	/**
	offset 652 bit 14 */
	bool injectorState3 : 1 {};
	/**
	offset 652 bit 15 */
	bool injectorState4 : 1 {};
	/**
	offset 652 bit 16 */
	bool injectorState5 : 1 {};
	/**
	offset 652 bit 17 */
	bool injectorState6 : 1 {};
	/**
	offset 652 bit 18 */
	bool injectorState7 : 1 {};
	/**
	offset 652 bit 19 */
	bool injectorState8 : 1 {};
	/**
	offset 652 bit 20 */
	bool injectorState9 : 1 {};
	/**
	offset 652 bit 21 */
	bool injectorState10 : 1 {};
	/**
	offset 652 bit 22 */
	bool injectorState11 : 1 {};
	/**
	offset 652 bit 23 */
	bool injectorState12 : 1 {};
	/**
	offset 652 bit 24 */
	bool triggerChannel1 : 1 {};
	/**
	offset 652 bit 25 */
	bool triggerChannel2 : 1 {};
	/**
	offset 652 bit 26 */
	bool vvtChannel1 : 1 {};
	/**
	offset 652 bit 27 */
	bool vvtChannel2 : 1 {};
	/**
	offset 652 bit 28 */
	bool vvtChannel3 : 1 {};
	/**
	offset 652 bit 29 */
	bool vvtChannel4 : 1 {};
	/**
	offset 652 bit 30 */
	bool unusedBit_241_30 : 1 {};
	/**
	offset 652 bit 31 */
	bool unusedBit_241_31 : 1 {};
	/**
	 * offset 656
	 */
	uint32_t outputRequestPeriod = (uint32_t)0;
	/**
	 * offset 660
	 */
	float mapFast = (float)0;
	/**
	 * Lua: Gauge
	value
	 * offset 664
	 */
	float luaGauges[LUA_GAUGE_COUNT];
	/**
	V
	 * offset 672
	 */
	scaled_channel<uint16_t, 1000, 1> rawMaf2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_MEASURED_2@@
	kg/h
	 * offset 674
	 */
	scaled_channel<uint16_t, 10, 1> mafMeasured2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVS@@
	kph
	 * offset 676
	 */
	scaled_channel<uint16_t, 100, 1> vehicleSpeedKph = (uint16_t)0;
	/**
	%
	 * offset 678
	 */
	scaled_channel<uint16_t, 100, 1> Gego = (uint16_t)0;
	/**
	count
	 * offset 680
	 */
	uint16_t testBenchIter = (uint16_t)0;
	/**
	deg C
	 * offset 682
	 */
	scaled_channel<int16_t, 100, 1> oilTemp = (int16_t)0;
	/**
	deg C
	 * offset 684
	 */
	scaled_channel<int16_t, 100, 1> fuelTemp = (int16_t)0;
	/**
	deg C
	 * offset 686
	 */
	scaled_channel<int16_t, 100, 1> ambientTemp = (int16_t)0;
	/**
	deg C
	 * offset 688
	 */
	scaled_channel<int16_t, 100, 1> compressorDischargeTemp = (int16_t)0;
	/**
	 * offset 690
	 */
	uint8_t unusedAtTheEnd[132];
	/**
	 * need 4 byte alignment
	units
	 * offset 822
	 */
	uint8_t alignmentFill_at_822[2];
};
static_assert(sizeof(output_channels_s) == 824);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) console/binary/output_channels.txt Mon Sep 18 12:56:47 UTC 2023
