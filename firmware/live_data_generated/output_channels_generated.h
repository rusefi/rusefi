// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) console/binary/output_channels.txt
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
	offset 0 bit 4 */
	bool isO2HeaterOn : 1 {};
	/**
	offset 0 bit 5 */
	bool checkEngine : 1 {};
	/**
	offset 0 bit 6 */
	bool needBurn : 1 {};
	/**
	 * SD: MSD
	offset 0 bit 7 */
	bool sd_msd : 1 {};
	/**
	 * Tooth Logger Ready
	offset 0 bit 8 */
	bool toothLogReady : 1 {};
	/**
	 * Error: TPS
	offset 0 bit 9 */
	bool isTpsError : 1 {};
	/**
	 * Error: CLT
	offset 0 bit 10 */
	bool isCltError : 1 {};
	/**
	 * Error: MAP
	offset 0 bit 11 */
	bool isMapError : 1 {};
	/**
	 * Error: IAT
	offset 0 bit 12 */
	bool isIatError : 1 {};
	/**
	 * Error: Trigger
	offset 0 bit 13 */
	bool isTriggerError : 1 {};
	/**
	 * Error: Active
	offset 0 bit 14 */
	bool hasCriticalError : 1 {};
	/**
	 * Warning: Active
	offset 0 bit 15 */
	bool isWarnNow : 1 {};
	/**
	 * Error: Pedal
	offset 0 bit 16 */
	bool isPedalError : 1 {};
	/**
	 * Error: TPS2
	offset 0 bit 17 */
	bool isTps2Error : 1 {};
	/**
	 * Injector Fault
	offset 0 bit 18 */
	bool injectorFault : 1 {};
	/**
	 * Ignition Fault
	offset 0 bit 19 */
	bool ignitionFault : 1 {};
	/**
	 * isUsbConnected
	 * Original reason for this is to check if USB is connected from Lua
	offset 0 bit 20 */
	bool isUsbConnected : 1 {};
	/**
	offset 0 bit 21 */
	bool dfcoActive : 1 {};
	/**
	 * SD card writing
	offset 0 bit 22 */
	bool sd_active_wr : 1 {};
	/**
	 * SD card reading
	offset 0 bit 23 */
	bool sd_active_rd : 1 {};
	/**
	 * MAP from sensor seems valid
	offset 0 bit 24 */
	bool isMapValid : 1 {};
	/**
	offset 0 bit 25 */
	bool triggerPageRefreshFlag : 1 {};
	/**
	offset 0 bit 26 */
	bool hasFaultReportFile : 1 {};
	/**
	 * Analog sensors supply failure
	offset 0 bit 27 */
	bool isAnalogFailure : 1 {};
	/**
	offset 0 bit 28 */
	bool isTuningNow : 1 {};
	/**
	 * SD: formating is in progress
	offset 0 bit 29 */
	bool sd_formating : 1 {};
	/**
	offset 0 bit 30 */
	bool isMapAveraging : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_31_31 : 1 {};
	/**
	 * @@GAUGE_NAME_RPM@@
	 * units: RPM
	 * offset 4
	 */
	uint16_t RPMValue = (uint16_t)0;
	/**
	 * dRPM
	 * units: RPM acceleration/Rate of Change/ROC
	 * offset 6
	 */
	int16_t rpmAcceleration = (int16_t)0;
	/**
	 * @@GAUGE_NAME_GEAR_RATIO@@
	 * units: value
	 * offset 8
	 */
	scaled_channel<uint16_t, 100, 1> speedToRpmRatio = (uint16_t)0;
	/**
	 * units: V
	 * offset 10
	 */
	scaled_channel<int16_t, 1000, 1> internalVref = (int16_t)0;
	/**
	 * units: V
	 * offset 12
	 */
	scaled_channel<int16_t, 1000, 1> internalVbat = (int16_t)0;
	/**
	 * @@GAUGE_NAME_CLT@@
	 * units: deg C
	 * offset 14
	 */
	scaled_channel<int16_t, 100, 1> coolant = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IAT@@
	 * units: deg C
	 * offset 16
	 */
	scaled_channel<int16_t, 100, 1> intake = (int16_t)0;
	/**
	 * units: deg C
	 * offset 18
	 */
	scaled_channel<int16_t, 100, 1> auxTemp1 = (int16_t)0;
	/**
	 * units: deg C
	 * offset 20
	 */
	scaled_channel<int16_t, 100, 1> auxTemp2 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TPS@@
	 * units: %
	 * offset 22
	 */
	scaled_channel<int16_t, 100, 1> TPSValue = (int16_t)0;
	/**
	 * @@GAUGE_NAME_THROTTLE_PEDAL@@
	 * units: %
	 * offset 24
	 */
	scaled_channel<int16_t, 100, 1> throttlePedalPosition = (int16_t)0;
	/**
	 * units: ADC
	 * offset 26
	 */
	uint16_t tpsADC = (uint16_t)0;
	/**
	 * units: V
	 * offset 28
	 */
	scaled_channel<uint16_t, 1000, 1> rawMaf = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_MEASURED@@
	 * units: kg/h
	 * offset 30
	 */
	scaled_channel<uint16_t, 10, 1> mafMeasured = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_MAP@@
	 * units: kPa
	 * offset 32
	 */
	scaled_channel<uint16_t, 30, 1> MAPValue = (uint16_t)0;
	/**
	 * units: kPa
	 * offset 34
	 */
	scaled_channel<uint16_t, 30, 1> baroPressure = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA@@
	 * offset 36
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VBAT@@
	 * units: V
	 * offset 38
	 */
	scaled_channel<uint16_t, 1000, 1> VBatt = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VIGN@@
	 * units: V
	 * offset 40
	 */
	scaled_channel<uint16_t, 1000, 1> VIgn = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_OIL_PRESSURE@@
	 * units: kPa
	 * offset 42
	 */
	scaled_channel<uint16_t, 30, 1> oilPressure = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1I@@
	 * units: deg
	 * offset 44
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION@@
	 * Actual last injection time - including all compensation and injection mode
	 * units: ms
	 * offset 46
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjection = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION_RATIO@@
	 * Last injection time divided to previous injection time
	 * offset 48
	 */
	float actualLastInjectionRatio = (float)0;
	/**
	 * offset 52
	 */
	uint8_t stopEngineCode = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_INJ_DUTY@@
	 * units: %
	 * offset 53
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycle = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_CPU_TEMP@@
	 * units: deg C
	 * offset 54
	 */
	int8_t internalMcuTemperature = (int8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 55
	 */
	uint8_t alignmentFill_at_55[1] = {};
	/**
	 * @@GAUGE_NAME_FUEL_INJECTION_TIMING@@
	 * units: deg
	 * offset 56
	 */
	int16_t injectionOffset = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ENGINE_CRC16@@
	 * units: crc16
	 * offset 58
	 */
	uint16_t engineMakeCodeNameCrc16 = (uint16_t)0;
	/**
	 * Flex: AE multiplier
	 * units: mult
	 * offset 60
	 */
	scaled_channel<uint16_t, 1000, 1> flexAeMultiplier = (uint16_t)0;
	/**
	 * Flex: WW tau multiplier
	 * units: mult
	 * offset 62
	 */
	scaled_channel<uint16_t, 1000, 1> flexWwTauMultiplier = (uint16_t)0;
	/**
	 * Flex: WW beta multiplier
	 * units: mult
	 * offset 64
	 */
	scaled_channel<uint16_t, 1000, 1> flexWwBetaMultiplier = (uint16_t)0;
	/**
	 * offset 66
	 */
	uint16_t revolutionCounterSinceStart = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_READ_OK@@
	 * offset 68
	 */
	uint16_t canReadCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_TPS_EXTRA@@
	 * units: ms/frac
	 * offset 70
	 */
	scaled_channel<int16_t, 300, 1> tpsAccelFuel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IGNITION_MODE@@
	 * offset 72
	 */
	uint8_t currentIgnitionMode = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_INJECTION_MODE@@
	 * offset 73
	 */
	uint8_t currentInjectionMode = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_DWELL_DUTY@@
	 * units: %
	 * offset 74
	 */
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ETB_DUTY@@
	 * units: %
	 * offset 76
	 */
	scaled_channel<int16_t, 100, 1> etb1DutyCycle = (int16_t)0;
	/**
	 * Fuel level
	 * units: %
	 * offset 78
	 */
	scaled_channel<int16_t, 100, 1> fuelTankLevel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_CONSUMPTION@@
	 * units: grams
	 * offset 80
	 */
	uint16_t totalFuelConsumption = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_FLOW@@
	 * units: gram/s
	 * offset 82
	 */
	scaled_channel<uint16_t, 200, 1> fuelFlowRate = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TPS2@@
	 * units: %
	 * offset 84
	 */
	scaled_channel<int16_t, 100, 1> TPS2Value = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TUNE_CRC16@@
	 * units: crc16
	 * offset 86
	 */
	uint16_t tuneCrc16 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_UPTIME@@
	 * units: sec
	 * offset 88
	 */
	uint32_t seconds = (uint32_t)0;
	/**
	 * Engine Mode
	 * units: em
	 * offset 92
	 */
	uint32_t engineMode = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_VERSION@@
	 * units: version_f
	 * offset 96
	 */
	uint32_t firmwareVersion = (uint32_t)0;
	/**
	 * units: V
	 * offset 100
	 */
	scaled_channel<int16_t, 1000, 1> rawIdlePositionSensor = (int16_t)0;
	/**
	 * units: V
	 * offset 102
	 */
	scaled_channel<int16_t, 1000, 1> rawWastegatePosition = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_LAT@@
	 * units: G
	 * offset 104
	 */
	scaled_channel<int16_t, 1000, 1> accelerationLat = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_LON@@
	 * units: G
	 * offset 106
	 */
	scaled_channel<int16_t, 1000, 1> accelerationLon = (int16_t)0;
	/**
	 * @@GAUGE_NAME_DETECTED_GEAR@@
	 * offset 108
	 */
	int8_t detectedGear = (int8_t)0;
	/**
	 * offset 109
	 */
	uint8_t maxTriggerReentrant = (uint8_t)0;
	/**
	 * units: V
	 * offset 110
	 */
	scaled_channel<int16_t, 1000, 1> rawLowFuelPressure = (int16_t)0;
	/**
	 * units: V
	 * offset 112
	 */
	scaled_channel<int16_t, 1000, 1> rawHighFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_LOW@@
	 * units: kpa
	 * offset 114
	 */
	scaled_channel<int16_t, 30, 1> lowFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_DESIRED_GEAR@@
	 * units: gear
	 * offset 116
	 */
	int8_t tcuDesiredGear = (int8_t)0;
	/**
	 * @@GAUGE_NAME_FLEX@@
	 * units: %
	 * offset 117
	 */
	scaled_channel<uint8_t, 2, 1> flexPercent = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_WG_POSITION@@
	 * units: %
	 * offset 118
	 */
	scaled_channel<int16_t, 100, 1> wastegatePositionSensor = (int16_t)0;
	/**
	 * offset 120
	 */
	float calibrationValue = (float)0;
	/**
	 * @@GAUGE_NAME_TRG_ERR@@
	 * units: counter
	 * offset 124
	 */
	uint32_t totalTriggerErrorCounter = (uint32_t)0;
	/**
	 * offset 128
	 */
	uint32_t orderingErrorCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_COUNTER@@
	 * units: count
	 * offset 132
	 */
	uint16_t warningCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_LAST@@
	 * units: error
	 * offset 134
	 */
	uint16_t lastErrorCode = (uint16_t)0;
	/**
	 * Warning code
	 * units: error
	 * offset 136
	 */
	uint16_t recentErrorCode[8] = {};
	/**
	 * units: val
	 * offset 152
	 */
	float debugFloatField1 = (float)0;
	/**
	 * units: val
	 * offset 156
	 */
	float debugFloatField2 = (float)0;
	/**
	 * units: val
	 * offset 160
	 */
	float debugFloatField3 = (float)0;
	/**
	 * units: val
	 * offset 164
	 */
	float debugFloatField4 = (float)0;
	/**
	 * units: val
	 * offset 168
	 */
	float debugFloatField5 = (float)0;
	/**
	 * units: val
	 * offset 172
	 */
	float debugFloatField6 = (float)0;
	/**
	 * units: val
	 * offset 176
	 */
	float debugFloatField7 = (float)0;
	/**
	 * units: val
	 * offset 180
	 */
	uint32_t debugIntField1 = (uint32_t)0;
	/**
	 * units: val
	 * offset 184
	 */
	uint32_t debugIntField2 = (uint32_t)0;
	/**
	 * units: val
	 * offset 188
	 */
	uint32_t debugIntField3 = (uint32_t)0;
	/**
	 * units: val
	 * offset 192
	 */
	uint32_t debugIntField4 = (uint32_t)0;
	/**
	 * units: val
	 * offset 196
	 */
	uint32_t debugIntField5 = (uint32_t)0;
	/**
	 * EGT
	 * units: deg C
	 * offset 200
	 */
	scaled_channel<int16_t, 4, 1> egt[EGT_CHANNEL_COUNT] = {};
	/**
	 * units: V
	 * offset 216
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Primary = (int16_t)0;
	/**
	 * units: V
	 * offset 218
	 */
	scaled_channel<int16_t, 1000, 1> rawClt = (int16_t)0;
	/**
	 * units: V
	 * offset 220
	 */
	scaled_channel<int16_t, 1000, 1> rawIat = (int16_t)0;
	/**
	 * units: V
	 * offset 222
	 */
	scaled_channel<int16_t, 1000, 1> rawOilPressure = (int16_t)0;
	/**
	 * units: V
	 * offset 224
	 */
	scaled_channel<int16_t, 1000, 1> rawAcPressure = (int16_t)0;
	/**
	 * units: V
	 * offset 226
	 */
	scaled_channel<int16_t, 1000, 1> rawFuelLevel = (int16_t)0;
	/**
	 * units: V
	 * offset 228
	 */
	float rawPpsPrimary = (float)0;
	/**
	 * units: V
	 * offset 232
	 */
	float rawPpsSecondary = (float)0;
	/**
	 * units: V
	 * offset 236
	 */
	float rawRawPpsPrimary = (float)0;
	/**
	 * units: V
	 * offset 240
	 */
	float rawRawPpsSecondary = (float)0;
	/**
	 * @@GAUGE_NAME_IDLE_POSITION@@
	 * units: %
	 * offset 244
	 */
	scaled_channel<int16_t, 100, 1> idlePositionSensor = (int16_t)0;
	/**
	 * @@GAUGE_NAME_AFR@@
	 * units: AFR
	 * offset 246
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR2@@
	 * units: AFR
	 * offset 248
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue2 = (uint16_t)0;
	/**
	 * @@SMOOTHED_GAUGE_NAME_AFR@@
	 * units: AFR
	 * offset 250
	 */
	scaled_channel<uint16_t, 1000, 1> SmoothedAFRValue = (uint16_t)0;
	/**
	 * @@SMOOTHED_GAUGE_NAME_AFR2@@
	 * units: AFR
	 * offset 252
	 */
	scaled_channel<uint16_t, 1000, 1> SmoothedAFRValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA2@@
	 * offset 254
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1E@@
	 * units: deg
	 * offset 256
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1E = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2I@@
	 * units: deg
	 * offset 258
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2E@@
	 * units: deg
	 * offset 260
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2E = (int16_t)0;
	/**
	 * units: V
	 * offset 262
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Secondary = (int16_t)0;
	/**
	 * units: V
	 * offset 264
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Primary = (int16_t)0;
	/**
	 * units: V
	 * offset 266
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Secondary = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_VERT@@
	 * units: G
	 * offset 268
	 */
	scaled_channel<int16_t, 1000, 1> accelerationVert = (int16_t)0;
	/**
	 * @@GAUGE_NAME_GYRO_YAW@@
	 * units: deg/sec
	 * offset 270
	 */
	scaled_channel<int16_t, 1000, 1> gyroYaw = (int16_t)0;
	/**
	 * units: deg
	 * offset 272
	 */
	int8_t vvtTargets[4] = {};
	/**
	 * @@GAUGE_NAME_TURBO_SPEED@@
	 * units: hz
	 * offset 276
	 */
	uint16_t turboSpeed = (uint16_t)0;
	/**
	 * Ign: Timing Cyl
	 * units: deg
	 * offset 278
	 */
	scaled_channel<int16_t, 50, 1> ignitionAdvanceCyl[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: %
	 * offset 302
	 */
	scaled_channel<int16_t, 100, 1> tps1Split = (int16_t)0;
	/**
	 * units: %
	 * offset 304
	 */
	scaled_channel<int16_t, 100, 1> tps2Split = (int16_t)0;
	/**
	 * units: %
	 * offset 306
	 */
	scaled_channel<int16_t, 100, 1> tps12Split = (int16_t)0;
	/**
	 * units: %
	 * offset 308
	 */
	scaled_channel<int16_t, 100, 1> accPedalSplit = (int16_t)0;
	/**
	 * units: %
	 * offset 310
	 */
	scaled_channel<int16_t, 100, 1> accPedalUnfiltered = (int16_t)0;
	/**
	 * Ign: Cut Code
	 * units: code
	 * offset 312
	 */
	int8_t sparkCutReason = (int8_t)0;
	/**
	 * Fuel: Cut Code
	 * units: code
	 * offset 313
	 */
	int8_t fuelCutReason = (int8_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_ESTIMATE@@
	 * units: kg/h
	 * offset 314
	 */
	scaled_channel<uint16_t, 10, 1> mafEstimate = (uint16_t)0;
	/**
	 * sync: instant RPM
	 * units: rpm
	 * offset 316
	 */
	uint16_t instantRpm = (uint16_t)0;
	/**
	 * units: V
	 * offset 318
	 */
	scaled_channel<uint16_t, 1000, 1> rawMap = (uint16_t)0;
	/**
	 * units: V
	 * offset 320
	 */
	scaled_channel<uint16_t, 1000, 1> rawMapFast = (uint16_t)0;
	/**
	 * units: V
	 * offset 322
	 */
	scaled_channel<uint16_t, 1000, 1> rawAfr = (uint16_t)0;
	/**
	 * offset 324
	 */
	int8_t sparkCutReasonBlinker = (int8_t)0;
	/**
	 * offset 325
	 */
	int8_t fuelCutReasonBlinker = (int8_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_HIGH@@
	 * units: bar
	 * offset 326
	 */
	scaled_channel<int16_t, 10, 1> highFuelPressure = (int16_t)0;
	/**
	 * offset 328
	 */
	float calibrationValue2 = (float)0;
	/**
	 * Lua: Tick counter
	 * units: count
	 * offset 332
	 */
	uint32_t luaInvocationCounter = (uint32_t)0;
	/**
	 * Lua: Last tick duration
	 * units: nt
	 * offset 336
	 */
	uint32_t luaLastCycleDuration = (uint32_t)0;
	/**
	 * offset 340
	 */
	uint32_t vssEdgeCounter = (uint32_t)0;
	/**
	 * offset 344
	 */
	uint32_t issEdgeCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_1@@
	 * offset 348
	 */
	float auxLinear1 = (float)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_2@@
	 * offset 352
	 */
	float auxLinear2 = (float)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_3@@
	 * offset 356
	 */
	float auxLinear3 = (float)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_4@@
	 * offset 360
	 */
	float auxLinear4 = (float)0;
	/**
	 * units: kPa
	 * offset 364
	 */
	scaled_channel<uint16_t, 10, 1> fallbackMap = (uint16_t)0;
	/**
	 * Effective MAP
	 * units: kPa
	 * offset 366
	 */
	scaled_channel<uint16_t, 10, 1> effectiveMap = (uint16_t)0;
	/**
	 * AE: Map Pred New Cycles
	 * offset 368
	 */
	uint16_t predTimerResetCnt = (uint16_t)0;
	/**
	 * AE: Map Pred Expired
	 * offset 370
	 */
	uint16_t mapPredEventOver = (uint16_t)0;
	/**
	 * Instant MAP
	 * units: kPa
	 * offset 372
	 */
	scaled_channel<uint16_t, 30, 1> instantMAPValue = (uint16_t)0;
	/**
	 * units: us
	 * offset 374
	 */
	uint16_t maxLockedDuration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_WRITE_OK@@
	 * offset 376
	 */
	uint16_t canWriteOk = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_WRITE_ERR@@
	 * offset 378
	 */
	uint16_t canWriteNotOk = (uint16_t)0;
	/**
	 * offset 380
	 */
	uint32_t triggerPrimaryFall = (uint32_t)0;
	/**
	 * offset 384
	 */
	uint32_t triggerPrimaryRise = (uint32_t)0;
	/**
	 * offset 388
	 */
	uint32_t triggerSecondaryFall = (uint32_t)0;
	/**
	 * offset 392
	 */
	uint32_t triggerSecondaryRise = (uint32_t)0;
	/**
	 * offset 396
	 */
	uint8_t starterState = (uint8_t)0;
	/**
	 * offset 397
	 */
	uint8_t starterRelayDisable = (uint8_t)0;
	/**
	 * Ign: Multispark count
	 * offset 398
	 */
	uint8_t multiSparkCounter = (uint8_t)0;
	/**
	 * offset 399
	 */
	uint8_t extiOverflowCount = (uint8_t)0;
	/**
	 * offset 400
	 */
	pid_status_s alternatorStatus;
	/**
	 * offset 416
	 */
	pid_status_s idleStatus;
	/**
	 * offset 432
	 */
	pid_status_s etbStatus;
	/**
	 * offset 448
	 */
	pid_status_s boostStatus;
	/**
	 * offset 464
	 */
	pid_status_s wastegateDcStatus;
	/**
	 * offset 480
	 */
	pid_status_s vvtStatus[CAM_INPUTS_COUNT] = {};
	/**
	 * Aux speed 1
	 * units: s
	 * offset 544
	 */
	uint16_t auxSpeed1 = (uint16_t)0;
	/**
	 * Aux speed 2
	 * units: s
	 * offset 546
	 */
	uint16_t auxSpeed2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ISS@@
	 * units: RPM
	 * offset 548
	 */
	uint16_t ISSValue = (uint16_t)0;
	/**
	 * units: V
	 * offset 550
	 */
	scaled_channel<int16_t, 1000, 1> rawAnalogInput[LUA_ANALOG_INPUT_COUNT] = {};
	/**
	 * GPPWM Output
	 * units: %
	 * offset 566
	 */
	scaled_channel<uint8_t, 2, 1> gppwmOutput[4] = {};
	/**
	 * offset 570
	 */
	scaled_channel<int16_t, 1, 1> gppwmXAxis[4] = {};
	/**
	 * offset 578
	 */
	scaled_channel<int16_t, 10, 1> gppwmYAxis[4] = {};
	/**
	 * units: V
	 * offset 586
	 */
	scaled_channel<int16_t, 1000, 1> rawBattery = (int16_t)0;
	/**
	 * offset 588
	 */
	scaled_channel<int16_t, 10, 1> ignBlendParameter[IGN_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 596
	 */
	scaled_channel<uint8_t, 2, 1> ignBlendBias[IGN_BLEND_COUNT] = {};
	/**
	 * units: deg
	 * offset 600
	 */
	scaled_channel<int16_t, 100, 1> ignBlendOutput[IGN_BLEND_COUNT] = {};
	/**
	 * offset 608
	 */
	scaled_channel<int16_t, 10, 1> ignBlendYAxis[IGN_BLEND_COUNT] = {};
	/**
	 * offset 616
	 */
	scaled_channel<int16_t, 10, 1> veBlendParameter[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 624
	 */
	scaled_channel<uint8_t, 2, 1> veBlendBias[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 628
	 */
	scaled_channel<int16_t, 100, 1> veBlendOutput[VE_BLEND_COUNT] = {};
	/**
	 * offset 636
	 */
	scaled_channel<int16_t, 10, 1> veBlendYAxis[VE_BLEND_COUNT] = {};
	/**
	 * offset 644
	 */
	scaled_channel<int16_t, 10, 1> secondVeBlendParameter = (int16_t)0;
	/**
	 * offset 646
	 */
	scaled_channel<int16_t, 10, 1> secondIgnitionBlendParameter = (int16_t)0;
	/**
	 * units: %
	 * offset 648
	 */
	scaled_channel<uint8_t, 2, 1> secondVeBlendBias = (uint8_t)0;
	/**
	 * units: %
	 * offset 649
	 */
	scaled_channel<uint8_t, 2, 1> secondIgnitionBlendBias = (uint8_t)0;
	/**
	 * offset 650
	 */
	scaled_channel<int16_t, 10, 1> boostOpenLoopBlendParameter[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 654
	 */
	scaled_channel<uint8_t, 2, 1> boostOpenLoopBlendBias[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 656
	 */
	int8_t boostOpenLoopBlendOutput[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 658
	 */
	scaled_channel<int16_t, 10, 1> boostOpenLoopBlendYAxis[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 662
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendParameter[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 666
	 */
	scaled_channel<uint8_t, 2, 1> boostClosedLoopBlendBias[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 668
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendOutput[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 672
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendYAxis[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 676
	 */
	scaled_channel<int16_t, 10, 1> targetAfrBlendParameter[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 680
	 */
	scaled_channel<uint8_t, 2, 1> targetAfrBlendBias[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 682
	 */
	scaled_channel<int16_t, 10, 1> targetAfrBlendOutput[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * offset 686
	 */
	scaled_channel<int16_t, 10, 1> targetAfrBlendYAxis[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION_STAGE_2@@
	 * units: ms
	 * offset 690
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjectionStage2 = (uint16_t)0;
	/**
	offset 692 bit 0 */
	bool coilState1 : 1 {};
	/**
	offset 692 bit 1 */
	bool coilState2 : 1 {};
	/**
	offset 692 bit 2 */
	bool coilState3 : 1 {};
	/**
	offset 692 bit 3 */
	bool coilState4 : 1 {};
	/**
	offset 692 bit 4 */
	bool coilState5 : 1 {};
	/**
	offset 692 bit 5 */
	bool coilState6 : 1 {};
	/**
	offset 692 bit 6 */
	bool coilState7 : 1 {};
	/**
	offset 692 bit 7 */
	bool coilState8 : 1 {};
	/**
	offset 692 bit 8 */
	bool coilState9 : 1 {};
	/**
	offset 692 bit 9 */
	bool coilState10 : 1 {};
	/**
	offset 692 bit 10 */
	bool coilState11 : 1 {};
	/**
	offset 692 bit 11 */
	bool coilState12 : 1 {};
	/**
	offset 692 bit 12 */
	bool injectorState1 : 1 {};
	/**
	offset 692 bit 13 */
	bool injectorState2 : 1 {};
	/**
	offset 692 bit 14 */
	bool injectorState3 : 1 {};
	/**
	offset 692 bit 15 */
	bool injectorState4 : 1 {};
	/**
	offset 692 bit 16 */
	bool injectorState5 : 1 {};
	/**
	offset 692 bit 17 */
	bool injectorState6 : 1 {};
	/**
	offset 692 bit 18 */
	bool injectorState7 : 1 {};
	/**
	offset 692 bit 19 */
	bool injectorState8 : 1 {};
	/**
	offset 692 bit 20 */
	bool injectorState9 : 1 {};
	/**
	offset 692 bit 21 */
	bool injectorState10 : 1 {};
	/**
	offset 692 bit 22 */
	bool injectorState11 : 1 {};
	/**
	offset 692 bit 23 */
	bool injectorState12 : 1 {};
	/**
	offset 692 bit 24 */
	bool triggerChannel1 : 1 {};
	/**
	offset 692 bit 25 */
	bool triggerChannel2 : 1 {};
	/**
	 * bank 1 intake cam input
	offset 692 bit 26 */
	bool vvtChannel1 : 1 {};
	/**
	 * bank 1 exhaust cam input
	offset 692 bit 27 */
	bool vvtChannel2 : 1 {};
	/**
	 * bank 2 intake cam input
	offset 692 bit 28 */
	bool vvtChannel3 : 1 {};
	/**
	 * bank 2 exhaust cam input
	offset 692 bit 29 */
	bool vvtChannel4 : 1 {};
	/**
	 * AE: Map Prediction Active
	offset 692 bit 30 */
	bool isMapPredictionActive : 1 {};
	/**
	 * Error: Flex
	offset 692 bit 31 */
	bool isFlexError : 1 {};
	/**
	 * offset 696
	 */
	uint32_t outputRequestPeriod = (uint32_t)0;
	/**
	 * offset 700
	 */
	float mapFast = (float)0;
	/**
	 * Lua: Gauge
	 * units: value
	 * offset 704
	 */
	float luaGauges[LUA_GAUGE_COUNT] = {};
	/**
	 * units: V
	 * offset 736
	 */
	scaled_channel<uint16_t, 1000, 1> rawMaf2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_MEASURED_2@@
	 * units: kg/h
	 * offset 738
	 */
	scaled_channel<uint16_t, 10, 1> mafMeasured2 = (uint16_t)0;
	/**
	 * offset 740
	 */
	uint16_t schedulingUsedCount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVS@@
	 * units: kph
	 * offset 742
	 */
	scaled_channel<uint16_t, 100, 1> vehicleSpeedKph = (uint16_t)0;
	/**
	 * units: %
	 * offset 744
	 */
	scaled_channel<uint16_t, 100, 1> Gego = (uint16_t)0;
	/**
	 * units: count
	 * offset 746
	 */
	uint16_t testBenchIter = (uint16_t)0;
	/**
	 * units: deg C
	 * offset 748
	 */
	scaled_channel<int16_t, 100, 1> oilTemp = (int16_t)0;
	/**
	 * units: deg C
	 * offset 750
	 */
	scaled_channel<int16_t, 100, 1> fuelTemp = (int16_t)0;
	/**
	 * units: deg C
	 * offset 752
	 */
	scaled_channel<int16_t, 100, 1> ambientTemp = (int16_t)0;
	/**
	 * units: deg C
	 * offset 754
	 */
	scaled_channel<int16_t, 100, 1> compressorDischargeTemp = (int16_t)0;
	/**
	 * units: kPa
	 * offset 756
	 */
	scaled_channel<uint16_t, 30, 1> compressorDischargePressure = (uint16_t)0;
	/**
	 * units: kPa
	 * offset 758
	 */
	scaled_channel<uint16_t, 30, 1> throttleInletPressure = (uint16_t)0;
	/**
	 * units: sec
	 * offset 760
	 */
	uint16_t ignitionOnTime = (uint16_t)0;
	/**
	 * units: sec
	 * offset 762
	 */
	uint16_t engineRunTime = (uint16_t)0;
	/**
	 * units: km
	 * offset 764
	 */
	scaled_channel<uint16_t, 10, 1> distanceTraveled = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR_GAS_SCALE@@
	 * units: AFR
	 * offset 766
	 */
	scaled_channel<uint16_t, 1000, 1> afrGasolineScale = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR2_GAS_SCALE@@
	 * units: AFR
	 * offset 768
	 */
	scaled_channel<uint16_t, 1000, 1> afr2GasolineScale = (uint16_t)0;
	/**
	 * offset 770
	 */
	uint8_t calibrationMode = (uint8_t)0;
	/**
	 * Idle: Stepper target position
	 * offset 771
	 */
	uint8_t idleStepperTargetPosition = (uint8_t)0;
	/**
	 * offset 772
	 */
	float wheelSlipRatio = (float)0;
	/**
	 * offset 776
	 */
	uint8_t ignitorDiagnostic[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 788
	 */
	uint8_t injectorDiagnostic[MAX_CYLINDER_COUNT] = {};
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION_RATIO_STAGE_2@@
	 * offset 800
	 */
	float actualLastInjectionRatioStage2 = (float)0;
	/**
	 * @@GAUGE_NAME_FUEL_INJ_DUTY_STAGE_2@@
	 * units: %
	 * offset 804
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycleStage2 = (uint8_t)0;
	/**
	 * offset 805
	 */
	uint8_t rawFlexFreq = (uint8_t)0;
	/**
	 * offset 806
	 */
	uint8_t canReWidebandCmdStatus = (uint8_t)0;
	/**
	 * offset 807
	 */
	uint8_t deviceUid = (uint8_t)0;
	/**
	 * offset 808
	 */
	uint16_t mc33810spiErrorCounter = (uint16_t)0;
	/**
	 * offset 810
	 */
	uint8_t injectionPrimingCounter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 811
	 */
	uint8_t alignmentFill_at_811[1] = {};
	/**
	 * @@GAUGE_NAME_AC_PRESSURE@@
	 * units: kPa
	 * offset 812
	 */
	float acPressure = (float)0;
	/**
	 * units: V
	 * offset 816
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog1 = (int16_t)0;
	/**
	 * units: V
	 * offset 818
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog2 = (int16_t)0;
	/**
	 * units: V
	 * offset 820
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog3 = (int16_t)0;
	/**
	 * units: V
	 * offset 822
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog4 = (int16_t)0;
	/**
	 * ECU: Fast ADC errors
	 * offset 824
	 */
	uint8_t fastAdcErrorCount = (uint8_t)0;
	/**
	 * ECU: Slow ADC errors
	 * offset 825
	 */
	uint8_t slowAdcErrorCount = (uint8_t)0;
	/**
	 * units: V
	 * offset 826
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxTemp1 = (int16_t)0;
	/**
	 * units: V
	 * offset 828
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxTemp2 = (int16_t)0;
	/**
	 * units: V
	 * offset 830
	 */
	scaled_channel<int16_t, 1000, 1> rawAmbientTemp = (int16_t)0;
	/**
	 * offset 832
	 */
	uint32_t rtcUnixEpochTime = (uint32_t)0;
	/**
	 * offset 836
	 */
	int16_t hp = (int16_t)0;
	/**
	 * offset 838
	 */
	int16_t torque = (int16_t)0;
	/**
	 * offset 840
	 */
	float throttlePressureRatio = (float)0;
	/**
	 * offset 844
	 */
	float throttleEffectiveAreaOpening = (float)0;
	/**
	 * offset 848
	 */
	uint32_t mcuSerial = (uint32_t)0;
	/**
	 * offset 852
	 */
	uint8_t sd_error = (uint8_t)0;
	/**
	 * SD: Logging state
	 * units: code
	 * offset 853
	 */
	uint8_t sdLoggingState = (uint8_t)0;
	/**
	 * ECU: Fast ADC overruns
	 * offset 854
	 */
	uint8_t fastAdcOverrunCount = (uint8_t)0;
	/**
	 * ECU: Slow ADC overruns
	 * offset 855
	 */
	uint8_t slowAdcOverrunCount = (uint8_t)0;
	/**
	 * ECU: Fast ADC error type
	 * offset 856
	 */
	uint8_t fastAdcLastError = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 857
	 */
	uint8_t alignmentFill_at_857[1] = {};
	/**
	 * ECU: Fast ADC period
	 * units: ticks
	 * offset 858
	 */
	uint16_t fastAdcPeriod = (uint16_t)0;
	/**
	 * ECU: Fast ADC conversions
	 * units: N
	 * offset 860
	 */
	uint16_t fastAdcConversionCount = (uint16_t)0;
	/**
	 * offset 862
	 */
	uint8_t canReWidebandVersion = (uint8_t)0;
	/**
	 * offset 863
	 */
	uint8_t canReWidebandFwDay = (uint8_t)0;
	/**
	 * offset 864
	 */
	uint8_t canReWidebandFwMon = (uint8_t)0;
	/**
	 * offset 865
	 */
	uint8_t canReWidebandFwYear = (uint8_t)0;
	/**
	 * offset 866
	 */
	uint16_t transitionEventCode = (uint16_t)0;
	/**
	 * offset 868
	 */
	uint16_t transitionEventsCounter = (uint16_t)0;
	/**
	 * units: kPa
	 * offset 870
	 */
	uint8_t mapPerCylinder[MAX_CYLINDER_COUNT] = {};
	/**
	 * ECU: Slow ADC complete scans
	 * units: N
	 * offset 882
	 */
	uint16_t slowAdcScanCount = (uint16_t)0;
	/**
	 * CLT: measured resistance
	 * units: Ohm
	 * offset 884
	 */
	float cltResistance = (float)0;
	/**
	 * IAT: measured resistance
	 * units: Ohm
	 * offset 888
	 */
	float iatResistance = (float)0;
	/**
	 * Aux temp 1: measured resistance
	 * units: Ohm
	 * offset 892
	 */
	float auxTemp1Resistance = (float)0;
	/**
	 * Aux temp 2: measured resistance
	 * units: Ohm
	 * offset 896
	 */
	float auxTemp2Resistance = (float)0;
	/**
	 * sync: instant RPM range
	 * Max minus min instant RPM within the last complete engine cycle, 0 when RPM is perfectly steady
	 * units: rpm
	 * offset 900
	 */
	uint16_t instantRpmRange = (uint16_t)0;
	/**
	 * Engine
	 * offset 902
	 */
	uint8_t engine = (uint8_t)0;
	/**
	 * SD: Mount mode
	 * units: code
	 * offset 903
	 */
	uint8_t sdCardMode = (uint8_t)0;
	/**
	 * Dash: Left turn
	offset 904 bit 0 */
	bool dashLeftTurn : 1 {};
	/**
	 * Dash: Right turn
	offset 904 bit 1 */
	bool dashRightTurn : 1 {};
	/**
	 * Dash: Check engine
	offset 904 bit 2 */
	bool dashCheckEngine : 1 {};
	/**
	 * Dash: High beams
	offset 904 bit 3 */
	bool dashHighBeams : 1 {};
	/**
	 * Dash: Parking brake
	offset 904 bit 4 */
	bool dashParkingBrake : 1 {};
	/**
	 * Transmission slip valid
	offset 904 bit 5 */
	bool transmissionSlipValid : 1 {};
	/**
	offset 904 bit 6 */
	bool unusedBit_326_6 : 1 {};
	/**
	offset 904 bit 7 */
	bool unusedBit_326_7 : 1 {};
	/**
	offset 904 bit 8 */
	bool unusedBit_326_8 : 1 {};
	/**
	offset 904 bit 9 */
	bool unusedBit_326_9 : 1 {};
	/**
	offset 904 bit 10 */
	bool unusedBit_326_10 : 1 {};
	/**
	offset 904 bit 11 */
	bool unusedBit_326_11 : 1 {};
	/**
	offset 904 bit 12 */
	bool unusedBit_326_12 : 1 {};
	/**
	offset 904 bit 13 */
	bool unusedBit_326_13 : 1 {};
	/**
	offset 904 bit 14 */
	bool unusedBit_326_14 : 1 {};
	/**
	offset 904 bit 15 */
	bool unusedBit_326_15 : 1 {};
	/**
	offset 904 bit 16 */
	bool unusedBit_326_16 : 1 {};
	/**
	offset 904 bit 17 */
	bool unusedBit_326_17 : 1 {};
	/**
	offset 904 bit 18 */
	bool unusedBit_326_18 : 1 {};
	/**
	offset 904 bit 19 */
	bool unusedBit_326_19 : 1 {};
	/**
	offset 904 bit 20 */
	bool unusedBit_326_20 : 1 {};
	/**
	offset 904 bit 21 */
	bool unusedBit_326_21 : 1 {};
	/**
	offset 904 bit 22 */
	bool unusedBit_326_22 : 1 {};
	/**
	offset 904 bit 23 */
	bool unusedBit_326_23 : 1 {};
	/**
	offset 904 bit 24 */
	bool unusedBit_326_24 : 1 {};
	/**
	offset 904 bit 25 */
	bool unusedBit_326_25 : 1 {};
	/**
	offset 904 bit 26 */
	bool unusedBit_326_26 : 1 {};
	/**
	offset 904 bit 27 */
	bool unusedBit_326_27 : 1 {};
	/**
	offset 904 bit 28 */
	bool unusedBit_326_28 : 1 {};
	/**
	offset 904 bit 29 */
	bool unusedBit_326_29 : 1 {};
	/**
	offset 904 bit 30 */
	bool unusedBit_326_30 : 1 {};
	/**
	offset 904 bit 31 */
	bool unusedBit_326_31 : 1 {};
	/**
	 * Output Shaft Speed
	 * units: RPM
	 * offset 908
	 */
	float outputShaftSpeed = (float)0;
	/**
	 * @@GAUGE_NAME_TRANS_SLIP@@
	 * units: %
	 * offset 912
	 */
	float transmissionSlipPercent = (float)0;
};
static_assert(sizeof(output_channels_s) == 916);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) console/binary/output_channels.txt
