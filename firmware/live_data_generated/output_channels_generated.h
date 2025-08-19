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
	 * Launch Control Triggered
	offset 0 bit 17 */
	bool launchTriggered : 1 {};
	/**
	 * Error: TPS2
	offset 0 bit 18 */
	bool isTps2Error : 1 {};
	/**
	 * Injector Fault
	offset 0 bit 19 */
	bool injectorFault : 1 {};
	/**
	 * Ignition Fault
	offset 0 bit 20 */
	bool ignitionFault : 1 {};
	/**
	 * isUsbConnected
	 * Original reason for this is to check if USB is connected from Lua
	offset 0 bit 21 */
	bool isUsbConnected : 1 {};
	/**
	offset 0 bit 22 */
	bool dfcoActive : 1 {};
	/**
	 * SD card writing
	offset 0 bit 23 */
	bool sd_active_wr : 1 {};
	/**
	 * SD card reading
	offset 0 bit 24 */
	bool sd_active_rd : 1 {};
	/**
	 * MAP from sensor seems valid
	offset 0 bit 25 */
	bool isMapValid : 1 {};
	/**
	offset 0 bit 26 */
	bool triggerPageRefreshFlag : 1 {};
	/**
	offset 0 bit 27 */
	bool hasFaultReportFile : 1 {};
	/**
	 * Analog sensors supply failure
	offset 0 bit 28 */
	bool isAnalogFailure : 1 {};
	/**
	offset 0 bit 29 */
	bool isTuningNow : 1 {};
	/**
	 * SD: formating is in progress
	offset 0 bit 30 */
	bool sd_formating : 1 {};
	/**
	offset 0 bit 31 */
	bool isMapAveraging : 1 {};
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
	 * @@GAUGE_NAME_VVS@@
	 * units: kph
	 * offset 10
	 */
	uint8_t unusedVehicleSpeedKph = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_CPU_TEMP@@
	 * units: deg C
	 * offset 11
	 */
	int8_t internalMcuTemperature = (int8_t)0;
	/**
	 * @@GAUGE_NAME_CLT@@
	 * units: deg C
	 * offset 12
	 */
	scaled_channel<int16_t, 100, 1> coolant = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IAT@@
	 * units: deg C
	 * offset 14
	 */
	scaled_channel<int16_t, 100, 1> intake = (int16_t)0;
	/**
	 * units: deg C
	 * offset 16
	 */
	scaled_channel<int16_t, 100, 1> auxTemp1 = (int16_t)0;
	/**
	 * units: deg C
	 * offset 18
	 */
	scaled_channel<int16_t, 100, 1> auxTemp2 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TPS@@
	 * units: %
	 * offset 20
	 */
	scaled_channel<int16_t, 100, 1> TPSValue = (int16_t)0;
	/**
	 * @@GAUGE_NAME_THROTTLE_PEDAL@@
	 * units: %
	 * offset 22
	 */
	scaled_channel<int16_t, 100, 1> throttlePedalPosition = (int16_t)0;
	/**
	 * units: ADC
	 * offset 24
	 */
	uint16_t tpsADC = (uint16_t)0;
	/**
	 * units: V
	 * offset 26
	 */
	scaled_channel<uint16_t, 1000, 1> rawMaf = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_MEASURED@@
	 * units: kg/h
	 * offset 28
	 */
	scaled_channel<uint16_t, 10, 1> mafMeasured = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_MAP@@
	 * units: kPa
	 * offset 30
	 */
	scaled_channel<uint16_t, 30, 1> MAPValue = (uint16_t)0;
	/**
	 * units: kPa
	 * offset 32
	 */
	scaled_channel<uint16_t, 30, 1> baroPressure = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA@@
	 * offset 34
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VBAT@@
	 * units: V
	 * offset 36
	 */
	scaled_channel<uint16_t, 1000, 1> VBatt = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_OIL_PRESSURE@@
	 * units: kPa
	 * offset 38
	 */
	scaled_channel<uint16_t, 30, 1> oilPressure = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1I@@
	 * units: deg
	 * offset 40
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION@@
	 * Actual last injection time - including all compensation and injection mode
	 * units: ms
	 * offset 42
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjection = (uint16_t)0;
	/**
	 * offset 44
	 */
	uint8_t stopEngineCode = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_INJ_DUTY@@
	 * units: %
	 * offset 45
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycle = (uint8_t)0;
	/**
	 * offset 46
	 */
	uint8_t tempLogging1 = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 47
	 */
	uint8_t alignmentFill_at_47[1] = {};
	/**
	 * @@GAUGE_NAME_FUEL_INJECTION_TIMING@@
	 * units: deg
	 * offset 48
	 */
	int16_t injectionOffset = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ENGINE_CRC16@@
	 * units: crc16
	 * offset 50
	 */
	uint16_t engineMakeCodeNameCrc16 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_AMOUNT@@
	 * units: mg
	 * offset 52
	 */
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_CORRECTION@@
	 * units: mg
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
	 * units: ms
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
	 * units: %
	 * offset 64
	 */
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ETB_DUTY@@
	 * units: %
	 * offset 66
	 */
	scaled_channel<int16_t, 100, 1> etb1DutyCycle = (int16_t)0;
	/**
	 * Fuel level
	 * units: %
	 * offset 68
	 */
	scaled_channel<int16_t, 100, 1> fuelTankLevel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_CONSUMPTION@@
	 * units: grams
	 * offset 70
	 */
	uint16_t totalFuelConsumption = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_FLOW@@
	 * units: gram/s
	 * offset 72
	 */
	scaled_channel<uint16_t, 200, 1> fuelFlowRate = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TPS2@@
	 * units: %
	 * offset 74
	 */
	scaled_channel<int16_t, 100, 1> TPS2Value = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TUNE_CRC16@@
	 * units: crc16
	 * offset 76
	 */
	uint16_t tuneCrc16 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_VE@@
	 * units: ratio
	 * offset 78
	 */
	scaled_channel<uint16_t, 10, 1> veValue = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_UPTIME@@
	 * units: sec
	 * offset 80
	 */
	uint32_t seconds = (uint32_t)0;
	/**
	 * Engine Mode
	 * units: em
	 * offset 84
	 */
	uint32_t engineMode = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_VERSION@@
	 * units: version_f
	 * offset 88
	 */
	uint32_t firmwareVersion = (uint32_t)0;
	/**
	 * units: V
	 * offset 92
	 */
	scaled_channel<int16_t, 1000, 1> rawIdlePositionSensor = (int16_t)0;
	/**
	 * units: V
	 * offset 94
	 */
	scaled_channel<int16_t, 1000, 1> rawWastegatePosition = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_LAT@@
	 * units: G
	 * offset 96
	 */
	scaled_channel<int16_t, 1000, 1> accelerationLat = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_LON@@
	 * units: G
	 * offset 98
	 */
	scaled_channel<int16_t, 1000, 1> accelerationLon = (int16_t)0;
	/**
	 * @@GAUGE_NAME_DETECTED_GEAR@@
	 * offset 100
	 */
	uint8_t detectedGear = (uint8_t)0;
	/**
	 * offset 101
	 */
	uint8_t maxTriggerReentrant = (uint8_t)0;
	/**
	 * units: V
	 * offset 102
	 */
	scaled_channel<int16_t, 1000, 1> rawLowFuelPressure = (int16_t)0;
	/**
	 * units: V
	 * offset 104
	 */
	scaled_channel<int16_t, 1000, 1> rawHighFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_LOW@@
	 * units: kpa
	 * offset 106
	 */
	scaled_channel<int16_t, 30, 1> lowFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_DESIRED_GEAR@@
	 * units: gear
	 * offset 108
	 */
	int8_t tcuDesiredGear = (int8_t)0;
	/**
	 * @@GAUGE_NAME_FLEX@@
	 * units: %
	 * offset 109
	 */
	scaled_channel<uint8_t, 2, 1> flexPercent = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_WG_POSITION@@
	 * units: %
	 * offset 110
	 */
	scaled_channel<int16_t, 100, 1> wastegatePositionSensor = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_HIGH@@
	 * units: bar
	 * offset 112
	 */
	scaled_channel<int16_t, 10, 1> highFuelPressure = (int16_t)0;
	/**
	 * offset 114
	 */
	uint8_t tempLogging3 = (uint8_t)0;
	/**
	 * offset 115
	 */
	uint8_t tempLogging4 = (uint8_t)0;
	/**
	 * offset 116
	 */
	float calibrationValue = (float)0;
	/**
	 * offset 120
	 */
	uint8_t calibrationMode = (uint8_t)0;
	/**
	 * Idle: Stepper target position
	 * offset 121
	 */
	uint8_t idleStepperTargetPosition = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 122
	 */
	uint8_t alignmentFill_at_122[2] = {};
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
	 * Vss Accel
	 * units: m/s2
	 * offset 254
	 */
	scaled_channel<uint16_t, 300, 1> VssAcceleration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA2@@
	 * offset 256
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1E@@
	 * units: deg
	 * offset 258
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1E = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2I@@
	 * units: deg
	 * offset 260
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2E@@
	 * units: deg
	 * offset 262
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2E = (int16_t)0;
	/**
	 * units: V
	 * offset 264
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Secondary = (int16_t)0;
	/**
	 * units: V
	 * offset 266
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Primary = (int16_t)0;
	/**
	 * units: V
	 * offset 268
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Secondary = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_VERT@@
	 * units: G
	 * offset 270
	 */
	scaled_channel<int16_t, 1000, 1> accelerationVert = (int16_t)0;
	/**
	 * @@GAUGE_NAME_GYRO_YAW@@
	 * units: deg/sec
	 * offset 272
	 */
	scaled_channel<int16_t, 1000, 1> gyroYaw = (int16_t)0;
	/**
	 * units: deg
	 * offset 274
	 */
	int8_t vvtTargets[4] = {};
	/**
	 * @@GAUGE_NAME_TURBO_SPEED@@
	 * units: hz
	 * offset 278
	 */
	uint16_t turboSpeed = (uint16_t)0;
	/**
	 * Ign: Timing Cyl
	 * units: deg
	 * offset 280
	 */
	scaled_channel<int16_t, 50, 1> ignitionAdvanceCyl[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: %
	 * offset 304
	 */
	scaled_channel<int16_t, 100, 1> tps1Split = (int16_t)0;
	/**
	 * units: %
	 * offset 306
	 */
	scaled_channel<int16_t, 100, 1> tps2Split = (int16_t)0;
	/**
	 * units: %
	 * offset 308
	 */
	scaled_channel<int16_t, 100, 1> tps12Split = (int16_t)0;
	/**
	 * units: %
	 * offset 310
	 */
	scaled_channel<int16_t, 100, 1> accPedalSplit = (int16_t)0;
	/**
	 * units: %
	 * offset 312
	 */
	scaled_channel<int16_t, 100, 1> accPedalUnfiltered = (int16_t)0;
	/**
	 * Ign: Cut Code
	 * units: code
	 * offset 314
	 */
	int8_t sparkCutReason = (int8_t)0;
	/**
	 * Fuel: Cut Code
	 * units: code
	 * offset 315
	 */
	int8_t fuelCutReason = (int8_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_ESTIMATE@@
	 * units: kg/h
	 * offset 316
	 */
	scaled_channel<uint16_t, 10, 1> mafEstimate = (uint16_t)0;
	/**
	 * sync: instant RPM
	 * units: rpm
	 * offset 318
	 */
	uint16_t instantRpm = (uint16_t)0;
	/**
	 * units: V
	 * offset 320
	 */
	scaled_channel<uint16_t, 1000, 1> rawMap = (uint16_t)0;
	/**
	 * units: V
	 * offset 322
	 */
	scaled_channel<uint16_t, 1000, 1> rawMapFast = (uint16_t)0;
	/**
	 * units: V
	 * offset 324
	 */
	scaled_channel<uint16_t, 1000, 1> rawAfr = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 326
	 */
	uint8_t alignmentFill_at_326[2] = {};
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
	 * Instant MAP
	 * units: kPa
	 * offset 368
	 */
	scaled_channel<uint16_t, 30, 1> instantMAPValue = (uint16_t)0;
	/**
	 * units: us
	 * offset 370
	 */
	uint16_t maxLockedDuration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_WRITE_OK@@
	 * offset 372
	 */
	uint16_t canWriteOk = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_WRITE_ERR@@
	 * offset 374
	 */
	uint16_t canWriteNotOk = (uint16_t)0;
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
	uint8_t starterState = (uint8_t)0;
	/**
	 * offset 393
	 */
	uint8_t starterRelayDisable = (uint8_t)0;
	/**
	 * Ign: Multispark count
	 * offset 394
	 */
	uint8_t multiSparkCounter = (uint8_t)0;
	/**
	 * offset 395
	 */
	uint8_t extiOverflowCount = (uint8_t)0;
	/**
	 * offset 396
	 */
	pid_status_s alternatorStatus;
	/**
	 * offset 412
	 */
	pid_status_s idleStatus;
	/**
	 * offset 428
	 */
	pid_status_s etbStatus;
	/**
	 * offset 444
	 */
	pid_status_s boostStatus;
	/**
	 * offset 460
	 */
	pid_status_s wastegateDcStatus;
	/**
	 * offset 476
	 */
	pid_status_s vvtStatus[CAM_INPUTS_COUNT] = {};
	/**
	 * Aux speed 1
	 * units: s
	 * offset 540
	 */
	uint16_t auxSpeed1 = (uint16_t)0;
	/**
	 * Aux speed 2
	 * units: s
	 * offset 542
	 */
	uint16_t auxSpeed2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ISS@@
	 * units: RPM
	 * offset 544
	 */
	uint16_t ISSValue = (uint16_t)0;
	/**
	 * units: V
	 * offset 546
	 */
	scaled_channel<int16_t, 1000, 1> rawAnalogInput[LUA_ANALOG_INPUT_COUNT] = {};
	/**
	 * GPPWM Output
	 * units: %
	 * offset 562
	 */
	scaled_channel<uint8_t, 2, 1> gppwmOutput[4] = {};
	/**
	 * offset 566
	 */
	scaled_channel<int16_t, 1, 1> gppwmXAxis[4] = {};
	/**
	 * offset 574
	 */
	scaled_channel<int16_t, 10, 1> gppwmYAxis[4] = {};
	/**
	 * units: V
	 * offset 582
	 */
	scaled_channel<int16_t, 1000, 1> rawBattery = (int16_t)0;
	/**
	 * offset 584
	 */
	scaled_channel<int16_t, 10, 1> ignBlendParameter[IGN_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 592
	 */
	scaled_channel<uint8_t, 2, 1> ignBlendBias[IGN_BLEND_COUNT] = {};
	/**
	 * units: deg
	 * offset 596
	 */
	scaled_channel<int16_t, 100, 1> ignBlendOutput[IGN_BLEND_COUNT] = {};
	/**
	 * offset 604
	 */
	scaled_channel<int16_t, 10, 1> ignBlendYAxis[IGN_BLEND_COUNT] = {};
	/**
	 * offset 612
	 */
	scaled_channel<int16_t, 10, 1> veBlendParameter[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 620
	 */
	scaled_channel<uint8_t, 2, 1> veBlendBias[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 624
	 */
	scaled_channel<int16_t, 100, 1> veBlendOutput[VE_BLEND_COUNT] = {};
	/**
	 * offset 632
	 */
	scaled_channel<int16_t, 10, 1> veBlendYAxis[VE_BLEND_COUNT] = {};
	/**
	 * offset 640
	 */
	scaled_channel<int16_t, 10, 1> boostOpenLoopBlendParameter[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 644
	 */
	scaled_channel<uint8_t, 2, 1> boostOpenLoopBlendBias[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 646
	 */
	int8_t boostOpenLoopBlendOutput[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 648
	 */
	scaled_channel<int16_t, 10, 1> boostOpenLoopBlendYAxis[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 652
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendParameter[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 656
	 */
	scaled_channel<uint8_t, 2, 1> boostClosedLoopBlendBias[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 658
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendOutput[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 662
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendYAxis[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 666
	 */
	scaled_channel<int16_t, 10, 1> targetAfrBlendParameter[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 670
	 */
	scaled_channel<uint8_t, 2, 1> targetAfrBlendBias[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 672
	 */
	scaled_channel<int16_t, 10, 1> targetAfrBlendOutput[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * offset 676
	 */
	scaled_channel<int16_t, 10, 1> targetAfrBlendYAxis[TARGET_AFR_BLEND_COUNT] = {};
	/**
	offset 680 bit 0 */
	bool coilState1 : 1 {};
	/**
	offset 680 bit 1 */
	bool coilState2 : 1 {};
	/**
	offset 680 bit 2 */
	bool coilState3 : 1 {};
	/**
	offset 680 bit 3 */
	bool coilState4 : 1 {};
	/**
	offset 680 bit 4 */
	bool coilState5 : 1 {};
	/**
	offset 680 bit 5 */
	bool coilState6 : 1 {};
	/**
	offset 680 bit 6 */
	bool coilState7 : 1 {};
	/**
	offset 680 bit 7 */
	bool coilState8 : 1 {};
	/**
	offset 680 bit 8 */
	bool coilState9 : 1 {};
	/**
	offset 680 bit 9 */
	bool coilState10 : 1 {};
	/**
	offset 680 bit 10 */
	bool coilState11 : 1 {};
	/**
	offset 680 bit 11 */
	bool coilState12 : 1 {};
	/**
	offset 680 bit 12 */
	bool injectorState1 : 1 {};
	/**
	offset 680 bit 13 */
	bool injectorState2 : 1 {};
	/**
	offset 680 bit 14 */
	bool injectorState3 : 1 {};
	/**
	offset 680 bit 15 */
	bool injectorState4 : 1 {};
	/**
	offset 680 bit 16 */
	bool injectorState5 : 1 {};
	/**
	offset 680 bit 17 */
	bool injectorState6 : 1 {};
	/**
	offset 680 bit 18 */
	bool injectorState7 : 1 {};
	/**
	offset 680 bit 19 */
	bool injectorState8 : 1 {};
	/**
	offset 680 bit 20 */
	bool injectorState9 : 1 {};
	/**
	offset 680 bit 21 */
	bool injectorState10 : 1 {};
	/**
	offset 680 bit 22 */
	bool injectorState11 : 1 {};
	/**
	offset 680 bit 23 */
	bool injectorState12 : 1 {};
	/**
	offset 680 bit 24 */
	bool triggerChannel1 : 1 {};
	/**
	offset 680 bit 25 */
	bool triggerChannel2 : 1 {};
	/**
	 * bank 1 intake cam input
	offset 680 bit 26 */
	bool vvtChannel1 : 1 {};
	/**
	 * bank 1 exhaust cam input
	offset 680 bit 27 */
	bool vvtChannel2 : 1 {};
	/**
	 * bank 2 intake cam input
	offset 680 bit 28 */
	bool vvtChannel3 : 1 {};
	/**
	 * bank 2 exhaust cam input
	offset 680 bit 29 */
	bool vvtChannel4 : 1 {};
	/**
	offset 680 bit 30 */
	bool unusedBit_242_30 : 1 {};
	/**
	offset 680 bit 31 */
	bool unusedBit_242_31 : 1 {};
	/**
	 * offset 684
	 */
	uint32_t outputRequestPeriod = (uint32_t)0;
	/**
	 * offset 688
	 */
	float mapFast = (float)0;
	/**
	 * Lua: Gauge
	 * units: value
	 * offset 692
	 */
	float luaGauges[LUA_GAUGE_COUNT] = {};
	/**
	 * units: V
	 * offset 724
	 */
	scaled_channel<uint16_t, 1000, 1> rawMaf2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_MEASURED_2@@
	 * units: kg/h
	 * offset 726
	 */
	scaled_channel<uint16_t, 10, 1> mafMeasured2 = (uint16_t)0;
	/**
	 * offset 728
	 */
	uint16_t schedulingUsedCount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVS@@
	 * units: kph
	 * offset 730
	 */
	scaled_channel<uint16_t, 100, 1> vehicleSpeedKph = (uint16_t)0;
	/**
	 * units: %
	 * offset 732
	 */
	scaled_channel<uint16_t, 100, 1> Gego = (uint16_t)0;
	/**
	 * units: count
	 * offset 734
	 */
	uint16_t testBenchIter = (uint16_t)0;
	/**
	 * units: deg C
	 * offset 736
	 */
	scaled_channel<int16_t, 100, 1> oilTemp = (int16_t)0;
	/**
	 * units: deg C
	 * offset 738
	 */
	scaled_channel<int16_t, 100, 1> fuelTemp = (int16_t)0;
	/**
	 * units: deg C
	 * offset 740
	 */
	scaled_channel<int16_t, 100, 1> ambientTemp = (int16_t)0;
	/**
	 * units: deg C
	 * offset 742
	 */
	scaled_channel<int16_t, 100, 1> compressorDischargeTemp = (int16_t)0;
	/**
	 * units: kPa
	 * offset 744
	 */
	scaled_channel<uint16_t, 30, 1> compressorDischargePressure = (uint16_t)0;
	/**
	 * units: kPa
	 * offset 746
	 */
	scaled_channel<uint16_t, 30, 1> throttleInletPressure = (uint16_t)0;
	/**
	 * units: sec
	 * offset 748
	 */
	uint16_t ignitionOnTime = (uint16_t)0;
	/**
	 * units: sec
	 * offset 750
	 */
	uint16_t engineRunTime = (uint16_t)0;
	/**
	 * units: km
	 * offset 752
	 */
	scaled_channel<uint16_t, 10, 1> distanceTraveled = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR_GAS_SCALE@@
	 * units: AFR
	 * offset 754
	 */
	scaled_channel<uint16_t, 1000, 1> afrGasolineScale = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR2_GAS_SCALE@@
	 * units: AFR
	 * offset 756
	 */
	scaled_channel<uint16_t, 1000, 1> afr2GasolineScale = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 758
	 */
	uint8_t alignmentFill_at_758[2] = {};
	/**
	 * offset 760
	 */
	float wheelSlipRatio = (float)0;
	/**
	 * offset 764
	 */
	uint8_t ignitorDiagnostic[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 776
	 */
	uint8_t injectorDiagnostic[MAX_CYLINDER_COUNT] = {};
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION_STAGE_2@@
	 * units: ms
	 * offset 788
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjectionStage2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_INJ_DUTY_STAGE_2@@
	 * units: %
	 * offset 790
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycleStage2 = (uint8_t)0;
	/**
	 * offset 791
	 */
	uint8_t rawFlexFreq = (uint8_t)0;
	/**
	 * offset 792
	 */
	uint8_t canReWidebandCmdStatus = (uint8_t)0;
	/**
	 * offset 793
	 */
	uint8_t deviceUid = (uint8_t)0;
	/**
	 * offset 794
	 */
	uint16_t mc33810spiErrorCounter = (uint16_t)0;
	/**
	 * offset 796
	 */
	uint8_t sadDwellRatioCounter = (uint8_t)0;
	/**
	 * offset 797
	 */
	uint8_t injectionPrimingCounter = (uint8_t)0;
	/**
	 * offset 798
	 */
	uint8_t tempLogging2 = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 799
	 */
	uint8_t alignmentFill_at_799[1] = {};
	/**
	 * @@GAUGE_NAME_AC_PRESSURE@@
	 * units: kPa
	 * offset 800
	 */
	float acPressure = (float)0;
	/**
	 * units: V
	 * offset 804
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog1 = (int16_t)0;
	/**
	 * units: V
	 * offset 806
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog2 = (int16_t)0;
	/**
	 * units: V
	 * offset 808
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog3 = (int16_t)0;
	/**
	 * units: V
	 * offset 810
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog4 = (int16_t)0;
	/**
	 * ECU: Fast ADC errors
	 * offset 812
	 */
	uint8_t fastAdcErrorCount = (uint8_t)0;
	/**
	 * ECU: Slow ADC errors
	 * offset 813
	 */
	uint8_t slowAdcErrorCount = (uint8_t)0;
	/**
	 * units: V
	 * offset 814
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxTemp1 = (int16_t)0;
	/**
	 * units: V
	 * offset 816
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxTemp2 = (int16_t)0;
	/**
	 * units: V
	 * offset 818
	 */
	scaled_channel<int16_t, 1000, 1> rawAmbientTemp = (int16_t)0;
	/**
	 * offset 820
	 */
	uint32_t rtcUnixEpochTime = (uint32_t)0;
	/**
	 * offset 824
	 */
	int8_t sparkCutReasonBlinker = (int8_t)0;
	/**
	 * offset 825
	 */
	int8_t fuelCutReasonBlinker = (int8_t)0;
	/**
	 * offset 826
	 */
	int16_t hp = (int16_t)0;
	/**
	 * offset 828
	 */
	int16_t torque = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 830
	 */
	uint8_t alignmentFill_at_830[2] = {};
	/**
	 * offset 832
	 */
	float throttlePressureRatio = (float)0;
	/**
	 * offset 836
	 */
	float throttleEffectiveAreaOpening = (float)0;
	/**
	 * offset 840
	 */
	uint32_t mcuSerial = (uint32_t)0;
	/**
	 * offset 844
	 */
	uint8_t sd_error = (uint8_t)0;
	/**
	 * ECU: Fast ADC overruns
	 * offset 845
	 */
	uint8_t fastAdcOverrunCount = (uint8_t)0;
	/**
	 * ECU: Slow ADC overruns
	 * offset 846
	 */
	uint8_t slowAdcOverrunCount = (uint8_t)0;
	/**
	 * ECU: Fast ADC error type
	 * offset 847
	 */
	uint8_t fastAdcLastError = (uint8_t)0;
	/**
	 * ECU: Fast ADC period
	 * units: ticks
	 * offset 848
	 */
	uint16_t fastAdcPeriod = (uint16_t)0;
	/**
	 * ECU: Fast ADC conversions
	 * units: N
	 * offset 850
	 */
	uint16_t fastAdcConversionCount = (uint16_t)0;
	/**
	 * offset 852
	 */
	uint8_t canReWidebandVersion = (uint8_t)0;
	/**
	 * offset 853
	 */
	uint8_t canReWidebandFwDay = (uint8_t)0;
	/**
	 * offset 854
	 */
	uint8_t canReWidebandFwMon = (uint8_t)0;
	/**
	 * offset 855
	 */
	uint8_t canReWidebandFwYear = (uint8_t)0;
	/**
	 * offset 856
	 */
	uint16_t transitionEventCode = (uint16_t)0;
	/**
	 * offset 858
	 */
	uint16_t transitionEventsCounter = (uint16_t)0;
	/**
	 * units: kPa
	 * offset 860
	 */
	uint8_t mapPerCylinder[MAX_CYLINDER_COUNT] = {};
};
static_assert(sizeof(output_channels_s) == 872);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) console/binary/output_channels.txt
