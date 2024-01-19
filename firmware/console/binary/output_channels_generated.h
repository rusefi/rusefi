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
static_assert(offsetof(pid_status_s, pTerm) == 0);
static_assert(offsetof(pid_status_s, iTerm) == 4);
static_assert(offsetof(pid_status_s, dTerm) == 6);
static_assert(offsetof(pid_status_s, output) == 8);
static_assert(offsetof(pid_status_s, error) == 10);
static_assert(offsetof(pid_status_s, resetCounter) == 12);

struct output_channels_s {
	// SD: Present
	// offset 0 bit 0
	bool sd_present : 1 {};
	// SD: Logging
	// offset 0 bit 1
	bool sd_logging_internal : 1 {};
	// offset 0 bit 2
	bool triggerScopeReady : 1 {};
	// offset 0 bit 3
	bool antilagTriggered : 1 {};
	// Radiator Fan
	// offset 0 bit 4
	bool isFanOn : 1 {};
	// offset 0 bit 5
	bool isO2HeaterOn : 1 {};
	// offset 0 bit 6
	bool checkEngine : 1 {};
	// offset 0 bit 7
	bool needBurn : 1 {};
	// SD: MSD
	// offset 0 bit 8
	bool sd_msd : 1 {};
	// Radiator Fan 2
	// offset 0 bit 9
	bool isFan2On : 1 {};
	// Harley ACR: Active
	// offset 0 bit 10
	bool acrActive : 1 {};
	// Tooth Logger Ready
	// offset 0 bit 11
	bool toothLogReady : 1 {};
	// Error: TPS
	// offset 0 bit 12
	bool isTpsError : 1 {};
	// Error: CLT
	// offset 0 bit 13
	bool isCltError : 1 {};
	// Error: MAP
	// offset 0 bit 14
	bool isMapError : 1 {};
	// Error: IAT
	// offset 0 bit 15
	bool isIatError : 1 {};
	// Error: Trigger
	// offset 0 bit 16
	bool isTriggerError : 1 {};
	// Error: Active
	// offset 0 bit 17
	bool hasCriticalError : 1 {};
	// Warning: Active
	// offset 0 bit 18
	bool isWarnNow : 1 {};
	// Error: Pedal
	// offset 0 bit 19
	bool isPedalError : 1 {};
	// Launch Control Triggered
	// offset 0 bit 20
	bool launchTriggered : 1 {};
	// Error: TPS2
	// offset 0 bit 21
	bool isTps2Error : 1 {};
	// Injector Fault
	// offset 0 bit 22
	bool injectorFault : 1 {};
	// Ignition Fault
	// offset 0 bit 23
	bool ignitionFault : 1 {};
	// offset 0 bit 24
	bool isMainRelayOn : 1 {};
	// isUsbConnected
	// Original reason for this is to check if USB is connected from Lua
	// offset 0 bit 25
	bool isUsbConnected : 1 {};
	// offset 0 bit 26
	bool dfcoActive : 1 {};
	// offset 0 bit 27
	bool unusedBit_0_27 : 1 {};
	// offset 0 bit 28
	bool unusedBit_0_28 : 1 {};
	// offset 0 bit 29
	bool unusedBit_0_29 : 1 {};
	// offset 0 bit 30
	bool unusedBit_0_30 : 1 {};
	// offset 0 bit 31
	bool unusedBit_0_31 : 1 {};
	// RPM
	// RPM
	// offset 4
	uint16_t RPMValue = (uint16_t)0;
	// dRPM
	// RPM acceleration
	// offset 6
	int16_t rpmAcceleration = (int16_t)0;
	// Gearbox Ratio
	// value
	// offset 8
	scaled_channel<uint16_t, 100, 1> speedToRpmRatio = (uint16_t)0;
	// Vehicle Speed
	// kph
	// offset 10
	uint8_t vehicleSpeedKph = (uint8_t)0;
	// ECU temperature
	// deg C
	// offset 11
	int8_t internalMcuTemperature = (int8_t)0;
	// CLT
	// deg C
	// offset 12
	scaled_channel<int16_t, 100, 1> coolant = (int16_t)0;
	// IAT
	// deg C
	// offset 14
	scaled_channel<int16_t, 100, 1> intake = (int16_t)0;
	// deg C
	// offset 16
	scaled_channel<int16_t, 100, 1> auxTemp1 = (int16_t)0;
	// deg C
	// offset 18
	scaled_channel<int16_t, 100, 1> auxTemp2 = (int16_t)0;
	// TPS
	// %
	// offset 20
	scaled_channel<int16_t, 100, 1> TPSValue = (int16_t)0;
	// Throttle pedal position
	// %
	// offset 22
	scaled_channel<int16_t, 100, 1> throttlePedalPosition = (int16_t)0;
	// ADC
	// offset 24
	uint16_t tpsADC = (uint16_t)0;
	// V
	// offset 26
	scaled_channel<uint16_t, 1000, 1> rawMaf = (uint16_t)0;
	// MAF
	// kg/h
	// offset 28
	scaled_channel<uint16_t, 10, 1> mafMeasured = (uint16_t)0;
	// MAP
	// kPa
	// offset 30
	scaled_channel<uint16_t, 30, 1> MAPValue = (uint16_t)0;
	// kPa
	// offset 32
	scaled_channel<uint16_t, 30, 1> baroPressure = (uint16_t)0;
	// Lambda
	// offset 34
	scaled_channel<uint16_t, 10000, 1> lambdaValue = (uint16_t)0;
	// VBatt
	// V
	// offset 36
	scaled_channel<uint16_t, 1000, 1> VBatt = (uint16_t)0;
	// Oil Pressure
	// kPa
	// offset 38
	scaled_channel<uint16_t, 30, 1> oilPressure = (uint16_t)0;
	// VVT: bank 1 intake
	// deg
	// offset 40
	scaled_channel<int16_t, 50, 1> vvtPositionB1I = (int16_t)0;
	// Fuel: Last inj pulse width
	// ms
	// offset 42
	scaled_channel<uint16_t, 300, 1> actualLastInjection = (uint16_t)0;
	// Fuel: injector duty cycle
	// %
	// offset 44
	scaled_channel<uint8_t, 2, 1> injectorDutyCycle = (uint8_t)0;
	// Fuel: VE
	// ratio
	// offset 45
	scaled_channel<uint8_t, 2, 1> veValue = (uint8_t)0;
	// Fuel: Injection timing SOI
	// deg
	// offset 46
	int16_t injectionOffset = (int16_t)0;
	// Engine CRC16
	// crc16
	// offset 48
	uint16_t engineMakeCodeNameCrc16 = (uint16_t)0;
	// Fuel: wall amount
	// mg
	// offset 50
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;
	// Fuel: wall correction
	// mg
	// offset 52
	scaled_channel<int16_t, 100, 1> wallFuelCorrectionValue = (int16_t)0;
	// offset 54
	uint16_t revolutionCounterSinceStart = (uint16_t)0;
	// CAN: Rx
	// offset 56
	uint16_t canReadCounter = (uint16_t)0;
	// Fuel: TPS AE add fuel ms
	// ms
	// offset 58
	scaled_channel<int16_t, 300, 1> tpsAccelFuel = (int16_t)0;
	// Ign: Timing Base
	// deg
	// offset 60
	scaled_channel<int16_t, 50, 1> ignitionAdvance = (int16_t)0;
	// Ign: Mode
	// offset 62
	uint8_t currentIgnitionMode = (uint8_t)0;
	// Fuel: Injection mode
	// offset 63
	uint8_t currentInjectionMode = (uint8_t)0;
	// Ign: Coil duty cycle
	// %
	// offset 64
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;
	// ETB Target
	// %
	// offset 66
	scaled_channel<int16_t, 100, 1> etbTarget = (int16_t)0;
	// ETB: Duty
	// %
	// offset 68
	scaled_channel<int16_t, 100, 1> etb1DutyCycle = (int16_t)0;
	// Fuel level
	// %
	// offset 70
	scaled_channel<int16_t, 100, 1> fuelTankLevel = (int16_t)0;
	// Fuel: Total consumed
	// grams
	// offset 72
	uint16_t totalFuelConsumption = (uint16_t)0;
	// Fuel: Flow rate
	// gram/s
	// offset 74
	scaled_channel<uint16_t, 200, 1> fuelFlowRate = (uint16_t)0;
	// TPS2
	// %
	// offset 76
	scaled_channel<int16_t, 100, 1> TPS2Value = (int16_t)0;
	// Tune CRC16
	// crc16
	// offset 78
	uint16_t tuneCrc16 = (uint16_t)0;
	// Uptime
	// sec
	// offset 80
	uint32_t seconds = (uint32_t)0;
	// Engine Mode
	// em
	// offset 84
	uint32_t engineMode = (uint32_t)0;
	// firmware
	// version_f
	// offset 88
	uint32_t firmwareVersion = (uint32_t)0;
	// V
	// offset 92
	scaled_channel<int16_t, 1000, 1> rawWastegatePosition = (int16_t)0;
	// Accel: Lateral
	// G
	// offset 94
	scaled_channel<int16_t, 1000, 1> accelerationLat = (int16_t)0;
	// Accel: Longitudinal
	// G
	// offset 96
	scaled_channel<int16_t, 1000, 1> accelerationLon = (int16_t)0;
	// Detected Gear
	// offset 98
	uint8_t detectedGear = (uint8_t)0;
	// offset 99
	uint8_t maxTriggerReentrant = (uint8_t)0;
	// V
	// offset 100
	scaled_channel<int16_t, 1000, 1> rawLowFuelPressure = (int16_t)0;
	// V
	// offset 102
	scaled_channel<int16_t, 1000, 1> rawHighFuelPressure = (int16_t)0;
	// Fuel pressure (low)
	// kpa
	// offset 104
	scaled_channel<int16_t, 30, 1> lowFuelPressure = (int16_t)0;
	// Fuel pressure (high)
	// bar
	// offset 106
	scaled_channel<int16_t, 10, 1> highFuelPressure = (int16_t)0;
	// V
	// offset 108
	scaled_channel<int16_t, 1000, 1> rawPpsSecondary = (int16_t)0;
	// TCU: Desired Gear
	// gear
	// offset 110
	int8_t tcuDesiredGear = (int8_t)0;
	// Flex Ethanol %
	// %
	// offset 111
	scaled_channel<uint8_t, 2, 1> flexPercent = (uint8_t)0;
	// Wastegate position sensor
	// %
	// offset 112
	scaled_channel<int16_t, 100, 1> wastegatePositionSensor = (int16_t)0;
	// offset 114
	uint8_t alignmentFill_at_114[2];
	// offset 116
	float calibrationValue = (float)0;
	// offset 120
	uint8_t calibrationMode = (uint8_t)0;
	// Idle: Stepper target position
	// offset 121
	uint8_t idleStepperTargetPosition = (uint8_t)0;
	// offset 122
	uint8_t alignmentFill_at_122[2];
	// offset 124
	uint32_t tsConfigVersion = (uint32_t)0;
	// Trigger Error Counter
	// counter
	// offset 128
	uint32_t totalTriggerErrorCounter = (uint32_t)0;
	// offset 132
	uint32_t orderingErrorCounter = (uint32_t)0;
	// Warning: counter
	// count
	// offset 136
	uint16_t warningCounter = (uint16_t)0;
	// Warning: last
	// error
	// offset 138
	uint16_t lastErrorCode = (uint16_t)0;
	// error
	// offset 140
	uint16_t recentErrorCode[8];
	// val
	// offset 156
	float debugFloatField1 = (float)0;
	// val
	// offset 160
	float debugFloatField2 = (float)0;
	// val
	// offset 164
	float debugFloatField3 = (float)0;
	// val
	// offset 168
	float debugFloatField4 = (float)0;
	// val
	// offset 172
	float debugFloatField5 = (float)0;
	// val
	// offset 176
	float debugFloatField6 = (float)0;
	// val
	// offset 180
	float debugFloatField7 = (float)0;
	// val
	// offset 184
	uint32_t debugIntField1 = (uint32_t)0;
	// val
	// offset 188
	uint32_t debugIntField2 = (uint32_t)0;
	// val
	// offset 192
	uint32_t debugIntField3 = (uint32_t)0;
	// val
	// offset 196
	int16_t debugIntField4 = (int16_t)0;
	// val
	// offset 198
	int16_t debugIntField5 = (int16_t)0;
	// EGT
	// deg C
	// offset 200
	uint16_t egt[8];
	// V
	// offset 216
	scaled_channel<int16_t, 1000, 1> rawTps1Primary = (int16_t)0;
	// V
	// offset 218
	scaled_channel<int16_t, 1000, 1> rawPpsPrimary = (int16_t)0;
	// V
	// offset 220
	scaled_channel<int16_t, 1000, 1> rawClt = (int16_t)0;
	// V
	// offset 222
	scaled_channel<int16_t, 1000, 1> rawIat = (int16_t)0;
	// V
	// offset 224
	scaled_channel<int16_t, 1000, 1> rawOilPressure = (int16_t)0;
	// offset 226
	uint8_t fuelClosedLoopBinIdx = (uint8_t)0;
	// Current Gear
	// gear
	// offset 227
	int8_t tcuCurrentGear = (int8_t)0;
	// Air/Fuel Ratio
	// AFR
	// offset 228
	scaled_channel<uint16_t, 1000, 1> AFRValue = (uint16_t)0;
	// Vss Accel
	// m/s2
	// offset 230
	scaled_channel<uint16_t, 300, 1> VssAcceleration = (uint16_t)0;
	// Lambda 2
	// offset 232
	scaled_channel<uint16_t, 10000, 1> lambdaValue2 = (uint16_t)0;
	// Air/Fuel Ratio 2
	// AFR
	// offset 234
	scaled_channel<uint16_t, 1000, 1> AFRValue2 = (uint16_t)0;
	// VVT: bank 1 exhaust
	// deg
	// offset 236
	scaled_channel<int16_t, 50, 1> vvtPositionB1E = (int16_t)0;
	// VVT: bank 2 intake
	// deg
	// offset 238
	scaled_channel<int16_t, 50, 1> vvtPositionB2I = (int16_t)0;
	// VVT: bank 2 exhaust
	// deg
	// offset 240
	scaled_channel<int16_t, 50, 1> vvtPositionB2E = (int16_t)0;
	// Fuel: Trim bank
	// %
	// offset 242
	scaled_channel<int16_t, 100, 1> fuelPidCorrection[2];
	// V
	// offset 246
	scaled_channel<int16_t, 1000, 1> rawTps1Secondary = (int16_t)0;
	// V
	// offset 248
	scaled_channel<int16_t, 1000, 1> rawTps2Primary = (int16_t)0;
	// V
	// offset 250
	scaled_channel<int16_t, 1000, 1> rawTps2Secondary = (int16_t)0;
	// Accel: Vertical
	// G
	// offset 252
	scaled_channel<int16_t, 1000, 1> accelerationVert = (int16_t)0;
	// Gyro: Yaw rate
	// deg/sec
	// offset 254
	scaled_channel<int16_t, 1000, 1> gyroYaw = (int16_t)0;
	// deg
	// offset 256
	int8_t vvtTargets[4];
	// Turbocharger Speed
	// hz
	// offset 260
	uint16_t turboSpeed = (uint16_t)0;
	// Ign: Timing Cyl
	// deg
	// offset 262
	scaled_channel<int16_t, 50, 1> ignitionAdvanceCyl[12];
	// %
	// offset 286
	scaled_channel<int16_t, 100, 1> tps1Split = (int16_t)0;
	// %
	// offset 288
	scaled_channel<int16_t, 100, 1> tps2Split = (int16_t)0;
	// %
	// offset 290
	scaled_channel<int16_t, 100, 1> tps12Split = (int16_t)0;
	// %
	// offset 292
	scaled_channel<int16_t, 100, 1> accPedalSplit = (int16_t)0;
	// Ign: Cut Code
	// code
	// offset 294
	int8_t sparkCutReason = (int8_t)0;
	// Fuel: Cut Code
	// code
	// offset 295
	int8_t fuelCutReason = (int8_t)0;
	// Air: Flow estimate
	// kg/h
	// offset 296
	scaled_channel<uint16_t, 10, 1> mafEstimate = (uint16_t)0;
	// rpm
	// offset 298
	uint16_t instantRpm = (uint16_t)0;
	// V
	// offset 300
	scaled_channel<uint16_t, 1000, 1> rawMap = (uint16_t)0;
	// V
	// offset 302
	scaled_channel<uint16_t, 1000, 1> rawAfr = (uint16_t)0;
	// offset 304
	float calibrationValue2 = (float)0;
	// Lua: Tick counter
	// count
	// offset 308
	uint32_t luaInvocationCounter = (uint32_t)0;
	// Lua: Last tick duration
	// nt
	// offset 312
	uint32_t luaLastCycleDuration = (uint32_t)0;
	// TCU: Current Range
	// offset 316
	uint8_t tcu_currentRange = (uint8_t)0;
	// offset 317
	uint8_t alignmentFill_at_317[1];
	// TCU: Torque Converter Ratio
	// value
	// offset 318
	scaled_channel<uint16_t, 100, 1> tcRatio = (uint16_t)0;
	// offset 320
	float lastShiftTime = (float)0;
	// offset 324
	uint32_t vssEdgeCounter = (uint32_t)0;
	// offset 328
	uint32_t issEdgeCounter = (uint32_t)0;
	// Aux linear #1
	// offset 332
	float auxLinear1 = (float)0;
	// Aux linear #2
	// offset 336
	float auxLinear2 = (float)0;
	// kPa
	// offset 340
	scaled_channel<uint16_t, 10, 1> fallbackMap = (uint16_t)0;
	// Instant MAP
	// kPa
	// offset 342
	scaled_channel<uint16_t, 30, 1> instantMAPValue = (uint16_t)0;
	// us
	// offset 344
	uint16_t maxLockedDuration = (uint16_t)0;
	// CAN: Tx OK
	// offset 346
	uint16_t canWriteOk = (uint16_t)0;
	// CAN: Tx err
	// offset 348
	uint16_t canWriteNotOk = (uint16_t)0;
	// offset 350
	uint8_t alignmentFill_at_350[2];
	// offset 352
	uint32_t triggerPrimaryFall = (uint32_t)0;
	// offset 356
	uint32_t triggerPrimaryRise = (uint32_t)0;
	// offset 360
	uint32_t triggerSecondaryFall = (uint32_t)0;
	// offset 364
	uint32_t triggerSecondaryRise = (uint32_t)0;
	// offset 368
	uint32_t triggerVvtFall = (uint32_t)0;
	// offset 372
	uint32_t triggerVvtRise = (uint32_t)0;
	// offset 376
	uint8_t starterState = (uint8_t)0;
	// offset 377
	uint8_t starterRelayDisable = (uint8_t)0;
	// Ign: Multispark count
	// offset 378
	uint8_t multiSparkCounter = (uint8_t)0;
	// offset 379
	uint8_t extiOverflowCount = (uint8_t)0;
	// offset 380
	pid_status_s alternatorStatus;
	// offset 396
	pid_status_s idleStatus;
	// offset 412
	pid_status_s etbStatus;
	// offset 428
	pid_status_s boostStatus;
	// offset 444
	pid_status_s wastegateDcStatus;
	// offset 460
	pid_status_s vvtStatus[4];
	// Aux speed 1
	// s
	// offset 524
	uint16_t auxSpeed1 = (uint16_t)0;
	// Aux speed 2
	// s
	// offset 526
	uint16_t auxSpeed2 = (uint16_t)0;
	// TCU: Input Shaft Speed
	// RPM
	// offset 528
	uint16_t ISSValue = (uint16_t)0;
	// V
	// offset 530
	scaled_channel<int16_t, 1000, 1> rawAnalogInput[8];
	// GPPWM Output
	// %
	// offset 546
	scaled_channel<uint8_t, 2, 1> gppwmOutput[4];
	// offset 550
	int16_t gppwmXAxis[4];
	// offset 558
	scaled_channel<int16_t, 10, 1> gppwmYAxis[4];
	// V
	// offset 566
	scaled_channel<int16_t, 1000, 1> rawBattery = (int16_t)0;
	// offset 568
	scaled_channel<int16_t, 10, 1> ignBlendParameter[4];
	// %
	// offset 576
	scaled_channel<uint8_t, 2, 1> ignBlendBias[4];
	// deg
	// offset 580
	scaled_channel<int16_t, 100, 1> ignBlendOutput[4];
	// offset 588
	scaled_channel<int16_t, 10, 1> veBlendParameter[4];
	// %
	// offset 596
	scaled_channel<uint8_t, 2, 1> veBlendBias[4];
	// %
	// offset 600
	scaled_channel<int16_t, 100, 1> veBlendOutput[4];
	// offset 608
	scaled_channel<int16_t, 10, 1> boostOpenLoopBlendParameter[2];
	// %
	// offset 612
	scaled_channel<uint8_t, 2, 1> boostOpenLoopBlendBias[2];
	// %
	// offset 614
	int8_t boostOpenLoopBlendOutput[2];
	// offset 616
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendParameter[2];
	// %
	// offset 620
	scaled_channel<uint8_t, 2, 1> boostClosedLoopBlendBias[2];
	// %
	// offset 622
	scaled_channel<int16_t, 10, 1> boostClosedLoopBlendOutput[2];
	// offset 626
	uint8_t alignmentFill_at_626[2];
	// offset 628 bit 0
	bool coilState1 : 1 {};
	// offset 628 bit 1
	bool coilState2 : 1 {};
	// offset 628 bit 2
	bool coilState3 : 1 {};
	// offset 628 bit 3
	bool coilState4 : 1 {};
	// offset 628 bit 4
	bool coilState5 : 1 {};
	// offset 628 bit 5
	bool coilState6 : 1 {};
	// offset 628 bit 6
	bool coilState7 : 1 {};
	// offset 628 bit 7
	bool coilState8 : 1 {};
	// offset 628 bit 8
	bool coilState9 : 1 {};
	// offset 628 bit 9
	bool coilState10 : 1 {};
	// offset 628 bit 10
	bool coilState11 : 1 {};
	// offset 628 bit 11
	bool coilState12 : 1 {};
	// offset 628 bit 12
	bool injectorState1 : 1 {};
	// offset 628 bit 13
	bool injectorState2 : 1 {};
	// offset 628 bit 14
	bool injectorState3 : 1 {};
	// offset 628 bit 15
	bool injectorState4 : 1 {};
	// offset 628 bit 16
	bool injectorState5 : 1 {};
	// offset 628 bit 17
	bool injectorState6 : 1 {};
	// offset 628 bit 18
	bool injectorState7 : 1 {};
	// offset 628 bit 19
	bool injectorState8 : 1 {};
	// offset 628 bit 20
	bool injectorState9 : 1 {};
	// offset 628 bit 21
	bool injectorState10 : 1 {};
	// offset 628 bit 22
	bool injectorState11 : 1 {};
	// offset 628 bit 23
	bool injectorState12 : 1 {};
	// offset 628 bit 24
	bool unusedBit_628_24 : 1 {};
	// offset 628 bit 25
	bool unusedBit_628_25 : 1 {};
	// offset 628 bit 26
	bool unusedBit_628_26 : 1 {};
	// offset 628 bit 27
	bool unusedBit_628_27 : 1 {};
	// offset 628 bit 28
	bool unusedBit_628_28 : 1 {};
	// offset 628 bit 29
	bool unusedBit_628_29 : 1 {};
	// offset 628 bit 30
	bool unusedBit_628_30 : 1 {};
	// offset 628 bit 31
	bool unusedBit_628_31 : 1 {};
	// offset 632
	uint32_t outputRequestPeriod = (uint32_t)0;
	// offset 636
	float mapFast = (float)0;
	// Lua: Gauge
	// value
	// offset 640
	float luaGauges[2];
	// V
	// offset 648
	scaled_channel<uint16_t, 1000, 1> rawMaf2 = (uint16_t)0;
	// MAF #2
	// kg/h
	// offset 650
	scaled_channel<uint16_t, 10, 1> mafMeasured2 = (uint16_t)0;
	// offset 652
	uint16_t schedulingUsedCount = (uint16_t)0;
	// %
	// offset 654
	scaled_channel<uint16_t, 100, 1> Gego = (uint16_t)0;
	// count
	// offset 656
	uint16_t testBenchIter = (uint16_t)0;
	// deg C
	// offset 658
	scaled_channel<int16_t, 100, 1> oilTemp = (int16_t)0;
	// deg C
	// offset 660
	scaled_channel<int16_t, 100, 1> fuelTemp = (int16_t)0;
	// deg C
	// offset 662
	scaled_channel<int16_t, 100, 1> ambientTemp = (int16_t)0;
	// deg C
	// offset 664
	scaled_channel<int16_t, 100, 1> compressorDischargeTemp = (int16_t)0;
	// kPa
	// offset 666
	scaled_channel<uint16_t, 30, 1> compressorDischargePressure = (uint16_t)0;
	// kPa
	// offset 668
	scaled_channel<uint16_t, 30, 1> throttleInletPressure = (uint16_t)0;
	// sec
	// offset 670
	uint16_t ignitionOnTime = (uint16_t)0;
	// sec
	// offset 672
	uint16_t engineRunTime = (uint16_t)0;
	// km
	// offset 674
	scaled_channel<uint16_t, 10, 1> distanceTraveled = (uint16_t)0;
	// Air/Fuel Ratio (Gas Scale)
	// AFR
	// offset 676
	scaled_channel<uint16_t, 1000, 1> afrGasolineScale = (uint16_t)0;
	// Air/Fuel Ratio 2 (Gas Scale)
	// AFR
	// offset 678
	scaled_channel<uint16_t, 1000, 1> afr2GasolineScale = (uint16_t)0;
	// Fuel: Last inj pulse width stg 2
	// ms
	// offset 680
	scaled_channel<uint16_t, 300, 1> actualLastInjectionStage2 = (uint16_t)0;
	// Fuel: injector duty cycle stage 2
	// %
	// offset 682
	scaled_channel<uint8_t, 2, 1> injectorDutyCycleStage2 = (uint8_t)0;
	// offset 683
	uint8_t unusedAtTheEnd[117];
};
static_assert(sizeof(output_channels_s) == 800);
static_assert(offsetof(output_channels_s, RPMValue) == 4);
static_assert(offsetof(output_channels_s, rpmAcceleration) == 6);
static_assert(offsetof(output_channels_s, speedToRpmRatio) == 8);
static_assert(offsetof(output_channels_s, vehicleSpeedKph) == 10);
static_assert(offsetof(output_channels_s, internalMcuTemperature) == 11);
static_assert(offsetof(output_channels_s, coolant) == 12);
static_assert(offsetof(output_channels_s, intake) == 14);
static_assert(offsetof(output_channels_s, auxTemp1) == 16);
static_assert(offsetof(output_channels_s, auxTemp2) == 18);
static_assert(offsetof(output_channels_s, TPSValue) == 20);
static_assert(offsetof(output_channels_s, throttlePedalPosition) == 22);
static_assert(offsetof(output_channels_s, tpsADC) == 24);
static_assert(offsetof(output_channels_s, rawMaf) == 26);
static_assert(offsetof(output_channels_s, mafMeasured) == 28);
static_assert(offsetof(output_channels_s, MAPValue) == 30);
static_assert(offsetof(output_channels_s, baroPressure) == 32);
static_assert(offsetof(output_channels_s, lambdaValue) == 34);
static_assert(offsetof(output_channels_s, VBatt) == 36);
static_assert(offsetof(output_channels_s, oilPressure) == 38);
static_assert(offsetof(output_channels_s, vvtPositionB1I) == 40);
static_assert(offsetof(output_channels_s, actualLastInjection) == 42);
static_assert(offsetof(output_channels_s, injectorDutyCycle) == 44);
static_assert(offsetof(output_channels_s, veValue) == 45);
static_assert(offsetof(output_channels_s, injectionOffset) == 46);
static_assert(offsetof(output_channels_s, engineMakeCodeNameCrc16) == 48);
static_assert(offsetof(output_channels_s, wallFuelAmount) == 50);
static_assert(offsetof(output_channels_s, wallFuelCorrectionValue) == 52);
static_assert(offsetof(output_channels_s, revolutionCounterSinceStart) == 54);
static_assert(offsetof(output_channels_s, canReadCounter) == 56);
static_assert(offsetof(output_channels_s, tpsAccelFuel) == 58);
static_assert(offsetof(output_channels_s, ignitionAdvance) == 60);
static_assert(offsetof(output_channels_s, currentIgnitionMode) == 62);
static_assert(offsetof(output_channels_s, currentInjectionMode) == 63);
static_assert(offsetof(output_channels_s, coilDutyCycle) == 64);
static_assert(offsetof(output_channels_s, etbTarget) == 66);
static_assert(offsetof(output_channels_s, etb1DutyCycle) == 68);
static_assert(offsetof(output_channels_s, fuelTankLevel) == 70);
static_assert(offsetof(output_channels_s, totalFuelConsumption) == 72);
static_assert(offsetof(output_channels_s, fuelFlowRate) == 74);
static_assert(offsetof(output_channels_s, TPS2Value) == 76);
static_assert(offsetof(output_channels_s, tuneCrc16) == 78);
static_assert(offsetof(output_channels_s, seconds) == 80);
static_assert(offsetof(output_channels_s, engineMode) == 84);
static_assert(offsetof(output_channels_s, firmwareVersion) == 88);
static_assert(offsetof(output_channels_s, rawWastegatePosition) == 92);
static_assert(offsetof(output_channels_s, accelerationLat) == 94);
static_assert(offsetof(output_channels_s, accelerationLon) == 96);
static_assert(offsetof(output_channels_s, detectedGear) == 98);
static_assert(offsetof(output_channels_s, maxTriggerReentrant) == 99);
static_assert(offsetof(output_channels_s, rawLowFuelPressure) == 100);
static_assert(offsetof(output_channels_s, rawHighFuelPressure) == 102);
static_assert(offsetof(output_channels_s, lowFuelPressure) == 104);
static_assert(offsetof(output_channels_s, highFuelPressure) == 106);
static_assert(offsetof(output_channels_s, rawPpsSecondary) == 108);
static_assert(offsetof(output_channels_s, tcuDesiredGear) == 110);
static_assert(offsetof(output_channels_s, flexPercent) == 111);
static_assert(offsetof(output_channels_s, wastegatePositionSensor) == 112);
static_assert(offsetof(output_channels_s, calibrationValue) == 116);
static_assert(offsetof(output_channels_s, calibrationMode) == 120);
static_assert(offsetof(output_channels_s, idleStepperTargetPosition) == 121);
static_assert(offsetof(output_channels_s, tsConfigVersion) == 124);
static_assert(offsetof(output_channels_s, totalTriggerErrorCounter) == 128);
static_assert(offsetof(output_channels_s, orderingErrorCounter) == 132);
static_assert(offsetof(output_channels_s, warningCounter) == 136);
static_assert(offsetof(output_channels_s, lastErrorCode) == 138);
static_assert(offsetof(output_channels_s, recentErrorCode) == 140);
static_assert(offsetof(output_channels_s, debugFloatField1) == 156);
static_assert(offsetof(output_channels_s, debugFloatField2) == 160);
static_assert(offsetof(output_channels_s, debugFloatField3) == 164);
static_assert(offsetof(output_channels_s, debugFloatField4) == 168);
static_assert(offsetof(output_channels_s, debugFloatField5) == 172);
static_assert(offsetof(output_channels_s, debugFloatField6) == 176);
static_assert(offsetof(output_channels_s, debugFloatField7) == 180);
static_assert(offsetof(output_channels_s, debugIntField1) == 184);
static_assert(offsetof(output_channels_s, debugIntField2) == 188);
static_assert(offsetof(output_channels_s, debugIntField3) == 192);
static_assert(offsetof(output_channels_s, debugIntField4) == 196);
static_assert(offsetof(output_channels_s, debugIntField5) == 198);
static_assert(offsetof(output_channels_s, egt) == 200);
static_assert(offsetof(output_channels_s, rawTps1Primary) == 216);
static_assert(offsetof(output_channels_s, rawPpsPrimary) == 218);
static_assert(offsetof(output_channels_s, rawClt) == 220);
static_assert(offsetof(output_channels_s, rawIat) == 222);
static_assert(offsetof(output_channels_s, rawOilPressure) == 224);
static_assert(offsetof(output_channels_s, fuelClosedLoopBinIdx) == 226);
static_assert(offsetof(output_channels_s, tcuCurrentGear) == 227);
static_assert(offsetof(output_channels_s, AFRValue) == 228);
static_assert(offsetof(output_channels_s, VssAcceleration) == 230);
static_assert(offsetof(output_channels_s, lambdaValue2) == 232);
static_assert(offsetof(output_channels_s, AFRValue2) == 234);
static_assert(offsetof(output_channels_s, vvtPositionB1E) == 236);
static_assert(offsetof(output_channels_s, vvtPositionB2I) == 238);
static_assert(offsetof(output_channels_s, vvtPositionB2E) == 240);
static_assert(offsetof(output_channels_s, fuelPidCorrection) == 242);
static_assert(offsetof(output_channels_s, rawTps1Secondary) == 246);
static_assert(offsetof(output_channels_s, rawTps2Primary) == 248);
static_assert(offsetof(output_channels_s, rawTps2Secondary) == 250);
static_assert(offsetof(output_channels_s, accelerationVert) == 252);
static_assert(offsetof(output_channels_s, gyroYaw) == 254);
static_assert(offsetof(output_channels_s, vvtTargets) == 256);
static_assert(offsetof(output_channels_s, turboSpeed) == 260);
static_assert(offsetof(output_channels_s, ignitionAdvanceCyl) == 262);
static_assert(offsetof(output_channels_s, tps1Split) == 286);
static_assert(offsetof(output_channels_s, tps2Split) == 288);
static_assert(offsetof(output_channels_s, tps12Split) == 290);
static_assert(offsetof(output_channels_s, accPedalSplit) == 292);
static_assert(offsetof(output_channels_s, sparkCutReason) == 294);
static_assert(offsetof(output_channels_s, fuelCutReason) == 295);
static_assert(offsetof(output_channels_s, mafEstimate) == 296);
static_assert(offsetof(output_channels_s, instantRpm) == 298);
static_assert(offsetof(output_channels_s, rawMap) == 300);
static_assert(offsetof(output_channels_s, rawAfr) == 302);
static_assert(offsetof(output_channels_s, calibrationValue2) == 304);
static_assert(offsetof(output_channels_s, luaInvocationCounter) == 308);
static_assert(offsetof(output_channels_s, luaLastCycleDuration) == 312);
static_assert(offsetof(output_channels_s, tcu_currentRange) == 316);
static_assert(offsetof(output_channels_s, tcRatio) == 318);
static_assert(offsetof(output_channels_s, lastShiftTime) == 320);
static_assert(offsetof(output_channels_s, vssEdgeCounter) == 324);
static_assert(offsetof(output_channels_s, issEdgeCounter) == 328);
static_assert(offsetof(output_channels_s, auxLinear1) == 332);
static_assert(offsetof(output_channels_s, auxLinear2) == 336);
static_assert(offsetof(output_channels_s, fallbackMap) == 340);
static_assert(offsetof(output_channels_s, instantMAPValue) == 342);
static_assert(offsetof(output_channels_s, maxLockedDuration) == 344);
static_assert(offsetof(output_channels_s, canWriteOk) == 346);
static_assert(offsetof(output_channels_s, canWriteNotOk) == 348);
static_assert(offsetof(output_channels_s, triggerPrimaryFall) == 352);
static_assert(offsetof(output_channels_s, triggerPrimaryRise) == 356);
static_assert(offsetof(output_channels_s, triggerSecondaryFall) == 360);
static_assert(offsetof(output_channels_s, triggerSecondaryRise) == 364);
static_assert(offsetof(output_channels_s, triggerVvtFall) == 368);
static_assert(offsetof(output_channels_s, triggerVvtRise) == 372);
static_assert(offsetof(output_channels_s, starterState) == 376);
static_assert(offsetof(output_channels_s, starterRelayDisable) == 377);
static_assert(offsetof(output_channels_s, multiSparkCounter) == 378);
static_assert(offsetof(output_channels_s, extiOverflowCount) == 379);
static_assert(offsetof(output_channels_s, auxSpeed1) == 524);
static_assert(offsetof(output_channels_s, auxSpeed2) == 526);
static_assert(offsetof(output_channels_s, ISSValue) == 528);
static_assert(offsetof(output_channels_s, rawAnalogInput) == 530);
static_assert(offsetof(output_channels_s, gppwmOutput) == 546);
static_assert(offsetof(output_channels_s, gppwmXAxis) == 550);
static_assert(offsetof(output_channels_s, gppwmYAxis) == 558);
static_assert(offsetof(output_channels_s, rawBattery) == 566);
static_assert(offsetof(output_channels_s, ignBlendParameter) == 568);
static_assert(offsetof(output_channels_s, ignBlendBias) == 576);
static_assert(offsetof(output_channels_s, ignBlendOutput) == 580);
static_assert(offsetof(output_channels_s, veBlendParameter) == 588);
static_assert(offsetof(output_channels_s, veBlendBias) == 596);
static_assert(offsetof(output_channels_s, veBlendOutput) == 600);
static_assert(offsetof(output_channels_s, boostOpenLoopBlendParameter) == 608);
static_assert(offsetof(output_channels_s, boostOpenLoopBlendBias) == 612);
static_assert(offsetof(output_channels_s, boostOpenLoopBlendOutput) == 614);
static_assert(offsetof(output_channels_s, boostClosedLoopBlendParameter) == 616);
static_assert(offsetof(output_channels_s, boostClosedLoopBlendBias) == 620);
static_assert(offsetof(output_channels_s, boostClosedLoopBlendOutput) == 622);
static_assert(offsetof(output_channels_s, outputRequestPeriod) == 632);
static_assert(offsetof(output_channels_s, mapFast) == 636);
static_assert(offsetof(output_channels_s, luaGauges) == 640);
static_assert(offsetof(output_channels_s, rawMaf2) == 648);
static_assert(offsetof(output_channels_s, mafMeasured2) == 650);
static_assert(offsetof(output_channels_s, schedulingUsedCount) == 652);
static_assert(offsetof(output_channels_s, Gego) == 654);
static_assert(offsetof(output_channels_s, testBenchIter) == 656);
static_assert(offsetof(output_channels_s, oilTemp) == 658);
static_assert(offsetof(output_channels_s, fuelTemp) == 660);
static_assert(offsetof(output_channels_s, ambientTemp) == 662);
static_assert(offsetof(output_channels_s, compressorDischargeTemp) == 664);
static_assert(offsetof(output_channels_s, compressorDischargePressure) == 666);
static_assert(offsetof(output_channels_s, throttleInletPressure) == 668);
static_assert(offsetof(output_channels_s, ignitionOnTime) == 670);
static_assert(offsetof(output_channels_s, engineRunTime) == 672);
static_assert(offsetof(output_channels_s, distanceTraveled) == 674);
static_assert(offsetof(output_channels_s, afrGasolineScale) == 676);
static_assert(offsetof(output_channels_s, afr2GasolineScale) == 678);
static_assert(offsetof(output_channels_s, actualLastInjectionStage2) == 680);
static_assert(offsetof(output_channels_s, injectorDutyCycleStage2) == 682);
static_assert(offsetof(output_channels_s, unusedAtTheEnd) == 683);

