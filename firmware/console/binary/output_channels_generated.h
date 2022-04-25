// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Mon Apr 25 03:43:32 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of pid_status_s
struct pid_status_s {
	/**
	 * offset 0
	 */
	scaled_channel<int16_t, 100, 1> iTerm = (int16_t)0;
	/**
	 * offset 2
	 */
	scaled_channel<int16_t, 100, 1> dTerm = (int16_t)0;
	/**
	 * offset 4
	 */
	scaled_channel<int16_t, 100, 1> output = (int16_t)0;
	/**
	 * offset 6
	 */
	scaled_channel<int16_t, 100, 1> error = (int16_t)0;
	/**
	 * offset 8
	 */
	uint32_t resetCounter = (uint32_t)0;
};
static_assert(sizeof(pid_status_s) == 12);

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
	 * need 4 byte alignment
	units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2];
	/**
	 * @@GAUGE_NAME_VVS@@
	kph
	 * offset 12
	 */
	scaled_channel<uint8_t, 1, 1> vehicleSpeedKph = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 13
	 */
	uint8_t alignmentFill_at_13[3];
	/**
	 * @@GAUGE_NAME_CPU_TEMP@@
	deg C
	 * offset 16
	 */
	scaled_channel<int8_t, 1, 1> internalMcuTemperature = (int8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 17
	 */
	uint8_t alignmentFill_at_17[1];
	/**
	 * @@GAUGE_NAME_CLT@@
	deg C
	 * offset 18
	 */
	scaled_channel<int16_t, 100, 1> coolant = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IAT@@
	deg C
	 * offset 20
	 */
	scaled_channel<int16_t, 100, 1> intake = (int16_t)0;
	/**
	deg C
	 * offset 22
	 */
	scaled_channel<int16_t, 100, 1> auxTemp1 = (int16_t)0;
	/**
	deg C
	 * offset 24
	 */
	scaled_channel<int16_t, 100, 1> auxTemp2 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TPS@@
	%
	 * offset 26
	 */
	scaled_channel<int16_t, 100, 1> TPSValue = (int16_t)0;
	/**
	 * @@GAUGE_NAME_THROTTLE_PEDAL@@
	%
	 * offset 28
	 */
	scaled_channel<int16_t, 100, 1> throttlePedalPosition = (int16_t)0;
	/**
	ADC
	 * offset 30
	 */
	uint16_t tpsADC = (uint16_t)0;
	/**
	V
	 * offset 32
	 */
	scaled_channel<uint16_t, 1000, 1> rawMaf = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_MEASURED@@
	kg/h
	 * offset 34
	 */
	scaled_channel<uint16_t, 10, 1> mafMeasured = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_MAP@@
	kPa
	 * offset 36
	 */
	scaled_channel<uint16_t, 30, 1> MAPValue = (uint16_t)0;
	/**
	kPa
	 * offset 38
	 */
	scaled_channel<uint16_t, 30, 1> baroPressure = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA@@
	 * offset 40
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 42
	 */
	uint8_t alignmentFill_at_42[2];
	/**
	deg
	 * offset 44
	 */
	scaled_channel<uint8_t, 10, 1> knockRetard = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 45
	 */
	uint8_t alignmentFill_at_45[3];
	/**
	%
	 * offset 48
	 */
	scaled_channel<uint8_t, 2, 1> idleCurrentPosition = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 49
	 */
	uint8_t alignmentFill_at_49[1];
	/**
	 * @@GAUGE_NAME_VBAT@@
	V
	 * offset 50
	 */
	scaled_channel<uint16_t, 1000, 1> VBatt = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_OIL_PRESSURE@@
	kPa
	 * offset 52
	 */
	scaled_channel<uint16_t, 30, 1> oilPressure = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1I@@
	deg
	 * offset 54
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_AIR_MASS@@
	g
	 * offset 56
	 */
	scaled_channel<uint16_t, 1000, 1> chargeAirMass = (uint16_t)0;
	/**
	 * crankingFuelMs
	 * airmass in mg, 0-65 grams
	ms
	 * offset 58
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelMs = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_AFR@@
	ratio
	 * offset 60
	 */
	scaled_channel<uint16_t, 1000, 1> currentTargetAfr = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_BASE@@
	 * This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	mg
	 * offset 62
	 */
	scaled_channel<uint16_t, 100, 1> baseFuel = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_RUNNING@@
	 * Total fuel with CLT IAT and TPS acceleration without injector lag corrections per cycle, as pulse per cycle
	mg
	 * offset 64
	 */
	scaled_channel<uint16_t, 100, 1> fuelRunning = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_LAST_INJECTION@@
	 * Actual last injection time - including all compensation and injection mode
	ms
	 * offset 66
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjection = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_INJ_DUTY@@
	%
	 * offset 68
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycle = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 69
	 */
	uint8_t alignmentFill_at_69[3];
	/**
	 * @@GAUGE_NAME_FUEL_VE@@
	ratio
	 * offset 72
	 */
	scaled_channel<uint8_t, 2, 1> veValue = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 73
	 */
	uint8_t alignmentFill_at_73[1];
	/**
	deg
	 * offset 74
	 */
	scaled_channel<int16_t, 1, 1> injectionOffset = (int16_t)0;
	/**
	deg C
	 * offset 76
	 */
	scaled_channel<int16_t, 100, 1> tCharge = (int16_t)0;
	/**
	 * @@GAUGE_NAME_INJECTOR_LAG@@
	ms
	 * offset 78
	 */
	scaled_channel<uint16_t, 300, 1> injectorLagMs = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_IAT_CORR@@
	%
	 * offset 80
	 */
	scaled_channel<uint16_t, 100, 1> iatCorrection = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_CLT_CORR@@
	%
	 * offset 82
	 */
	scaled_channel<uint16_t, 100, 1> cltCorrection = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_BARO_CORR@@
	%
	 * offset 84
	 */
	scaled_channel<uint16_t, 100, 1> baroCorrection = (uint16_t)0;
	/**
	 * "Engine Phase"
	deg
	 * offset 86
	 */
	scaled_channel<uint16_t, 1, 1> currentEnginePhase = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_AMOUNT@@
	mg
	 * offset 88
	 */
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_CORRECTION@@
	mg
	 * offset 90
	 */
	scaled_channel<int16_t, 100, 1> wallFuelCorrection = (int16_t)0;
	/**
	 * offset 92
	 */
	uint16_t revolutionCounterSinceStart = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_TPS_ROC@@
	ratio
	 * offset 94
	 */
	scaled_channel<int16_t, 100, 1> deltaTps = (int16_t)0;
	/**
	 * offset 96
	 */
	uint16_t canReadCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_TPS_EXTRA@@
	ms
	 * offset 98
	 */
	scaled_channel<int16_t, 300, 1> tpsAccelFuel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TIMING_ADVANCE@@
	deg
	 * offset 100
	 */
	scaled_channel<int16_t, 50, 1> ignitionAdvance = (int16_t)0;
	/**
	 * @@GAUGE_COIL_DWELL_TIME@@
	ms
	 * offset 102
	 */
	scaled_channel<uint16_t, 300, 1> sparkDwellValue = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_DWELL_DUTY@@
	%
	 * offset 104
	 */
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_IAC@@
	%
	 * offset 106
	 */
	scaled_channel<uint16_t, 100, 1> idleAirValvePosition = (uint16_t)0;
	/**
	 * "ETB Target"
	%
	 * offset 108
	 */
	scaled_channel<int16_t, 100, 1> etbTarget = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ETB_DUTY@@
	%
	 * offset 110
	 */
	scaled_channel<int16_t, 100, 1> etb1DutyCycle = (int16_t)0;
	/**
	 * "ETB Error"
	%
	 * offset 112
	 */
	scaled_channel<int16_t, 100, 1> etb1Error = (int16_t)0;
	/**
	 * "Fuel level"
	%
	 * offset 114
	 */
	scaled_channel<int16_t, 100, 1> fuelTankLevel = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_CONSUMPTION@@
	grams
	 * offset 116
	 */
	scaled_channel<uint16_t, 1, 1> totalFuelConsumption = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_FLOW@@
	gram/s
	 * offset 118
	 */
	scaled_channel<uint16_t, 200, 1> fuelFlowRate = (uint16_t)0;
	/**
	%
	 * offset 120
	 */
	scaled_channel<uint16_t, 100, 1> veTableYAxis = (uint16_t)0;
	/**
	%
	 * offset 122
	 */
	scaled_channel<uint16_t, 100, 1> afrTableYAxis = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_KNOCK_LEVEL@@
	Volts
	 * offset 124
	 */
	scaled_channel<float, 1, 1> knockLevel = (float)0;
	/**
	 * @@GAUGE_NAME_UPTIME@@
	sec
	 * offset 128
	 */
	scaled_channel<uint32_t, 1, 1> seconds = (uint32_t)0;
	/**
	 * "Engine Mode"
	em
	 * offset 132
	 */
	scaled_channel<uint32_t, 1, 1> engineMode = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_VERSION@@
	version_f
	 * offset 136
	 */
	scaled_channel<uint32_t, 1, 1> firmwareVersion = (uint32_t)0;
	/**
	 * offset 140
	 */
	scaled_channel<uint32_t, 1, 1> tsConfigVersion = (uint32_t)0;
	/**
	 * offset 144
	 */
	scaled_channel<float, 1, 1> calibrationValue = (float)0;
	/**
	 * offset 148
	 */
	scaled_channel<uint8_t, 1, 1> calibrationMode = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 149
	 */
	uint8_t alignmentFill_at_149[3];
	/**
	 * offset 152
	 */
	scaled_channel<uint8_t, 1, 1> idleTargetPosition = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 153
	 */
	uint8_t alignmentFill_at_153[1];
	/**
	 * @@GAUGE_NAME_FUEL_LOAD@@
	%
	 * offset 154
	 */
	scaled_channel<uint16_t, 100, 1> fuelingLoad = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_IGNITION_LOAD@@
	%
	 * offset 156
	 */
	scaled_channel<uint16_t, 100, 1> ignitionLoad = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ENGINE_CRC16@@
	crc16
	 * offset 158
	 */
	scaled_channel<uint16_t, 1, 1> engineMakeCodeNameCrc16 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_TRG_ERR@@
	counter
	 * offset 160
	 */
	scaled_channel<uint32_t, 1, 1> totalTriggerErrorCounter = (uint32_t)0;
	/**
	 * offset 164
	 */
	uint32_t orderingErrorCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_COUNTER@@
	count
	 * offset 168
	 */
	scaled_channel<uint16_t, 1, 1> warningCounter = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_WARNING_LAST@@
	error
	 * offset 170
	 */
	scaled_channel<uint16_t, 1, 1> lastErrorCode = (uint16_t)0;
	/**
	error
	 * offset 172
	 */
	uint16_t recentErrorCode[8];
	/**
	val
	 * offset 188
	 */
	scaled_channel<float, 1, 1> debugFloatField1 = (float)0;
	/**
	val
	 * offset 192
	 */
	scaled_channel<float, 1, 1> debugFloatField2 = (float)0;
	/**
	val
	 * offset 196
	 */
	scaled_channel<float, 1, 1> debugFloatField3 = (float)0;
	/**
	val
	 * offset 200
	 */
	scaled_channel<float, 1, 1> debugFloatField4 = (float)0;
	/**
	val
	 * offset 204
	 */
	scaled_channel<float, 1, 1> debugFloatField5 = (float)0;
	/**
	val
	 * offset 208
	 */
	scaled_channel<float, 1, 1> debugFloatField6 = (float)0;
	/**
	val
	 * offset 212
	 */
	scaled_channel<float, 1, 1> debugFloatField7 = (float)0;
	/**
	val
	 * offset 216
	 */
	scaled_channel<uint32_t, 1, 1> debugIntField1 = (uint32_t)0;
	/**
	val
	 * offset 220
	 */
	scaled_channel<uint32_t, 1, 1> debugIntField2 = (uint32_t)0;
	/**
	val
	 * offset 224
	 */
	scaled_channel<uint32_t, 1, 1> debugIntField3 = (uint32_t)0;
	/**
	val
	 * offset 228
	 */
	scaled_channel<int16_t, 1, 1> debugIntField4 = (int16_t)0;
	/**
	val
	 * offset 230
	 */
	scaled_channel<int16_t, 1, 1> debugIntField5 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_X@@
	G
	 * offset 232
	 */
	scaled_channel<int16_t, 100, 1> accelerationX = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_Y@@
	G
	 * offset 234
	 */
	scaled_channel<int16_t, 100, 1> accelerationY = (int16_t)0;
	/**
	deg C
	 * offset 236
	 */
	uint16_t egt[EGT_CHANNEL_COUNT];
	/**
	 * @@GAUGE_NAME_TPS2@@
	%
	 * offset 252
	 */
	scaled_channel<int16_t, 100, 1> TPS2Value = (int16_t)0;
	/**
	V
	 * offset 254
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Primary = (int16_t)0;
	/**
	V
	 * offset 256
	 */
	scaled_channel<int16_t, 1000, 1> rawPpsPrimary = (int16_t)0;
	/**
	V
	 * offset 258
	 */
	scaled_channel<int16_t, 1000, 1> rawClt = (int16_t)0;
	/**
	V
	 * offset 260
	 */
	scaled_channel<int16_t, 1000, 1> rawIat = (int16_t)0;
	/**
	V
	 * offset 262
	 */
	scaled_channel<int16_t, 1000, 1> rawOilPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TUNE_CRC16@@
	crc16
	 * offset 264
	 */
	scaled_channel<uint16_t, 1, 1> tuneCrc16 = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 266
	 */
	uint8_t alignmentFill_at_266[2];
	/**
	 * offset 268
	 */
	scaled_channel<uint8_t, 1, 1> fuelClosedLoopBinIdx = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 269
	 */
	uint8_t alignmentFill_at_269[3];
	/**
	 * @@GAUGE_NAME_CURRENT_GEAR@@
	gear
	 * offset 272
	 */
	scaled_channel<int8_t, 1, 1> tcuCurrentGear = (int8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 273
	 */
	uint8_t alignmentFill_at_273[1];
	/**
	V
	 * offset 274
	 */
	scaled_channel<int16_t, 1000, 1> rawPpsSecondary = (int16_t)0;
	/**
	dBv
	 * offset 276
	 */
	scaled_channel<int8_t, 1, 1> knock[12];
	/**
	 * @@GAUGE_NAME_DESIRED_GEAR@@
	gear
	 * offset 288
	 */
	scaled_channel<int8_t, 1, 1> tcuDesiredGear = (int8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 289
	 */
	uint8_t alignmentFill_at_289[3];
	/**
	 * @@GAUGE_NAME_FLEX@@
	%
	 * offset 292
	 */
	scaled_channel<uint8_t, 2, 1> flexPercent = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 293
	 */
	uint8_t alignmentFill_at_293[1];
	/**
	V
	 * offset 294
	 */
	scaled_channel<int16_t, 1000, 1> rawIdlePositionSensor = (int16_t)0;
	/**
	V
	 * offset 296
	 */
	scaled_channel<int16_t, 1000, 1> rawWastegatePosition = (int16_t)0;
	/**
	 * @@GAUGE_NAME_WG_POSITION@@
	%
	 * offset 298
	 */
	scaled_channel<int16_t, 100, 1> wastegatePositionSensor = (int16_t)0;
	/**
	 * @@GAUGE_NAME_IDLE_POSITION@@
	%
	 * offset 300
	 */
	scaled_channel<int16_t, 100, 1> idlePositionSensor = (int16_t)0;
	/**
	V
	 * offset 302
	 */
	scaled_channel<int16_t, 1000, 1> rawLowFuelPressure = (int16_t)0;
	/**
	V
	 * offset 304
	 */
	scaled_channel<int16_t, 1000, 1> rawHighFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_LOW@@
	kpa
	 * offset 306
	 */
	scaled_channel<int16_t, 30, 1> lowFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_PRESSURE_HIGH@@
	bar
	 * offset 308
	 */
	scaled_channel<int16_t, 10, 1> highFuelPressure = (int16_t)0;
	/**
	 * @@GAUGE_NAME_TARGET_LAMBDA@@
	 * offset 310
	 */
	scaled_channel<uint16_t, 10000, 1> targetLambda = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR@@
	AFR
	 * offset 312
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue = (uint16_t)0;
	/**
	 * "Vss Accel"
	m/s2
	 * offset 314
	 */
	scaled_channel<uint16_t, 300, 1> VssAcceleration = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_LAMBDA2@@
	 * offset 316
	 */
	scaled_channel<uint16_t, 10000, 1> lambdaValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_AFR2@@
	AFR
	 * offset 318
	 */
	scaled_channel<uint16_t, 1000, 1> AFRValue2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B1E@@
	deg
	 * offset 320
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB1E = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2I@@
	deg
	 * offset 322
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2I = (int16_t)0;
	/**
	 * @@GAUGE_NAME_VVT_B2E@@
	deg
	 * offset 324
	 */
	scaled_channel<int16_t, 50, 1> vvtPositionB2E = (int16_t)0;
	/**
	%
	 * offset 326
	 */
	scaled_channel<int16_t, 100, 1> fuelPidCorrection[2];
	/**
	V
	 * offset 330
	 */
	scaled_channel<int16_t, 1000, 1> rawTps1Secondary = (int16_t)0;
	/**
	V
	 * offset 332
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Primary = (int16_t)0;
	/**
	V
	 * offset 334
	 */
	scaled_channel<int16_t, 1000, 1> rawTps2Secondary = (int16_t)0;
	/**
	 * "knock: count"
	 * offset 336
	 */
	uint16_t knockCount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_Z@@
	G
	 * offset 338
	 */
	scaled_channel<int16_t, 100, 1> accelerationZ = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_ROLL@@
	G
	 * offset 340
	 */
	scaled_channel<int16_t, 100, 1> accelerationRoll = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_YAW@@
	G
	 * offset 342
	 */
	scaled_channel<int16_t, 100, 1> accelerationYaw = (int16_t)0;
	/**
	deg
	 * offset 344
	 */
	scaled_channel<int8_t, 1, 1> vvtTargets[4];
	/**
	 * @@GAUGE_NAME_TURBO_SPEED@@
	hz
	 * offset 348
	 */
	uint16_t turboSpeed = (uint16_t)0;
	/**
	deg C
	 * offset 350
	 */
	uint16_t wbTemperature[2];
	/**
	 * need 4 byte alignment
	units
	 * offset 354
	 */
	uint8_t alignmentFill_at_354[2];
	/**
	%
	 * offset 356
	 */
	uint8_t wbHeaterDuty[2];
	/**
	%
	 * offset 358
	 */
	scaled_channel<int16_t, 100, 1> tps1Split = (int16_t)0;
	/**
	%
	 * offset 360
	 */
	scaled_channel<int16_t, 100, 1> tps2Split = (int16_t)0;
	/**
	%
	 * offset 362
	 */
	scaled_channel<int16_t, 100, 1> tps12Split = (int16_t)0;
	/**
	%
	 * offset 364
	 */
	scaled_channel<int16_t, 100, 1> accPedalSplit = (int16_t)0;
	/**
	 * "DI: Pump Angle"
	deg
	 * offset 366
	 */
	scaled_channel<int16_t, 50, 1> m_requested_pump = (int16_t)0;
	/**
	 * @@GAUGE_NAME_BOOST_TARGET@@
	kPa
	 * offset 368
	 */
	scaled_channel<int16_t, 30, 1> boostControlTarget = (int16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 370
	 */
	uint8_t alignmentFill_at_370[2];
	/**
	 * "Spark Cut Code"
	code
	 * offset 372
	 */
	scaled_channel<int8_t, 1, 1> sparkCutReason = (int8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 373
	 */
	uint8_t alignmentFill_at_373[3];
	/**
	 * "Fuel Cut Code"
	code
	 * offset 376
	 */
	scaled_channel<int8_t, 1, 1> fuelCutReason = (int8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 377
	 */
	uint8_t alignmentFill_at_377[3];
	/**
	 * "DI: fuel_requested_percent"
	v
	 * offset 380
	 */
	float fuel_requested_percent = (float)0;
	/**
	 * "DI: fuel_requested_percent_pi"
	v
	 * offset 384
	 */
	float fuel_requested_percent_pi = (float)0;
	/**
	 * "DI: m_I_sum_percent"
	v
	 * offset 388
	 */
	float m_I_sum_percent = (float)0;
	/**
	 * "DI: m_pressureTarget_kPa"
	v
	 * offset 392
	 */
	float m_pressureTarget_kPa = (float)0;
	/**
	 * @@GAUGE_NAME_AIR_FLOW_ESTIMATE@@
	kg/h
	 * offset 396
	 */
	scaled_channel<uint16_t, 10, 1> mafEstimate = (uint16_t)0;
	/**
	rpm
	 * offset 398
	 */
	uint16_t instantRpm = (uint16_t)0;
	/**
	 * "DI: next start"
	v
	 * offset 400
	 */
	float di_nextStart = (float)0;
	/**
	counter
	 * offset 404
	 */
	uint16_t systemEventReuse = (uint16_t)0;
	/**
	V
	 * offset 406
	 */
	scaled_channel<uint16_t, 1000, 1> rawMap = (uint16_t)0;
	/**
	V
	 * offset 408
	 */
	scaled_channel<uint16_t, 1000, 1> rawAfr = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 410
	 */
	uint8_t alignmentFill_at_410[2];
	/**
	%
	 * offset 412
	 */
	uint8_t tpsAccelFrom = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 413
	 */
	uint8_t alignmentFill_at_413[3];
	/**
	%
	 * offset 416
	 */
	uint8_t tpsAccelTo = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 417
	 */
	uint8_t alignmentFill_at_417[3];
	/**
	 * offset 420
	 */
	scaled_channel<float, 1, 1> calibrationValue2 = (float)0;
	/**
	offset 424 bit 0 */
	bool isMainRelayOn : 1 {};
	/**
	 * Original reason for this is to check if USB is connected from Lua
	offset 424 bit 1 */
	bool isUsbConnected : 1 {};
	/**
	offset 424 bit 2 */
	bool unusedBit_204_2 : 1 {};
	/**
	offset 424 bit 3 */
	bool unusedBit_204_3 : 1 {};
	/**
	offset 424 bit 4 */
	bool unusedBit_204_4 : 1 {};
	/**
	offset 424 bit 5 */
	bool unusedBit_204_5 : 1 {};
	/**
	offset 424 bit 6 */
	bool unusedBit_204_6 : 1 {};
	/**
	offset 424 bit 7 */
	bool unusedBit_204_7 : 1 {};
	/**
	offset 424 bit 8 */
	bool unusedBit_204_8 : 1 {};
	/**
	offset 424 bit 9 */
	bool unusedBit_204_9 : 1 {};
	/**
	offset 424 bit 10 */
	bool unusedBit_204_10 : 1 {};
	/**
	offset 424 bit 11 */
	bool unusedBit_204_11 : 1 {};
	/**
	offset 424 bit 12 */
	bool unusedBit_204_12 : 1 {};
	/**
	offset 424 bit 13 */
	bool unusedBit_204_13 : 1 {};
	/**
	offset 424 bit 14 */
	bool unusedBit_204_14 : 1 {};
	/**
	offset 424 bit 15 */
	bool unusedBit_204_15 : 1 {};
	/**
	offset 424 bit 16 */
	bool unusedBit_204_16 : 1 {};
	/**
	offset 424 bit 17 */
	bool unusedBit_204_17 : 1 {};
	/**
	offset 424 bit 18 */
	bool unusedBit_204_18 : 1 {};
	/**
	offset 424 bit 19 */
	bool unusedBit_204_19 : 1 {};
	/**
	offset 424 bit 20 */
	bool unusedBit_204_20 : 1 {};
	/**
	offset 424 bit 21 */
	bool unusedBit_204_21 : 1 {};
	/**
	offset 424 bit 22 */
	bool unusedBit_204_22 : 1 {};
	/**
	offset 424 bit 23 */
	bool unusedBit_204_23 : 1 {};
	/**
	offset 424 bit 24 */
	bool unusedBit_204_24 : 1 {};
	/**
	offset 424 bit 25 */
	bool unusedBit_204_25 : 1 {};
	/**
	offset 424 bit 26 */
	bool unusedBit_204_26 : 1 {};
	/**
	offset 424 bit 27 */
	bool unusedBit_204_27 : 1 {};
	/**
	offset 424 bit 28 */
	bool unusedBit_204_28 : 1 {};
	/**
	offset 424 bit 29 */
	bool unusedBit_204_29 : 1 {};
	/**
	offset 424 bit 30 */
	bool unusedBit_204_30 : 1 {};
	/**
	offset 424 bit 31 */
	bool unusedBit_204_31 : 1 {};
	/**
	count
	 * offset 428
	 */
	int luaInvocationCounter = (int)0;
	/**
	nt
	 * offset 432
	 */
	int luaLastCycleDuration = (int)0;
	/**
	count
	 * offset 436
	 */
	uint8_t testBenchIter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 437
	 */
	uint8_t alignmentFill_at_437[3];
	/**
	 * offset 440
	 */
	uint8_t unusedWat = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 441
	 */
	uint8_t alignmentFill_at_441[1];
	/**
	 * @@GAUGE_NAME_TC_RATIO@@
	value
	 * offset 442
	 */
	scaled_channel<uint16_t, 100, 1> tcRatio = (uint16_t)0;
	/**
	 * "S"
	 * offset 444
	 */
	float lastShiftTime = (float)0;
	/**
	 * offset 448
	 */
	uint8_t unusedInTheMiddle[8];
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_1@@
	 * offset 456
	 */
	scaled_channel<int16_t, 100, 1> auxLinear1 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_2@@
	 * offset 458
	 */
	scaled_channel<int16_t, 100, 1> auxLinear2 = (int16_t)0;
	/**
	offset 460 bit 0 */
	bool launchSpeedCondition : 1 {};
	/**
	offset 460 bit 1 */
	bool launchRpmCondition : 1 {};
	/**
	offset 460 bit 2 */
	bool launchTpsCondition : 1 {};
	/**
	offset 460 bit 3 */
	bool launchActivateSwitchCondition : 1 {};
	/**
	offset 460 bit 4 */
	bool launchIsLaunchCondition : 1 {};
	/**
	offset 460 bit 5 */
	bool launchCombinedConditions : 1 {};
	/**
	offset 460 bit 6 */
	bool dfcoActive : 1 {};
	/**
	offset 460 bit 7 */
	bool tpsAccelActive : 1 {};
	/**
	offset 460 bit 8 */
	bool unusedBit_253_8 : 1 {};
	/**
	offset 460 bit 9 */
	bool unusedBit_253_9 : 1 {};
	/**
	offset 460 bit 10 */
	bool unusedBit_253_10 : 1 {};
	/**
	offset 460 bit 11 */
	bool unusedBit_253_11 : 1 {};
	/**
	offset 460 bit 12 */
	bool unusedBit_253_12 : 1 {};
	/**
	offset 460 bit 13 */
	bool unusedBit_253_13 : 1 {};
	/**
	offset 460 bit 14 */
	bool unusedBit_253_14 : 1 {};
	/**
	offset 460 bit 15 */
	bool unusedBit_253_15 : 1 {};
	/**
	offset 460 bit 16 */
	bool unusedBit_253_16 : 1 {};
	/**
	offset 460 bit 17 */
	bool unusedBit_253_17 : 1 {};
	/**
	offset 460 bit 18 */
	bool unusedBit_253_18 : 1 {};
	/**
	offset 460 bit 19 */
	bool unusedBit_253_19 : 1 {};
	/**
	offset 460 bit 20 */
	bool unusedBit_253_20 : 1 {};
	/**
	offset 460 bit 21 */
	bool unusedBit_253_21 : 1 {};
	/**
	offset 460 bit 22 */
	bool unusedBit_253_22 : 1 {};
	/**
	offset 460 bit 23 */
	bool unusedBit_253_23 : 1 {};
	/**
	offset 460 bit 24 */
	bool unusedBit_253_24 : 1 {};
	/**
	offset 460 bit 25 */
	bool unusedBit_253_25 : 1 {};
	/**
	offset 460 bit 26 */
	bool unusedBit_253_26 : 1 {};
	/**
	offset 460 bit 27 */
	bool unusedBit_253_27 : 1 {};
	/**
	offset 460 bit 28 */
	bool unusedBit_253_28 : 1 {};
	/**
	offset 460 bit 29 */
	bool unusedBit_253_29 : 1 {};
	/**
	offset 460 bit 30 */
	bool unusedBit_253_30 : 1 {};
	/**
	offset 460 bit 31 */
	bool unusedBit_253_31 : 1 {};
	/**
	 * offset 464
	 */
	uint8_t unused_8_1 = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 465
	 */
	uint8_t alignmentFill_at_465[3];
	/**
	 * offset 468
	 */
	uint8_t unused_8_2 = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 469
	 */
	uint8_t alignmentFill_at_469[3];
	/**
	 * @@GAUGE_NAME_BOOST_OUTPUT@@
	%
	 * offset 472
	 */
	scaled_channel<uint8_t, 2, 1> boostControllerOutput = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 473
	 */
	uint8_t alignmentFill_at_473[3];
	/**
	 * @@GAUGE_NAME_BOOST_OPEN_LOOP@@
	%
	 * offset 476
	 */
	scaled_channel<uint8_t, 2, 1> boostControllerOpenLoopPart = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 477
	 */
	uint8_t alignmentFill_at_477[3];
	/**
	 * offset 480
	 */
	float unused_32_1 = (float)0;
	/**
	 * offset 484
	 */
	float unused_32_2 = (float)0;
	/**
	 * offset 488
	 */
	scaled_channel<float, 1, 1> vvtSyncGapRatio = (float)0;
	/**
	 * offset 492
	 */
	scaled_channel<float, 1, 1> vvtCurrentPosition = (float)0;
	/**
	 * @@GAUGE_NAME_TRG_GAP@@
	 * offset 496
	 */
	scaled_channel<float, 1, 1> triggerSyncGapRatio = (float)0;
	/**
	 * offset 500
	 */
	uint8_t triggerStateIndex = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 501
	 */
	uint8_t alignmentFill_at_501[3];
	/**
	 * offset 504
	 */
	uint8_t vvtCounter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 505
	 */
	uint8_t alignmentFill_at_505[3];
	/**
	 * offset 508
	 */
	uint8_t vvtSyncCounter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 509
	 */
	uint8_t alignmentFill_at_509[3];
	/**
	 * offset 512
	 */
	uint8_t vvtStateIndex = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 513
	 */
	uint8_t alignmentFill_at_513[1];
	/**
	kPa
	 * offset 514
	 */
	scaled_channel<uint16_t, 10, 1> fallbackMap = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_BOOST_CLOSED_LOOP@@
	%
	 * offset 516
	 */
	scaled_channel<int8_t, 2, 1> boostControllerClosedLoopPart = (int8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 517
	 */
	uint8_t alignmentFill_at_517[3];
	/**
	 * @@GAUGE_NAME_DETECTED_GEAR@@
	 * offset 520
	 */
	uint8_t detectedGear = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 521
	 */
	uint8_t alignmentFill_at_521[1];
	/**
	deg
	 * offset 522
	 */
	scaled_channel<int16_t, 100, 1> timingCltCorrection = (int16_t)0;
	/**
	deg
	 * offset 524
	 */
	scaled_channel<int16_t, 100, 1> timingIatCorrection = (int16_t)0;
	/**
	deg
	 * offset 526
	 */
	scaled_channel<int16_t, 100, 1> timingPidCorrection = (int16_t)0;
	/**
	 * Instant MAP
	kPa
	 * offset 528
	 */
	scaled_channel<uint16_t, 30, 1> instantMAPValue = (uint16_t)0;
	/**
	 * offset 530
	 */
	uint16_t mostRecentTimeBetweenSparkEvents = (uint16_t)0;
	/**
	 * offset 532
	 */
	uint16_t mostRecentTimeBetweenIgnitionEvents = (uint16_t)0;
	/**
	 * offset 534
	 */
	uint16_t maxLockedDuration = (uint16_t)0;
	/**
	 * offset 536
	 */
	uint16_t maxTriggerReentrant = (uint16_t)0;
	/**
	 * offset 538
	 */
	uint16_t canWriteOk = (uint16_t)0;
	/**
	 * offset 540
	 */
	uint16_t canWriteNotOk = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 542
	 */
	uint8_t alignmentFill_at_542[2];
	/**
	 * offset 544
	 */
	int triggerPrimaryFall = (int)0;
	/**
	 * offset 548
	 */
	int triggerPrimaryRise = (int)0;
	/**
	 * offset 552
	 */
	int triggerSecondaryFall = (int)0;
	/**
	 * offset 556
	 */
	int triggerSecondaryRise = (int)0;
	/**
	 * offset 560
	 */
	int triggerVvtFall = (int)0;
	/**
	 * offset 564
	 */
	int triggerVvtRise = (int)0;
	/**
	 * offset 568
	 */
	uint8_t startStopStateToggleCounter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 569
	 */
	uint8_t alignmentFill_at_569[3];
	/**
	 * offset 572
	 */
	uint8_t starterState = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 573
	 */
	uint8_t alignmentFill_at_573[3];
	/**
	 * offset 576
	 */
	uint8_t starterRelayDisable = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 577
	 */
	uint8_t alignmentFill_at_577[3];
	/**
	 * offset 580
	 */
	uint8_t multiSparkCounter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 581
	 */
	uint8_t alignmentFill_at_581[3];
	/**
	 * offset 584
	 */
	float injectorFlowPressureRatio = (float)0;
	/**
	kPa
	 * offset 588
	 */
	float injectorFlowPressureDelta = (float)0;
	/**
	 * offset 592
	 */
	float etbIntegralError = (float)0;
	/**
	%
	 * offset 596
	 */
	float etbCurrentTarget = (float)0;
	/**
	 * offset 600
	 */
	pid_status_s alternatorStatus;
	/**
	 * offset 612
	 */
	pid_status_s idleStatus;
	/**
	 * offset 624
	 */
	pid_status_s etbStatus;
	/**
	 * offset 636
	 */
	pid_status_s boostStatus;
	/**
	 * "aux speed 1"
	s
	 * offset 648
	 */
	uint16_t auxSpeed1 = (uint16_t)0;
	/**
	 * "aux speed 2"
	s
	 * offset 650
	 */
	uint16_t auxSpeed2 = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_ISS@@
	RPM
	 * offset 652
	 */
	scaled_channel<uint16_t, 1, 1> ISSValue = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 654
	 */
	uint8_t alignmentFill_at_654[2];
	/**
	 * offset 656
	 */
	uint8_t unusedAtTheEnd[78];
	/**
	 * need 4 byte alignment
	units
	 * offset 734
	 */
	uint8_t alignmentFill_at_734[2];
};
static_assert(sizeof(output_channels_s) == 736);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Mon Apr 25 03:43:32 UTC 2022
