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
	 * @@GAUGE_NAME_CPU_TEMP@@
	 * units: deg C
	 * offset 10
	 */
	int8_t internalMcuTemperature = (int8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 11
	 */
	uint8_t alignmentFill_at_11[1] = {};
	/**
	 * units: V
	 * offset 12
	 */
	scaled_channel<int16_t, 1000, 1> internalVref = (int16_t)0;
	/**
	 * units: V
	 * offset 14
	 */
	scaled_channel<int16_t, 1000, 1> internalVbat = (int16_t)0;
	/**
	 * @@GAUGE_NAME_CLT@@
	 * units: deg C
	 * offset 16
	 */
	scaled_channel<int16_t, 100, 1> coolant = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IAT@@
	 * units: deg C
	 * offset 18
	 */
	scaled_channel<int16_t, 100, 1> intake = (int16_t)0;
	/**
	 * units: deg C
	 * offset 20
	 */
	scaled_channel<int16_t, 100, 1> auxTemp1 = (int16_t)0;
	/**
	 * units: deg C
	 * offset 22
	 */
	scaled_channel<int16_t, 100, 1> auxTemp2 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TPS@@
	 * units: %
	 * offset 24
	 */
	scaled_channel<int16_t, 100, 1> TPSValue = (int16_t)0;
	/**
	 * @@GAUGE_NAME_THROTTLE_PEDAL@@
	 * units: %
	 * offset 26
	 */
	scaled_channel<int16_t, 100, 1> throttlePedalPosition = (int16_t)0;
	/**
	 * units: ADC
	 * offset 28
	 */
	uint16_t tpsADC = (uint16_t)0;
	/**
	 * units: V
	 * offset 30
	 */
	scaled_channel<uint16_t, 1000, 1> rawMaf = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_MEASURED@@
	 * units: kg/h
	 * offset 32
	 */
	scaled_channel<uint16_t, 10, 1> mafMeasured = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_MAP@@
	 * units: kPa
	 * offset 34
	 */
	scaled_channel<uint16_t, 30, 1> MAPValue = (uint16_t)0;
	/**
	 * units: kPa
	 * offset 36
	 */
	scaled_channel<uint16_t, 30, 1> baroPressure = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA@@
	 * offset 38
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VBAT@@
	 * units: V
	 * offset 40
	 */
	scaled_channel<uint16_t, 1000, 1> VBatt = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VIGN@@
	 * units: V
	 * offset 42
	 */
	scaled_channel<uint16_t, 1000, 1> VIgn = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_OIL_PRESSURE@@
	 * units: kPa
	 * offset 44
	 */
	scaled_channel<uint16_t, 30, 1> oilPressure = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1I@@
	 * units: deg
	 * offset 46
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION@@
	 * Actual last injection time - including all compensation and injection mode
	 * units: ms
	 * offset 48
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjection = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 50
	 */
	uint8_t alignmentFill_at_50[2] = {};
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION_RATIO@@
	 * Last injection time divided to previous injection time
	 * offset 52
	 */
	float actualLastInjectionRatio = (float)0;
	/**
	 * offset 56
	 */
	uint8_t stopEngineCode = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_INJ_DUTY@@
	 * units: %
	 * offset 57
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycle = (uint8_t)0;
	/**
	 * offset 58
	 */
	uint8_t tempLogging1 = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 59
	 */
	uint8_t alignmentFill_at_59[1] = {};
	/**
	 * @@GAUGE_NAME_FUEL_INJECTION_TIMING@@
	 * units: deg
	 * offset 60
	 */
	int16_t injectionOffset = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ENGINE_CRC16@@
	 * units: crc16
	 * offset 62
	 */
	uint16_t engineMakeCodeNameCrc16 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_AMOUNT@@
	 * units: mg
	 * offset 64
	 */
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_CORRECTION@@
	 * units: mg
	 * offset 66
	 */
	scaled_channel<int16_t, 100, 1> wallFuelCorrectionValue = (int16_t)0;
	/**
	 * Flex: AE multiplier
	 * units: mult
	 * offset 68
	 */
	scaled_channel<uint16_t, 1000, 1> flexAeMultiplier = (uint16_t)0;
	/**
	 * Flex: WW tau multiplier
	 * units: mult
	 * offset 70
	 */
	scaled_channel<uint16_t, 1000, 1> flexWwTauMultiplier = (uint16_t)0;
	/**
	 * Flex: WW beta multiplier
	 * units: mult
	 * offset 72
	 */
	scaled_channel<uint16_t, 1000, 1> flexWwBetaMultiplier = (uint16_t)0;
	/**
	 * offset 74
	 */
	uint16_t revolutionCounterSinceStart = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_READ_OK@@
	 * offset 76
	 */
	uint16_t canReadCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_TPS_EXTRA@@
	 * units: ms
	 * offset 78
	 */
	scaled_channel<int16_t, 300, 1> tpsAccelFuel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IGNITION_MODE@@
	 * offset 80
	 */
	uint8_t currentIgnitionMode = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_INJECTION_MODE@@
	 * offset 81
	 */
	uint8_t currentInjectionMode = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_DWELL_DUTY@@
	 * units: %
	 * offset 82
	 */
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ETB_DUTY@@
	 * units: %
	 * offset 84
	 */
	scaled_channel<int16_t, 100, 1> etb1DutyCycle = (int16_t)0;
	/**
	 * Fuel level
	 * units: %
	 * offset 86
	 */
	scaled_channel<int16_t, 100, 1> fuelTankLevel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_CONSUMPTION@@
	 * units: grams
	 * offset 88
	 */
	uint16_t totalFuelConsumption = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_FLOW@@
	 * units: gram/s
	 * offset 90
	 */
	scaled_channel<uint16_t, 200, 1> fuelFlowRate = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TPS2@@
	 * units: %
	 * offset 92
	 */
	scaled_channel<int16_t, 100, 1> TPS2Value = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TUNE_CRC16@@
	 * units: crc16
	 * offset 94
	 */
	uint16_t tuneCrc16 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_VE@@
	 * units: ratio
	 * offset 96
	 */
	scaled_channel<uint16_t, 10, 1> veValue = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 98
	 */
	uint8_t alignmentFill_at_98[2] = {};
	/**
	 * @@GAUGE_NAME_UPTIME@@
	 * units: sec
	 * offset 100
	 */
	uint32_t seconds = (uint32_t)0;
	/**
	 * Engine Mode
	 * units: em
	 * offset 104
	 */
	uint32_t engineMode = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_VERSION@@
	 * units: version_f
	 * offset 108
	 */
	uint32_t firmwareVersion = (uint32_t)0;
	/**
	 * units: V
	 * offset 112
	 */
	scaled_channel<int16_t, 1000, 1> rawIdlePositionSensor = (int16_t)0;
	/**
	 * units: V
	 * offset 114
	 */
	scaled_channel<int16_t, 1000, 1> rawWastegatePosition = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_LAT@@
	 * units: G
	 * offset 116
	 */
	scaled_channel<int16_t, 1000, 1> accelerationLat = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_LON@@
	 * units: G
	 * offset 118
	 */
	scaled_channel<int16_t, 1000, 1> accelerationLon = (int16_t)0;
	/**
	 * @@GAUGE_NAME_DETECTED_GEAR@@
	 * offset 120
	 */
	uint8_t detectedGear = (uint8_t)0;
	/**
	 * offset 121
	 */
	uint8_t maxTriggerReentrant = (uint8_t)0;
	/**
	 * units: V
	 * offset 122
	 */
	scaled_channel<int16_t, 1000, 1> rawLowFuelPressure = (int16_t)0;
	/**
	 * units: V
	 * offset 124
	 */
	scaled_channel<int16_t, 1000, 1> rawHighFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_LOW@@
	 * units: kpa
	 * offset 126
	 */
	scaled_channel<int16_t, 30, 1> lowFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_DESIRED_GEAR@@
	 * units: gear
	 * offset 128
	 */
	int8_t tcuDesiredGear = (int8_t)0;
	/**
	 * @@GAUGE_NAME_FLEX@@
	 * units: %
	 * offset 129
	 */
	scaled_channel<uint8_t, 2, 1> flexPercent = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_WG_POSITION@@
	 * units: %
	 * offset 130
	 */
	scaled_channel<int16_t, 100, 1> wastegatePositionSensor = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_HIGH@@
	 * units: bar
	 * offset 132
	 */
	scaled_channel<int16_t, 10, 1> highFuelPressure = (int16_t)0;
	/**
	 * offset 134
	 */
	uint8_t tempLogging3 = (uint8_t)0;
	/**
	 * offset 135
	 */
	uint8_t tempLogging4 = (uint8_t)0;
	/**
	 * offset 136
	 */
	float calibrationValue = (float)0;
	/**
	 * offset 140
	 */
	uint8_t calibrationMode = (uint8_t)0;
	/**
	 * Idle: Stepper target position
	 * offset 141
	 */
	uint8_t idleStepperTargetPosition = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 142
	 */
	uint8_t alignmentFill_at_142[2] = {};
	/**
	 * @@GAUGE_NAME_TRG_ERR@@
	 * units: counter
	 * offset 144
	 */
	uint32_t totalTriggerErrorCounter = (uint32_t)0;
	/**
	 * offset 148
	 */
	uint32_t orderingErrorCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_COUNTER@@
	 * units: count
	 * offset 152
	 */
	uint16_t warningCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_LAST@@
	 * units: error
	 * offset 154
	 */
	uint16_t lastErrorCode = (uint16_t)0;
	/**
	 * Warning code
	 * units: error
	 * offset 156
	 */
	uint16_t recentErrorCode[8] = {};
	/**
	 * units: val
	 * offset 172
	 */
	float debugFloatField1 = (float)0;
	/**
	 * units: val
	 * offset 176
	 */
	float debugFloatField2 = (float)0;
	/**
	 * units: val
	 * offset 180
	 */
	float debugFloatField3 = (float)0;
	/**
	 * units: val
	 * offset 184
	 */
	float debugFloatField4 = (float)0;
	/**
	 * units: val
	 * offset 188
	 */
	float debugFloatField5 = (float)0;
	/**
	 * units: val
	 * offset 192
	 */
	float debugFloatField6 = (float)0;
	/**
	 * units: val
	 * offset 196
	 */
	float debugFloatField7 = (float)0;
	/**
	 * units: val
	 * offset 200
	 */
	uint32_t debugIntField1 = (uint32_t)0;
	/**
	 * units: val
	 * offset 204
	 */
	uint32_t debugIntField2 = (uint32_t)0;
	/**
	 * units: val
	 * offset 208
	 */
	uint32_t debugIntField3 = (uint32_t)0;
	/**
	 * units: val
	 * offset 212
	 */
	uint32_t debugIntField4 = (uint32_t)0;
	/**
	 * units: val
	 * offset 216
	 */
	uint32_t debugIntField5 = (uint32_t)0;
	/**
	 * EGT
	 * units: deg C
	 * offset 220
	 */
	scaled_channel<int16_t, 4, 1> egt[EGT_CHANNEL_COUNT] = {};
	/**
	 * units: V
	 * offset 236
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Primary = (int16_t)0;
	/**
	 * units: V
	 * offset 238
	 */
	scaled_channel<int16_t, 1000, 1> rawClt = (int16_t)0;
	/**
	 * units: V
	 * offset 240
	 */
	scaled_channel<int16_t, 1000, 1> rawIat = (int16_t)0;
	/**
	 * units: V
	 * offset 242
	 */
	scaled_channel<int16_t, 1000, 1> rawOilPressure = (int16_t)0;
	/**
	 * units: V
	 * offset 244
	 */
	scaled_channel<int16_t, 1000, 1> rawAcPressure = (int16_t)0;
	/**
	 * units: V
	 * offset 246
	 */
	scaled_channel<int16_t, 1000, 1> rawFuelLevel = (int16_t)0;
	/**
	 * units: V
	 * offset 248
	 */
	float rawPpsPrimary = (float)0;
	/**
	 * units: V
	 * offset 252
	 */
	float rawPpsSecondary = (float)0;
	/**
	 * units: V
	 * offset 256
	 */
	float rawRawPpsPrimary = (float)0;
	/**
	 * units: V
	 * offset 260
	 */
	float rawRawPpsSecondary = (float)0;
	/**
	 * @@GAUGE_NAME_IDLE_POSITION@@
	 * units: %
	 * offset 264
	 */
	scaled_channel<int16_t, 100, 1> idlePositionSensor = (int16_t)0;
	/**
	 * @@GAUGE_NAME_AFR@@
	 * units: AFR
	 * offset 266
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR2@@
	 * units: AFR
	 * offset 268
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue2 = (uint16_t)0;
	/**
	 * @@SMOOTHED_GAUGE_NAME_AFR@@
	 * units: AFR
	 * offset 270
	 */
	scaled_channel<uint16_t, 1000, 1> SmoothedAFRValue = (uint16_t)0;
	/**
	 * @@SMOOTHED_GAUGE_NAME_AFR2@@
	 * units: AFR
	 * offset 272
	 */
	scaled_channel<uint16_t, 1000, 1> SmoothedAFRValue2 = (uint16_t)0;
	/**
	 * Vss Accel
	 * units: m/s2
	 * offset 274
	 */
	scaled_channel<uint16_t, 300, 1> VssAcceleration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA2@@
	 * offset 276
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1E@@
	 * units: deg
	 * offset 278
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1E = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2I@@
	 * units: deg
	 * offset 280
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2E@@
	 * units: deg
	 * offset 282
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2E = (int16_t)0;
	/**
	 * units: V
	 * offset 284
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Secondary = (int16_t)0;
	/**
	 * units: V
	 * offset 286
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Primary = (int16_t)0;
	/**
	 * units: V
	 * offset 288
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Secondary = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_VERT@@
	 * units: G
	 * offset 290
	 */
	scaled_channel<int16_t, 1000, 1> accelerationVert = (int16_t)0;
	/**
	 * @@GAUGE_NAME_GYRO_YAW@@
	 * units: deg/sec
	 * offset 292
	 */
	scaled_channel<int16_t, 1000, 1> gyroYaw = (int16_t)0;
	/**
	 * units: deg
	 * offset 294
	 */
	int8_t vvtTargets[4] = {};
	/**
	 * @@GAUGE_NAME_TURBO_SPEED@@
	 * units: hz
	 * offset 298
	 */
	uint16_t turboSpeed = (uint16_t)0;
	/**
	 * Ign: Timing Cyl
	 * units: deg
	 * offset 300
	 */
	scaled_channel<int16_t, 50, 1> ignitionAdvanceCyl[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: %
	 * offset 324
	 */
	scaled_channel<int16_t, 100, 1> tps1Split = (int16_t)0;
	/**
	 * units: %
	 * offset 326
	 */
	scaled_channel<int16_t, 100, 1> tps2Split = (int16_t)0;
	/**
	 * units: %
	 * offset 328
	 */
	scaled_channel<int16_t, 100, 1> tps12Split = (int16_t)0;
	/**
	 * units: %
	 * offset 330
	 */
	scaled_channel<int16_t, 100, 1> accPedalSplit = (int16_t)0;
	/**
	 * units: %
	 * offset 332
	 */
	scaled_channel<int16_t, 100, 1> accPedalUnfiltered = (int16_t)0;
	/**
	 * Ign: Cut Code
	 * units: code
	 * offset 334
	 */
	int8_t sparkCutReason = (int8_t)0;
	/**
	 * Fuel: Cut Code
	 * units: code
	 * offset 335
	 */
	int8_t fuelCutReason = (int8_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_ESTIMATE@@
	 * units: kg/h
	 * offset 336
	 */
	scaled_channel<uint16_t, 10, 1> mafEstimate = (uint16_t)0;
	/**
	 * sync: instant RPM
	 * units: rpm
	 * offset 338
	 */
	uint16_t instantRpm = (uint16_t)0;
	/**
	 * units: V
	 * offset 340
	 */
	scaled_channel<uint16_t, 1000, 1> rawMap = (uint16_t)0;
	/**
	 * units: V
	 * offset 342
	 */
	scaled_channel<uint16_t, 1000, 1> rawMapFast = (uint16_t)0;
	/**
	 * units: V
	 * offset 344
	 */
	scaled_channel<uint16_t, 1000, 1> rawAfr = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 346
	 */
	uint8_t alignmentFill_at_346[2] = {};
	/**
	 * offset 348
	 */
	float calibrationValue2 = (float)0;
	/**
	 * Lua: Tick counter
	 * units: count
	 * offset 352
	 */
	uint32_t luaInvocationCounter = (uint32_t)0;
	/**
	 * Lua: Last tick duration
	 * units: nt
	 * offset 356
	 */
	uint32_t luaLastCycleDuration = (uint32_t)0;
	/**
	 * offset 360
	 */
	uint32_t vssEdgeCounter = (uint32_t)0;
	/**
	 * offset 364
	 */
	uint32_t issEdgeCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_1@@
	 * offset 368
	 */
	float auxLinear1 = (float)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_2@@
	 * offset 372
	 */
	float auxLinear2 = (float)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_3@@
	 * offset 376
	 */
	float auxLinear3 = (float)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_4@@
	 * offset 380
	 */
	float auxLinear4 = (float)0;
	/**
	 * units: kPa
	 * offset 384
	 */
	scaled_channel<uint16_t, 10, 1> fallbackMap = (uint16_t)0;
	/**
	 * Effective MAP
	 * units: kPa
	 * offset 386
	 */
	scaled_channel<uint16_t, 10, 1> effectiveMap = (uint16_t)0;
	/**
	 * AE: Map Pred New Cycles
	 * offset 388
	 */
	uint16_t predTimerResetCnt = (uint16_t)0;
	/**
	 * AE: Map Pred Expired
	 * offset 390
	 */
	uint16_t mapPredEventOver = (uint16_t)0;
	/**
	 * Instant MAP
	 * units: kPa
	 * offset 392
	 */
	scaled_channel<uint16_t, 30, 1> instantMAPValue = (uint16_t)0;
	/**
	 * units: us
	 * offset 394
	 */
	uint16_t maxLockedDuration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_WRITE_OK@@
	 * offset 396
	 */
	uint16_t canWriteOk = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_CAN_WRITE_ERR@@
	 * offset 398
	 */
	uint16_t canWriteNotOk = (uint16_t)0;
	/**
	 * offset 400
	 */
	uint32_t triggerPrimaryFall = (uint32_t)0;
	/**
	 * offset 404
	 */
	uint32_t triggerPrimaryRise = (uint32_t)0;
	/**
	 * offset 408
	 */
	uint32_t triggerSecondaryFall = (uint32_t)0;
	/**
	 * offset 412
	 */
	uint32_t triggerSecondaryRise = (uint32_t)0;
	/**
	 * offset 416
	 */
	uint8_t starterState = (uint8_t)0;
	/**
	 * offset 417
	 */
	uint8_t starterRelayDisable = (uint8_t)0;
	/**
	 * Ign: Multispark count
	 * offset 418
	 */
	uint8_t multiSparkCounter = (uint8_t)0;
	/**
	 * offset 419
	 */
	uint8_t extiOverflowCount = (uint8_t)0;
	/**
	 * offset 420
	 */
	pid_status_s alternatorStatus;
	/**
	 * offset 436
	 */
	pid_status_s idleStatus;
	/**
	 * offset 452
	 */
	pid_status_s etbStatus;
	/**
	 * offset 468
	 */
	pid_status_s boostStatus;
	/**
	 * offset 484
	 */
	pid_status_s wastegateDcStatus;
	/**
	 * offset 500
	 */
	pid_status_s vvtStatus[CAM_INPUTS_COUNT] = {};
	/**
	 * Aux speed 1
	 * units: s
	 * offset 564
	 */
	uint16_t auxSpeed1 = (uint16_t)0;
	/**
	 * Aux speed 2
	 * units: s
	 * offset 566
	 */
	uint16_t auxSpeed2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ISS@@
	 * units: RPM
	 * offset 568
	 */
	uint16_t ISSValue = (uint16_t)0;
	/**
	 * units: V
	 * offset 570
	 */
	scaled_channel<int16_t, 1000, 1> rawAnalogInput[LUA_ANALOG_INPUT_COUNT] = {};
	/**
	 * GPPWM Output
	 * units: %
	 * offset 586
	 */
	scaled_channel<uint8_t, 2, 1> gppwmOutput[4] = {};
	/**
	 * offset 590
	 */
	scaled_channel<int16_t, 1, 1> gppwmXAxis[4] = {};
	/**
	 * offset 598
	 */
	scaled_channel<int16_t, 10, 1> gppwmYAxis[4] = {};
	/**
	 * units: V
	 * offset 606
	 */
	scaled_channel<int16_t, 1000, 1> rawBattery = (int16_t)0;
	/**
	 * offset 608
	 */
	scaled_channel<int16_t, 10, 1> ignBlendParameter[IGN_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 616
	 */
	scaled_channel<uint8_t, 2, 1> ignBlendBias[IGN_BLEND_COUNT] = {};
	/**
	 * units: deg
	 * offset 620
	 */
	scaled_channel<int16_t, 100, 1> ignBlendOutput[IGN_BLEND_COUNT] = {};
	/**
	 * offset 628
	 */
	scaled_channel<int16_t, 10, 1> ignBlendYAxis[IGN_BLEND_COUNT] = {};
	/**
	 * offset 636
	 */
	scaled_channel<int16_t, 10, 1> veBlendParameter[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 644
	 */
	scaled_channel<uint8_t, 2, 1> veBlendBias[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 648
	 */
	scaled_channel<int16_t, 100, 1> veBlendOutput[VE_BLEND_COUNT] = {};
	/**
	 * offset 656
	 */
	scaled_channel<int16_t, 10, 1> veBlendYAxis[VE_BLEND_COUNT] = {};
	/**
	 * offset 664
	 */
	scaled_channel<int16_t, 10, 1> secondVeBlendParameter = (int16_t)0;
	/**
	 * units: %
	 * offset 666
	 */
	scaled_channel<uint8_t, 2, 1> secondVeBlendBias = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 667
	 */
	uint8_t alignmentFill_at_667[1] = {};
	/**
	 * offset 668
	 */
	scaled_channel<int16_t, 10, 1> secondIgnitionBlendParameter = (int16_t)0;
	/**
	 * units: %
	 * offset 670
	 */
	scaled_channel<uint8_t, 2, 1> secondIgnitionBlendBias = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 671
	 */
	uint8_t alignmentFill_at_671[1] = {};
	/**
	 * offset 672
	 */
	scaled_channel<int16_t, 10, 1> boostOpenLoopBlendParameter[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 676
	 */
	scaled_channel<uint8_t, 2, 1> boostOpenLoopBlendBias[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 678
	 */
	int8_t boostOpenLoopBlendOutput[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 680
	 */
	scaled_channel<int16_t, 10, 1> boostOpenLoopBlendYAxis[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 684
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendParameter[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 688
	 */
	scaled_channel<uint8_t, 2, 1> boostClosedLoopBlendBias[BOOST_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 690
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendOutput[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 694
	 */
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendYAxis[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 698
	 */
	scaled_channel<int16_t, 10, 1> targetAfrBlendParameter[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 702
	 */
	scaled_channel<uint8_t, 2, 1> targetAfrBlendBias[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 704
	 */
	scaled_channel<int16_t, 10, 1> targetAfrBlendOutput[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * offset 708
	 */
	scaled_channel<int16_t, 10, 1> targetAfrBlendYAxis[TARGET_AFR_BLEND_COUNT] = {};
	/**
	offset 712 bit 0 */
	bool coilState1 : 1 {};
	/**
	offset 712 bit 1 */
	bool coilState2 : 1 {};
	/**
	offset 712 bit 2 */
	bool coilState3 : 1 {};
	/**
	offset 712 bit 3 */
	bool coilState4 : 1 {};
	/**
	offset 712 bit 4 */
	bool coilState5 : 1 {};
	/**
	offset 712 bit 5 */
	bool coilState6 : 1 {};
	/**
	offset 712 bit 6 */
	bool coilState7 : 1 {};
	/**
	offset 712 bit 7 */
	bool coilState8 : 1 {};
	/**
	offset 712 bit 8 */
	bool coilState9 : 1 {};
	/**
	offset 712 bit 9 */
	bool coilState10 : 1 {};
	/**
	offset 712 bit 10 */
	bool coilState11 : 1 {};
	/**
	offset 712 bit 11 */
	bool coilState12 : 1 {};
	/**
	offset 712 bit 12 */
	bool injectorState1 : 1 {};
	/**
	offset 712 bit 13 */
	bool injectorState2 : 1 {};
	/**
	offset 712 bit 14 */
	bool injectorState3 : 1 {};
	/**
	offset 712 bit 15 */
	bool injectorState4 : 1 {};
	/**
	offset 712 bit 16 */
	bool injectorState5 : 1 {};
	/**
	offset 712 bit 17 */
	bool injectorState6 : 1 {};
	/**
	offset 712 bit 18 */
	bool injectorState7 : 1 {};
	/**
	offset 712 bit 19 */
	bool injectorState8 : 1 {};
	/**
	offset 712 bit 20 */
	bool injectorState9 : 1 {};
	/**
	offset 712 bit 21 */
	bool injectorState10 : 1 {};
	/**
	offset 712 bit 22 */
	bool injectorState11 : 1 {};
	/**
	offset 712 bit 23 */
	bool injectorState12 : 1 {};
	/**
	offset 712 bit 24 */
	bool triggerChannel1 : 1 {};
	/**
	offset 712 bit 25 */
	bool triggerChannel2 : 1 {};
	/**
	 * bank 1 intake cam input
	offset 712 bit 26 */
	bool vvtChannel1 : 1 {};
	/**
	 * bank 1 exhaust cam input
	offset 712 bit 27 */
	bool vvtChannel2 : 1 {};
	/**
	 * bank 2 intake cam input
	offset 712 bit 28 */
	bool vvtChannel3 : 1 {};
	/**
	 * bank 2 exhaust cam input
	offset 712 bit 29 */
	bool vvtChannel4 : 1 {};
	/**
	 * AE: Map Prediction Active
	offset 712 bit 30 */
	bool isMapPredictionActive : 1 {};
	/**
	 * Error: Flex
	offset 712 bit 31 */
	bool isFlexError : 1 {};
	/**
	 * offset 716
	 */
	uint32_t outputRequestPeriod = (uint32_t)0;
	/**
	 * offset 720
	 */
	float mapFast = (float)0;
	/**
	 * Lua: Gauge
	 * units: value
	 * offset 724
	 */
	float luaGauges[LUA_GAUGE_COUNT] = {};
	/**
	 * units: V
	 * offset 756
	 */
	scaled_channel<uint16_t, 1000, 1> rawMaf2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_MEASURED_2@@
	 * units: kg/h
	 * offset 758
	 */
	scaled_channel<uint16_t, 10, 1> mafMeasured2 = (uint16_t)0;
	/**
	 * offset 760
	 */
	uint16_t schedulingUsedCount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVS@@
	 * units: kph
	 * offset 762
	 */
	scaled_channel<uint16_t, 100, 1> vehicleSpeedKph = (uint16_t)0;
	/**
	 * units: %
	 * offset 764
	 */
	scaled_channel<uint16_t, 100, 1> Gego = (uint16_t)0;
	/**
	 * units: count
	 * offset 766
	 */
	uint16_t testBenchIter = (uint16_t)0;
	/**
	 * units: deg C
	 * offset 768
	 */
	scaled_channel<int16_t, 100, 1> oilTemp = (int16_t)0;
	/**
	 * units: deg C
	 * offset 770
	 */
	scaled_channel<int16_t, 100, 1> fuelTemp = (int16_t)0;
	/**
	 * units: deg C
	 * offset 772
	 */
	scaled_channel<int16_t, 100, 1> ambientTemp = (int16_t)0;
	/**
	 * units: deg C
	 * offset 774
	 */
	scaled_channel<int16_t, 100, 1> compressorDischargeTemp = (int16_t)0;
	/**
	 * units: kPa
	 * offset 776
	 */
	scaled_channel<uint16_t, 30, 1> compressorDischargePressure = (uint16_t)0;
	/**
	 * units: kPa
	 * offset 778
	 */
	scaled_channel<uint16_t, 30, 1> throttleInletPressure = (uint16_t)0;
	/**
	 * units: sec
	 * offset 780
	 */
	uint16_t ignitionOnTime = (uint16_t)0;
	/**
	 * units: sec
	 * offset 782
	 */
	uint16_t engineRunTime = (uint16_t)0;
	/**
	 * units: km
	 * offset 784
	 */
	scaled_channel<uint16_t, 10, 1> distanceTraveled = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR_GAS_SCALE@@
	 * units: AFR
	 * offset 786
	 */
	scaled_channel<uint16_t, 1000, 1> afrGasolineScale = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR2_GAS_SCALE@@
	 * units: AFR
	 * offset 788
	 */
	scaled_channel<uint16_t, 1000, 1> afr2GasolineScale = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 790
	 */
	uint8_t alignmentFill_at_790[2] = {};
	/**
	 * offset 792
	 */
	float wheelSlipRatio = (float)0;
	/**
	 * offset 796
	 */
	uint8_t ignitorDiagnostic[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 808
	 */
	uint8_t injectorDiagnostic[MAX_CYLINDER_COUNT] = {};
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION_STAGE_2@@
	 * units: ms
	 * offset 820
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjectionStage2 = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 822
	 */
	uint8_t alignmentFill_at_822[2] = {};
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION_RATIO_STAGE_2@@
	 * offset 824
	 */
	float actualLastInjectionRatioStage2 = (float)0;
	/**
	 * @@GAUGE_NAME_FUEL_INJ_DUTY_STAGE_2@@
	 * units: %
	 * offset 828
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycleStage2 = (uint8_t)0;
	/**
	 * offset 829
	 */
	uint8_t rawFlexFreq = (uint8_t)0;
	/**
	 * offset 830
	 */
	uint8_t canReWidebandCmdStatus = (uint8_t)0;
	/**
	 * offset 831
	 */
	uint8_t deviceUid = (uint8_t)0;
	/**
	 * offset 832
	 */
	uint16_t mc33810spiErrorCounter = (uint16_t)0;
	/**
	 * offset 834
	 */
	uint8_t injectionPrimingCounter = (uint8_t)0;
	/**
	 * offset 835
	 */
	uint8_t tempLogging2 = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_AC_PRESSURE@@
	 * units: kPa
	 * offset 836
	 */
	float acPressure = (float)0;
	/**
	 * units: V
	 * offset 840
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog1 = (int16_t)0;
	/**
	 * units: V
	 * offset 842
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog2 = (int16_t)0;
	/**
	 * units: V
	 * offset 844
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog3 = (int16_t)0;
	/**
	 * units: V
	 * offset 846
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxAnalog4 = (int16_t)0;
	/**
	 * ECU: Fast ADC errors
	 * offset 848
	 */
	uint8_t fastAdcErrorCount = (uint8_t)0;
	/**
	 * ECU: Slow ADC errors
	 * offset 849
	 */
	uint8_t slowAdcErrorCount = (uint8_t)0;
	/**
	 * units: V
	 * offset 850
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxTemp1 = (int16_t)0;
	/**
	 * units: V
	 * offset 852
	 */
	scaled_channel<int16_t, 1000, 1> rawAuxTemp2 = (int16_t)0;
	/**
	 * units: V
	 * offset 854
	 */
	scaled_channel<int16_t, 1000, 1> rawAmbientTemp = (int16_t)0;
	/**
	 * offset 856
	 */
	uint32_t rtcUnixEpochTime = (uint32_t)0;
	/**
	 * offset 860
	 */
	int8_t sparkCutReasonBlinker = (int8_t)0;
	/**
	 * offset 861
	 */
	int8_t fuelCutReasonBlinker = (int8_t)0;
	/**
	 * offset 862
	 */
	int16_t hp = (int16_t)0;
	/**
	 * offset 864
	 */
	int16_t torque = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 866
	 */
	uint8_t alignmentFill_at_866[2] = {};
	/**
	 * offset 868
	 */
	float throttlePressureRatio = (float)0;
	/**
	 * offset 872
	 */
	float throttleEffectiveAreaOpening = (float)0;
	/**
	 * offset 876
	 */
	uint32_t mcuSerial = (uint32_t)0;
	/**
	 * offset 880
	 */
	uint8_t sd_error = (uint8_t)0;
	/**
	 * SD: Logging state
	 * units: code
	 * offset 881
	 */
	uint8_t sdLoggingState = (uint8_t)0;
	/**
	 * ECU: Fast ADC overruns
	 * offset 882
	 */
	uint8_t fastAdcOverrunCount = (uint8_t)0;
	/**
	 * ECU: Slow ADC overruns
	 * offset 883
	 */
	uint8_t slowAdcOverrunCount = (uint8_t)0;
	/**
	 * ECU: Fast ADC error type
	 * offset 884
	 */
	uint8_t fastAdcLastError = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 885
	 */
	uint8_t alignmentFill_at_885[1] = {};
	/**
	 * ECU: Fast ADC period
	 * units: ticks
	 * offset 886
	 */
	uint16_t fastAdcPeriod = (uint16_t)0;
	/**
	 * ECU: Fast ADC conversions
	 * units: N
	 * offset 888
	 */
	uint16_t fastAdcConversionCount = (uint16_t)0;
	/**
	 * offset 890
	 */
	uint8_t canReWidebandVersion = (uint8_t)0;
	/**
	 * offset 891
	 */
	uint8_t canReWidebandFwDay = (uint8_t)0;
	/**
	 * offset 892
	 */
	uint8_t canReWidebandFwMon = (uint8_t)0;
	/**
	 * offset 893
	 */
	uint8_t canReWidebandFwYear = (uint8_t)0;
	/**
	 * offset 894
	 */
	uint16_t transitionEventCode = (uint16_t)0;
	/**
	 * offset 896
	 */
	uint16_t transitionEventsCounter = (uint16_t)0;
	/**
	 * units: kPa
	 * offset 898
	 */
	uint8_t mapPerCylinder[MAX_CYLINDER_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 910
	 */
	uint8_t alignmentFill_at_910[2] = {};
};
static_assert(sizeof(output_channels_s) == 912);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) console/binary/output_channels.txt
