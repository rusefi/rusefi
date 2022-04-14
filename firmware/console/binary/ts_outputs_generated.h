// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Thu Apr 14 15:30:56 EDT 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
#define accelerationRoll_offset 310
#define accelerationX_offset 212
#define accelerationY_offset 214
#define accelerationYaw_offset 312
#define accelerationZ_offset 308
#define accPedalSplit_offset 332
#define acState_offset 0
#define acSwitchState_offset 0
#define actualLastInjection_offset 54
#define adc_channel_e_enum "Disabled", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5"
#define ADC_CHANNEL_NONE 0
#define afrTableYAxis_offset 106
#define AFRValue2_offset 288
#define AFRValue_offset 282
#define AFTERSTART_DECAY_CURVE_SIZE 8
#define AFTERSTART_ENRICH_CURVE_SIZE 8
#define AFTERSTART_HOLD_CURVE_SIZE 8
#define alternatorOnOff_offset 0
#define alternatorStatus_dTerm_offset 518
#define alternatorStatus_error_offset 522
#define alternatorStatus_iTerm_offset 516
#define alternatorStatus_offset 516
#define alternatorStatus_output_offset 520
#define alternatorStatus_resetCounter_offset 524
#define antiLagActivationMode_e_enum "Always Active", "Switch Input"
#define AUX_ANALOG_INPUT_COUNT 8
#define AUX_DIGITAL_VALVE_COUNT 2
#define auxLinear1_offset 404
#define auxLinear2_offset 406
#define auxSpeed1_offset 564
#define auxSpeed2_offset 566
#define auxTemp1_offset 16
#define auxTemp2_offset 18
#define BANKS_COUNT 2
#define BARO_CORR_SIZE 4
#define baroCorrection_offset 68
#define baroPressure_offset 32
#define baseFuel_offset 50
#define BLOCKING_FACTOR 256
#define BOOST_LOAD_COUNT 8
#define BOOST_RPM_COUNT 8
#define boostControllerClosedLoopPart_offset 442
#define boostControllerOpenLoopPart_offset 415
#define boostControllerOutput_offset 414
#define boostControlTarget_offset 336
#define boostStatus_dTerm_offset 554
#define boostStatus_error_offset 558
#define boostStatus_iTerm_offset 552
#define boostStatus_offset 552
#define boostStatus_output_offset 556
#define boostStatus_resetCounter_offset 560
#define brain_input_pin_e_enum "NONE", "INVALID", "INVALID", "PA1", "PA2", "PA3", "INVALID", "PA5", "PA6", "PA7", "PA8", "PA9", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PA15", "INVALID", "INVALID", "INVALID", "PB3", "PB4", "PB5", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PC6", "PC7", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PE5", "PE6", "INVALID", "INVALID", "PE9", "INVALID", "PE11", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
#define brain_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
#define brakePedalState_offset 0
#define calibrationMode_offset 132
#define calibrationValue2_offset 372
#define calibrationValue_offset 128
#define CAM_INPUTS_COUNT 4
#define CAM_INPUTS_COUNT_padding 0
#define CAMS_PER_BANK 2
#define CAMS_PER_BANK_padding 0
#define can_baudrate_e_enum "100kbps", "250kbps", "500kbps", "1Mbps"
#define CAN_DEFAULT_BASE 0x200
#define CAN_ECU_SERIAL_RX_ID 0x100
#define CAN_ECU_SERIAL_TX_ID 0x102
#define CAN_RX_PREFIX "CAN_rx"
#define canReadCounter_offset 80
#define canWriteNotOk_offset 462
#define canWriteOk_offset 460
#define chargeAirMass_offset 44
#define checkEngine_offset 0
#define CLT_CRANKING_CURVE_SIZE 8
#define CLT_CURVE_SIZE 16
#define CLT_LIMITER_CURVE_SIZE 4
#define CLT_TIMING_CURVE_SIZE 8
#define cltCorrection_offset 66
#define clutchDownState_offset 0
#define clutchUpState_offset 0
#define CMD_AC_RELAY_BENCH "acrelaybench"
#define CMD_ALTERNATOR_PIN "set_alternator_pin"
#define CMD_BOOST_PIN "set_boost_pin"
#define CMD_BURNCONFIG "burnconfig"
#define CMD_DATE "date"
#define CMD_DISABLE "disable"
#define CMD_ENABLE "enable"
#define CMD_ENGINE_TYPE "engine_type"
#define CMD_ENGINESNIFFERRPMTHRESHOLD "engineSnifferRpmThreshold"
#define CMD_ETB_DUTY "set_etb_duty"
#define CMD_EXTERNAL_STIMULATION "ext_stimulation"
#define CMD_FAN2_BENCH "fan2bench"
#define CMD_FAN_BENCH "fanbench"
#define CMD_FUEL_BENCH "fuelbench"
#define CMD_FUNCTIONAL_TEST_MODE "test_mode"
#define CMD_GET "get"
#define CMD_HPFP_BENCH "hpfpbench"
#define CMD_IDLE_PIN "set_idle_pin"
#define CMD_IGNITION "ignition"
#define CMD_IGNITION_PIN "set_ignition_pin"
#define CMD_INJECTION "injection"
#define CMD_INJECTION_PIN "set_injection_pin"
#define CMD_LOGIC_PIN "set_logic_input_pin"
#define CMD_MIL_BENCH "milbench"
#define CMD_PINS "pins"
#define CMD_PWM "pwm"
#define CMD_REBOOT "reboot"
#define CMD_REBOOT_DFU "reboot_dfu"
#define CMD_RESET_ENGINE_SNIFFER "reset_engine_chart"
#define CMD_RPM "rpm"
#define CMD_SELF_STIMULATION "self_stimulation"
#define CMD_SET "set"
#define CMD_SPARK_BENCH "sparkbench"
#define CMD_STARTER_BENCH "starterbench"
#define CMD_TRIGGER_HW_INPUT "trigger_hw_input"
#define CMD_TRIGGER_PIN "set_trigger_input_pin"
#define CMD_TRIGGER_SIMULATOR_PIN "set_trigger_simulator_pin"
#define CMD_TRIGGERINFO "triggerinfo"
#define CMD_TS_BENCH_CATEGORY 22
#define CMD_TS_FSIO_CATEGORY 33
#define CMD_TS_SOLENOID_CATEGORY 41
#define CMD_TS_X14 20
#define CMD_TS_X31 49
#define CMD_VSS_PIN "vss_pin"
#define CMD_WRITECONFIG "writeconfig"
#define coilDutyCycle_offset 88
#define COMPOSITE_DATA_LENGTH 2500
#define COMPOSITE_DATA_LENGTH_HALF 1250
#define COMPOSITE_PACKET_COUNT 500
#define COMPOSITE_PACKET_SIZE 5
#define CONSOLE_DATA_PROTOCOL_TAG " @"
#define coolant_offset 12
#define CRANKING_ADVANCE_CURVE_SIZE 4
#define CRANKING_CLT_IDLE_CURVE_SIZE 8
#define CRANKING_CURVE_SIZE 8
#define crankingFuelMs_offset 46
#define CRITICAL_PREFIX "CRITICAL"
#define currentEnginePhase_offset 70
#define currentTargetAfr_offset 48
#define DC_PER_STEPPER 2
#define debug_mode_e_enum "INVALID", "TPS acceleration enrichment", "GPPWM", "Idle Control", "Engine Load accl enrich", "Trigger Counters", "Soft Spark Cut", "VVT1 PID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "SD card", "sr5", "Knock", "INVALID", "Electronic Throttle", "Executor", "Bench Test / TS commands", "INVALID", "Analog inputs #1", "INSTANT_RPM", "INVALID", "Status", "CJ125", "INVALID", "MAP", "Metrics", "INVALID", "Ion Sense", "TLE8888", "Analog inputs #2", "Dwell Metric", "INVALID", "INVALID", "Boost Control", "INVALID", "INVALID", "ETB Autotune", "Composite Log", "INVALID", "INVALID", "INVALID", "Dyno_View", "Logic_Analyzer", "rusEFI Wideband", "TCU", "Lua", "VVT2 PID", "VVT3 PID", "VVT4 PID", "mode 52", "mode 53"
#define debugFloatField1_offset 168
#define debugFloatField2_offset 172
#define debugFloatField3_offset 176
#define debugFloatField4_offset 180
#define debugFloatField5_offset 184
#define debugFloatField6_offset 188
#define debugFloatField7_offset 192
#define debugIntField1_offset 196
#define debugIntField2_offset 200
#define debugIntField3_offset 204
#define debugIntField4_offset 208
#define debugIntField5_offset 210
#define deltaTps_offset 78
#define detectedGear_offset 443
#define dfcoActive_offset 408
#define di_nextStart_offset 360
#define DIGIPOT_COUNT 4
#define DWELL_CURVE_SIZE 8
#define ego_sensor_e_enum "BPSX", "Innovate", "14Point7", "Narrow", "PLX", "Custom", "INVALID", "INVALID"
#define egt1_offset 216
#define egt2_offset 218
#define egt3_offset 220
#define egt4_offset 222
#define egt5_offset 224
#define egt6_offset 226
#define egt7_offset 228
#define egt8_offset 230
#define EGT_CHANNEL_COUNT 8
#define engine_load_mode_e_enum "INVALID", "INVALID", "INVALID", "Speed Density", "MAF Air Charge", "Alpha-N", "Lua"
#define ENGINE_MAKE_BMW "BMW"
#define ENGINE_MAKE_GM "GM"
#define ENGINE_MAKE_HONDA "Honda"
#define ENGINE_MAKE_Hyundai "Hyundai"
#define ENGINE_MAKE_LADA "Lada"
#define ENGINE_MAKE_MAZDA "Mazda"
#define ENGINE_MAKE_MERCEDES "Mercedes"
#define ENGINE_MAKE_NISSAN "Nissan"
#define ENGINE_MAKE_SUBARU "Subaru"
#define ENGINE_MAKE_TOYOTA "Toyota"
#define ENGINE_MAKE_VAG "VAG"
#define ENGINE_NOISE_CURVE_SIZE 8
#define ENGINE_SNIFFER_UNIT_US 10
#define engineMakeCodeNameCrc16_offset 138
#define engineMode_offset 116
#define ENUM_16_BITS 60000
#define ENUM_32_BITS 2000000000
#define ERROR_BUFFER_SIZE 120
#define etb1DutyCycle_offset 94
#define etb1Error_offset 96
#define ETB_BIAS_CURVE_LENGTH 8
#define ETB_COUNT 2
#define ETB_HW_MAX_FREQUENCY 3000
#define etbCurrentTarget_offset 512
#define etbFeedForward_offset 504
#define etbIntegralError_offset 508
#define etbStatus_dTerm_offset 542
#define etbStatus_error_offset 546
#define etbStatus_iTerm_offset 540
#define etbStatus_offset 540
#define etbStatus_output_offset 544
#define etbStatus_resetCounter_offset 548
#define etbTarget_offset 92
#define fallbackMap_offset 440
#define firmwareVersion_offset 120
#define FLASH_DATA_VERSION 10009
#define flexPercent_offset 263
#define FUEL_LEVEL_TABLE_COUNT 8
#define FUEL_LOAD_COUNT 16
#define fuel_requested_percent_offset 340
#define fuel_requested_percent_pi_offset 344
#define FUEL_RPM_COUNT 16
#define fuelClosedLoopBinIdx_offset 246
#define fuelCutReason_offset 339
#define fuelFlowRate_offset 102
#define fuelingLoad_offset 134
#define fuelPidCorrection1_offset 296
#define fuelPidCorrection2_offset 298
#define fuelRunning_offset 52
#define fuelTankLevel_offset 98
#define GAP_TRACKING_LENGTH 18
#define GAUGE_COIL_DWELL_TIME "dwell: coil charge time"
#define GAUGE_NAME_ACCEL_ROLL "Acceleration: Roll"
#define GAUGE_NAME_ACCEL_X "Acceleration: X"
#define GAUGE_NAME_ACCEL_Y "Acceleration: Y"
#define GAUGE_NAME_ACCEL_YAW "Acceleration: Yaw"
#define GAUGE_NAME_ACCEL_Z "Acceleration: Z"
#define GAUGE_NAME_AFR "Air/Fuel Ratio"
#define GAUGE_NAME_AFR2 "Air/Fuel Ratio 2"
#define GAUGE_NAME_AIR_FLOW_ESTIMATE "MAF estimate"
#define GAUGE_NAME_AIR_FLOW_MEASURED "MAF sensor"
#define GAUGE_NAME_AIR_MASS "air mass"
#define GAUGE_NAME_AUX_LINEAR_1 "Aux Linear 1"
#define GAUGE_NAME_AUX_LINEAR_2 "Aux Linear 2"
#define GAUGE_NAME_AUX_TEMP1 "Aux temp 1"
#define GAUGE_NAME_AUX_TEMP2 "Aux temp 2"
#define GAUGE_NAME_BARO_PRESSURE "Barometric pressure"
#define GAUGE_NAME_BOOST_CLOSED_LOOP "Boost Closed Loop"
#define GAUGE_NAME_BOOST_OPEN_LOOP "Boost Open Loop"
#define GAUGE_NAME_BOOST_OUTPUT "Boost Output"
#define GAUGE_NAME_BOOST_TARGET "Boost Target"
#define GAUGE_NAME_CLT "CLT"
#define GAUGE_NAME_CPU_TEMP "CPU Temperature"
#define GAUGE_NAME_CURRENT_GEAR "Current Gear"
#define GAUGE_NAME_DEBUG_F1 "debug f1"
#define GAUGE_NAME_DEBUG_F2 "debug f2: iTerm"
#define GAUGE_NAME_DEBUG_F3 "debug f3: prevError"
#define GAUGE_NAME_DEBUG_F4 "debug f4: iParam"
#define GAUGE_NAME_DEBUG_F5 "debug f5: dParam"
#define GAUGE_NAME_DEBUG_F6 "debug f6: dTerm"
#define GAUGE_NAME_DEBUG_F7 "debug f7"
#define GAUGE_NAME_DEBUG_I1 "debug i1: pParam"
#define GAUGE_NAME_DEBUG_I2 "debug i2: offset"
#define GAUGE_NAME_DEBUG_I3 "debug i3"
#define GAUGE_NAME_DEBUG_I4 "debug i4"
#define GAUGE_NAME_DEBUG_I5 "debug i5"
#define GAUGE_NAME_DESIRED_GEAR "Desired Gear"
#define GAUGE_NAME_DETECTED_GEAR "Detected Gear"
#define GAUGE_NAME_DWELL_DUTY "dwell: coil duty cycle"
#define GAUGE_NAME_ECU_TEMPERATURE "ECU temperature"
#define GAUGE_NAME_ENGINE_CRC16 "Engine CRC16"
#define GAUGE_NAME_ETB_DUTY "ETB Duty"
#define GAUGE_NAME_ETB_ERROR "ETB position error"
#define GAUGE_NAME_ETB_TARGET "ETB position target"
#define GAUGE_NAME_FLEX "Flex Ethanol %"
#define GAUGE_NAME_FUEL_BARO_CORR "fuel: Barometric pressure correction"
#define GAUGE_NAME_FUEL_BASE "fuel: base mass"
#define GAUGE_NAME_FUEL_CHARGE_TEMP "fuel: Estimated charge temperature"
#define GAUGE_NAME_FUEL_CLT_CORR "fuel: CLT correction"
#define GAUGE_NAME_FUEL_CONSUMPTION "fuel: Total consumed"
#define GAUGE_NAME_FUEL_CRANKING "fuel: cranking"
#define GAUGE_NAME_FUEL_EL_EXTRA "fuel: engine load acceleration extra fuel"
#define GAUGE_NAME_FUEL_FLOW "fuel: Flow rate"
#define GAUGE_NAME_FUEL_IAT_CORR "fuel: IAT correction"
#define GAUGE_NAME_FUEL_INJ_DUTY "fuel: injector duty cycle"
#define GAUGE_NAME_FUEL_LAST_INJECTION "fuel: Last injection"
#define GAUGE_NAME_FUEL_LEVEL "fuel level"
#define GAUGE_NAME_FUEL_LOAD "fuel: load"
#define GAUGE_NAME_FUEL_PRESSURE_HIGH "Fuel pressure (high)"
#define GAUGE_NAME_FUEL_PRESSURE_HIGH_UNITS "bar"
#define GAUGE_NAME_FUEL_PRESSURE_LOW "Fuel pressure (low)"
#define GAUGE_NAME_FUEL_PRESSURE_LOW_UNITS "kPa"
#define GAUGE_NAME_FUEL_RUNNING "fuel: running"
#define GAUGE_NAME_FUEL_TPS_EXTRA "fuel: TPS acceleration extra fuel ms"
#define GAUGE_NAME_FUEL_TPS_ROC "fuel: TPS change"
#define GAUGE_NAME_FUEL_TRIM "fuel: fuel trim"
#define GAUGE_NAME_FUEL_TRIM_2 "fuel: fuel trim 2"
#define GAUGE_NAME_FUEL_VE "fuel: VE"
#define GAUGE_NAME_FUEL_WALL_AMOUNT "fuel: wall amount"
#define GAUGE_NAME_FUEL_WALL_CORRECTION "fuel: wall correction"
#define GAUGE_NAME_FW_VERSION "ECU Software Version"
#define GAUGE_NAME_GEAR_RATIO "Gearbox Ratio"
#define GAUGE_NAME_IAC "Idle Air Valve"
#define GAUGE_NAME_IAT "IAT"
#define GAUGE_NAME_IDLE_POSITION "Idle position sensor"
#define GAUGE_NAME_IGNITION_LOAD "ign: load"
#define GAUGE_NAME_INJECTOR_LAG "fuel: injector lag"
#define GAUGE_NAME_KNOCK_1 "knock 1"
#define GAUGE_NAME_KNOCK_10 "knock 10"
#define GAUGE_NAME_KNOCK_11 "knock 11"
#define GAUGE_NAME_KNOCK_12 "knock 12"
#define GAUGE_NAME_KNOCK_2 "knock 2"
#define GAUGE_NAME_KNOCK_3 "knock 3"
#define GAUGE_NAME_KNOCK_4 "knock 4"
#define GAUGE_NAME_KNOCK_5 "knock 5"
#define GAUGE_NAME_KNOCK_6 "knock 6"
#define GAUGE_NAME_KNOCK_7 "knock 7"
#define GAUGE_NAME_KNOCK_8 "knock 8"
#define GAUGE_NAME_KNOCK_9 "knock 9"
#define GAUGE_NAME_KNOCK_COUNTER "knock: counter"
#define GAUGE_NAME_KNOCK_LEVEL "knock: current level"
#define GAUGE_NAME_LAMBDA "Lambda"
#define GAUGE_NAME_LAMBDA2 "Lambda 2"
#define GAUGE_NAME_LAST_ERROR "Last error"
#define GAUGE_NAME_MAF "MAF"
#define GAUGE_NAME_MAP "MAP"
#define GAUGE_NAME_OIL_PRESSURE "Oil Pressure"
#define GAUGE_NAME_OIL_PRESSURE_UNITS "kPa"
#define GAUGE_NAME_RAW_FUEL_PRESSURE_HIGH "Fuel pressure raw (high)"
#define GAUGE_NAME_RAW_FUEL_PRESSURE_LOW "Fuel pressure raw (low)"
#define GAUGE_NAME_RPM "RPM"
#define GAUGE_NAME_TARGET_AFR "fuel: target AFR"
#define GAUGE_NAME_TARGET_LAMBDA "fuel: target lambda"
#define GAUGE_NAME_TCHARGE "fuel: SD tCharge"
#define GAUGE_NAME_THROTTLE_PEDAL "Throttle pedal position"
#define GAUGE_NAME_TIME "Time"
#define GAUGE_NAME_TIMING_ADVANCE "timing"
#define GAUGE_NAME_TPS "TPS"
#define GAUGE_NAME_TPS2 "TPS2"
#define GAUGE_NAME_TRG_ERR "Trigger Error Counter"
#define GAUGE_NAME_TRG_GAP "Trigger Sync Latest Ratio"
#define GAUGE_NAME_TUNE_CRC16 "Tune CRC16"
#define GAUGE_NAME_TURBO_SPEED "Turbocharger Speed"
#define GAUGE_NAME_UPTIME "Uptime"
#define GAUGE_NAME_VBAT "VBatt"
#define GAUGE_NAME_VERSION "firmware"
#define GAUGE_NAME_VVS "Vehicle Speed"
#define GAUGE_NAME_VVT_B1E "VVT: bank 1 exhaust"
#define GAUGE_NAME_VVT_B1I "VVT: bank 1 intake"
#define GAUGE_NAME_VVT_B2E "VVT: bank 2 exhaust"
#define GAUGE_NAME_VVT_B2I "VVT: bank 2 intake"
#define GAUGE_NAME_VVT_TARGET_B1E "VVT: bank 1 exhaust target"
#define GAUGE_NAME_VVT_TARGET_B1I "VVT: bank 1 intake target"
#define GAUGE_NAME_VVT_TARGET_B2E "VVT: bank 2 exhaust target"
#define GAUGE_NAME_VVT_TARGET_B2I "VVT: bank 2 intake target"
#define GAUGE_NAME_WARNING_COUNT "Warning count"
#define GAUGE_NAME_WARNING_COUNTER "warning: counter"
#define GAUGE_NAME_WARNING_LAST "warning: last"
#define GAUGE_NAME_WG_POSITION "Wastegate position sensor"
#define gear_controller_e_enum "None", "Button Shift"
#define GEARS_COUNT 8
#define gppwm_channel_e_enum "TPS", "MAP", "CLT", "IAT", "Fuel Load", "Ignition Load", "Aux Temp 1", "Aux Temp 2", "Zero", "Accel Pedal", "Battery Voltage"
#define GPPWM_CHANNELS 4
#define GPPWM_LOAD_COUNT 8
#define GPPWM_NOTE_SIZE 16
#define GPPWM_RPM_COUNT 8
#define hasCriticalError_offset 0
#define highFuelPressure_offset 278
#define hpfp_cam_e_enum "NONE", "Intake 1", "Exhaust 1", "Intake 2", "Exhaust 2"
#define HPFP_COMPENSATION_SIZE 10
#define HPFP_DEADTIME_SIZE 8
#define HPFP_LOBE_PROFILE_SIZE 16
#define HPFP_TARGET_SIZE 10
#define HW_MAX_ADC_INDEX 17
#define IAC_PID_MULT_SIZE 8
#define IAT_CURVE_SIZE 16
#define iatCorrection_offset 64
#define IDLE_ADVANCE_CURVE_SIZE 8
#define IDLE_VE_SIZE 4
#define idleAirValvePosition_offset 90
#define idleCurrentPosition_offset 37
#define idlePositionSensor_offset 270
#define idleState_offset 492
#define idleStatus_dTerm_offset 530
#define idleStatus_error_offset 534
#define idleStatus_iTerm_offset 528
#define idleStatus_offset 528
#define idleStatus_output_offset 532
#define idleStatus_resetCounter_offset 536
#define idleTargetPosition_offset 133
#define IGN_LOAD_COUNT 16
#define IGN_RPM_COUNT 16
#define ignitionAdvance_offset 84
#define ignitionLoad_offset 136
#define INDICATOR_NAME_AC_SWITCH "AC switch"
#define INDICATOR_NAME_BRAKE_DOWN "brake: down"
#define INDICATOR_NAME_CLUTCH_DOWN "clutch: down"
#define INDICATOR_NAME_CLUTCH_UP "clutch: up"
#define injectionOffset_offset 58
#define injectorDutyCycle_offset 56
#define injectorFlowPressureDelta_offset 500
#define injectorFlowPressureRatio_offset 496
#define injectorLagMs_offset 62
#define instantMAPValue_offset 450
#define instantRpm_offset 358
#define intake_offset 14
#define internalMcuTemperature_offset 11
#define isCltError_offset 0
#define isFan2On_offset 0
#define isFanOn_offset 0
#define isFuelPumpOn_offset 0
#define isIatError_offset 0
#define isIdleClosedLoop_offset 0
#define isIdleCoasting_offset 0
#define isIgnitionEnabledIndicator_offset 0
#define isInjectionEnabledIndicator_offset 0
#define isKnockChipOk_offset 0
#define isMainRelayOn_offset 376
#define isMapError_offset 0
#define isO2HeaterOn_offset 0
#define isPedalError_offset 0
#define isTps2Error_offset 0
#define isTpsError_offset 0
#define isTriggerError_offset 0
#define isUsbConnected_offset 376
#define isWarnNow_offset 0
#define knock10_offset 259
#define knock11_offset 260
#define knock12_offset 261
#define knock1_offset 250
#define knock2_offset 251
#define knock3_offset 252
#define knock4_offset 253
#define knock5_offset 254
#define knock6_offset 255
#define knock7_offset 256
#define knock8_offset 257
#define knock9_offset 258
#define knockCount_offset 306
#define knockLevel_offset 108
#define knockRetard_offset 36
#define lambdaValue2_offset 286
#define lambdaValue_offset 34
#define lastErrorCode_offset 150
#define launchActivatePinState_offset 408
#define launchActivateSwitchCondition_offset 408
#define launchActivationMode_e_enum "Switch Input", "Clutch Input", "Always Active(Disabled By Speed)", "INVALID"
#define launchCombinedConditions_offset 408
#define launchIsLaunchCondition_offset 408
#define launchRpmCondition_offset 408
#define launchSpeedCondition_offset 408
#define launchTpsCondition_offset 408
#define launchTriggered_offset 0
#define LE_COMMAND_LENGTH 200
#define load_override_e_enum "None", "MAP", "TPS", "Acc Pedal", "Cyl Filling %"
#define LOG_DELIMITER "`"
#define LOGIC_ANALYZER_CHANNEL_COUNT 4
#define lowFuelPressure_offset 276
#define LUA_ANALOG_INPUT_COUNT 8
#define LUA_PWM_COUNT 8
#define LUA_SCRIPT_SIZE 8000
#define luaInvocationCounter_offset 380
#define luaLastCycleDuration_offset 384
#define m_I_sum_percent_offset 348
#define m_pressureTarget_kPa_offset 352
#define m_requested_pump_offset 334
#define MAF_DECODING_COUNT 256
#define maf_sensor_type_e_enum "v0", "v1", "v2", "v3"
#define mafEstimate_offset 356
#define mafMeasured_offset 28
#define MAIN_HELP_URL "http://www.rusefi.com/"
#define MAP_ANGLE_SIZE 8
#define MAP_WINDOW_SIZE 8
#define MAPValue_offset 30
#define MAX_CYLINDER_COUNT 12
#define maxLockedDuration_offset 456
#define maxTriggerReentrant_offset 458
#define MLQ_FIELD_HEADER_SIZE 55
#define MLQ_HEADER_SIZE 22
#define MOCK_PPS_POSITION_COMMAND "mock_pps_position"
#define MOCK_PPS_VOLTAGE_COMMAND "mock_pps_voltage"
#define mostRecentTimeBetweenIgnitionEvents_offset 454
#define mostRecentTimeBetweenSparkEvents_offset 452
#define multiSparkCounter_offset 491
#define NARROW_BAND_WIDE_BAND_CONVERSION_SIZE 8
#define needBurn_offset 0
#define oilPressure_offset 40
#define orderingErrorCounter_offset 144
#define output_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15", "MC33972_1", "MC33972_2", "MC33972_3", "MC33972_4", "MC33972_5", "MC33972_6", "MC33972_7", "MC33972_8", "MC33972_9", "MC33972_10", "MC33972_11", "MC33972_12", "MC33972_13", "MC33972_14", "MC33972_15", "MC33972_16", "MC33972_17", "MC33972_18", "MC33972_19", "MC33972_20", "MC33972_21", "MC33972_22", "TLE8888_1", "TLE8888_2", "TLE8888_3", "TLE8888_4", "TLE8888_5", "TLE8888_6", "TLE8888_7", "TLE8888_8", "TLE8888_9", "TLE8888_10", "TLE8888_11", "TLE8888_12", "TLE8888_13", "TLE8888_14", "TLE8888_15", "TLE8888_16", "TLE8888_17", "TLE8888_18", "TLE8888_19", "TLE8888_20", "TLE8888_21", "TLE8888_22", "TLE8888_23", "TLE8888_24", "TLE8888_25", "TLE8888_26", "TLE8888_27", "TLE8888_28", "INVALID", "INVALID", "INVALID", "TLE6240_1", "TLE6240_2", "TLE6240_3", "TLE6240_4", "TLE6240_5", "TLE6240_6", "TLE6240_7", "TLE6240_8", "TLE6240_9", "TLE6240_10", "TLE6240_11", "TLE6240_12", "TLE6240_13", "TLE6240_14", "TLE6240_15", "TLE6240_16", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
#define PACK_ADD_TEMPERATURE 40
#define PACK_MULT_AFR 1000
#define PACK_MULT_AFR_CFG 10
#define PACK_MULT_ANGLE 50
#define PACK_MULT_FUEL_FLOW 200
#define PACK_MULT_FUEL_MASS 100
#define PACK_MULT_HIGH_PRESSURE 10
#define PACK_MULT_LAMBDA 10000
#define PACK_MULT_LAMBDA_CFG 147
#define PACK_MULT_MAP_ESTIMATE 100
#define PACK_MULT_MASS_FLOW 10
#define PACK_MULT_MS 300
#define PACK_MULT_PERCENT 100
#define PACK_MULT_PRESSURE 30
#define PACK_MULT_TEMPERATURE 100
#define PACK_MULT_VOLTAGE 1000
#define PACK_PERCENT_BYTE_MULT 100.0
#define PEDAL_TO_TPS_SIZE 8
#define PERCENT_TRIM_BYTE_PACKING_DIV 0.02
#define pin_input_mode_e_enum "DEFAULT", "PULLUP", "PULLDOWN"
#define pin_mode_e_enum "default", "INVALID", "INVALID", "INVALID", "opendrain", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PULLUP", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PULLDOWN"
#define pin_output_mode_e_enum "default", "default inverted", "open collector", "open collector inverted"
#define PROTOCOL_ANALOG_CHART "analog_chart"
#define PROTOCOL_COIL1_SHORT_NAME "c1"
#define PROTOCOL_CRANK1 "t1"
#define PROTOCOL_CRANK2 "t2"
#define PROTOCOL_CRANK3 "t3"
#define PROTOCOL_ENGINE_SNIFFER "wave_chart"
#define PROTOCOL_ES_DOWN "d"
#define PROTOCOL_ES_UP "u"
#define PROTOCOL_HELLO_PREFIX "***"
#define PROTOCOL_HIP_NAME "HIP"
#define PROTOCOL_HPFP_NAME "hpfp"
#define PROTOCOL_INJ1_SHORT_NAME "i1"
#define PROTOCOL_MSG "msg"
#define PROTOCOL_OUTPIN "outpin"
#define PROTOCOL_SIGNATURE_PREFIX "rusEFI "
#define PROTOCOL_TACH_NAME "tach"
#define PROTOCOL_TEST_RESPONSE_TAG "ts_p_alive"
#define PROTOCOL_VERSION_TAG "rusEfiVersion"
#define PROTOCOL_VVT1_NAME "VVT1"
#define PROTOCOL_VVT2_NAME "VVT2"
#define PROTOCOL_VVT3_NAME "VVT3"
#define PROTOCOL_VVT4_NAME "VVT4"
#define PROTOCOL_WA_CHANNEL_1 "input1"
#define PROTOCOL_WA_CHANNEL_2 "input2"
#define PROTOCOL_WA_CHANNEL_3 "input3"
#define PROTOCOL_WA_CHANNEL_4 "input4"
#define rawAfr_offset 368
#define rawClt_offset 238
#define rawHighFuelPressure_offset 274
#define rawIat_offset 240
#define rawIdlePositionSensor_offset 264
#define rawLowFuelPressure_offset 272
#define rawMaf_offset 26
#define rawMap_offset 366
#define rawOilPressure_offset 242
#define rawPpsPrimary_offset 236
#define rawPpsSecondary_offset 248
#define rawTps1Primary_offset 234
#define rawTps1Secondary_offset 300
#define rawTps2Primary_offset 302
#define rawTps2Secondary_offset 304
#define rawWastegatePosition_offset 266
#define recentErrorCode1_offset 152
#define recentErrorCode2_offset 154
#define recentErrorCode3_offset 156
#define recentErrorCode4_offset 158
#define recentErrorCode5_offset 160
#define recentErrorCode6_offset 162
#define recentErrorCode7_offset 164
#define recentErrorCode8_offset 166
#define revolutionCounterSinceStart_offset 76
#define RPM_1_BYTE_PACKING_MULT 50
#define rpmAcceleration_offset 6
#define RPMValue_offset 4
#define SCRIPT_CURVE_16 16
#define SCRIPT_CURVE_8 8
#define SCRIPT_CURVE_COUNT 6
#define SCRIPT_SETTING_COUNT 8
#define SCRIPT_TABLE_8 8
#define SCRIPT_TABLE_COUNT 4
#define sd_logging_internal_offset 0
#define sd_msd_offset 0
#define sd_present_offset 0
#define seconds_offset 112
#define sensor_chart_e_enum "none", "trigger", "INVALID", "RPM ACCEL", "DETAILED RPM", "Fast Aux1"
#define SERVO_COUNT 8
#define show_Frankenso_presets true
#define show_test_presets true
#define sparkCutReason_offset 338
#define sparkDwellValue_offset 86
#define speedToRpmRatio_offset 8
#define starterRelayDisable_offset 490
#define starterState_offset 489
#define startStopStateToggleCounter_offset 488
#define stepper_num_micro_steps_e_enum "Full-Step (Default)", "INVALID", "Half-Step", "INVALID", "1/4 Micro-Step", "INVALID", "INVALID", "INVALID", "1/8 Micro-Step"
#define STFT_BANK_COUNT 2
#define STFT_CELL_COUNT 4
#define switch_input_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
#define systemEventReuse_offset 364
#define targetLambda_offset 280
#define tCharge_offset 60
#define tChargeMode_e_enum "RPM+TPS (Default)", "Air Mass Interpolation", "Table"
#define TCU_GEAR_COUNT 10
#define TCU_SOLENOID_COUNT 6
#define tcuCurrentGear_offset 247
#define tcuDesiredGear_offset 262
#define TEMPLOG_MAP_AT_CYCLE_COUNT_offset 412
#define TEMPLOG_MAP_AT_DIFF_offset 420
#define TEMPLOG_MAP_AT_SPECIAL_POINT_offset 416
#define TEMPLOG_map_peak_offset 413
#define testBenchIter_offset 388
#define throttlePedalPosition_offset 22
#define timingCltCorrection_offset 444
#define timingIatCorrection_offset 446
#define timingPidCorrection_offset 448
#define TOOTH_DATA_LENGTH 2000
#define TOOTH_PACKET_COUNT 1000
#define TOOTH_PACKET_SIZE 2
#define toothLogReady_offset 0
#define TOP_DEAD_CENTER_MESSAGE "r"
#define TORQUE_CURVE_SIZE 6
#define totalFuelConsumption_offset 100
#define totalTriggerErrorCounter_offset 140
#define tps12Split_offset 330
#define tps1Split_offset 326
#define tps2Split_offset 328
#define TPS2Value_offset 232
#define TPS_2_BYTE_PACKING_MULT 100
#define TPS_TPS_ACCEL_TABLE 8
#define tpsAccelActive_offset 408
#define tpsAccelFrom_offset 370
#define tpsAccelFuel_offset 82
#define tpsAccelTo_offset 371
#define tpsADC_offset 24
#define TPSValue_offset 20
#define transmission_controller_e_enum "None", "Simple Transmission", "GM 4L6X"
#define TRIGGER_COMMENT "#"
#define TRIGGER_HARDCODED_OPERATION_MODE "knownOperationMode"
#define TRIGGER_HAS_SECOND_CHANNEL "hasSecondChannel"
#define TRIGGER_INPUT_PIN_COUNT 3
#define TRIGGER_IS_CRANK_KEY "crankBased"
#define TRIGGER_IS_SECOND_WHEEL_CAM "isSecondWheelCam"
#define TRIGGER_SIMULATOR_PIN_COUNT 3
#define trigger_type_e_enum "custom toothed wheel", "Ford Aspire", "Dodge Neon 1995", "Miata NA", "INVALID", "GM_7X", "Cooper R50", "Mazda SOHC 4", "60/2", "36/1", "Mercedes Two Segment", "Mitsubishi 4G93", "Honda 4+24", "Honda 1+4+24", "Dodge Neon 2003", "Mazda DOHC 1+4", "1+1", "INVALID", "Single Tooth", "Dodge Ram 1+16", "60/2 VW", "Honda 1+24", "Dodge Stratus", "36_2_2_2", "Nissan Primera", "dev 2JZ 3/34 simulator", "Rover K", "GM LS 24", "Honda CBR 600", "2JZ_1_12", "Honda CBR 600 custom", "3/1 skipped", "Dodge Neon 2003 crank", "Miata NB", "INVALID", "INVALID", "Subaru 7+6", "Jeep 18-2-2-2", "WIP", "Dodge Neon 1995 crank only", "Jeep XJ 4 cyl", "FiatIAQ_P8", "Mazda Z5", "INVALID", "Renix 44-2-2", "Renix 66-2-2-2", "Honda K 12+1", "INVALID", "36/2", "Subaru SVX", "1+16", "Subaru 7 without 6", "INVALID", "TriTach", "GM 60/2/2/2", "Skoda Favorit", "Barra 3+1 Cam", "Kawa KX450F", "Nissan VQ35", "INVALID", "Nissan VQ30", "Nissan QR25", "Mitsubishi 3A92", "Subaru SVX Crank 1", "Subaru SVX Cam VVT", "Ford PIP", "Suzuki G13B", "Honda K 4+1", "Nissan MR18 Crank", "32/2", "36-2-1", "36-2-1-1", "trg72"
#define triggerPrimaryFall_offset 464
#define triggerPrimaryRise_offset 468
#define TRIGGERS_FILE_NAME "triggers.txt"
#define triggerSecondaryFall_offset 472
#define triggerSecondaryRise_offset 476
#define triggerStateIndex_offset 436
#define triggerSyncGapRatio_offset 432
#define triggerVvtFall_offset 480
#define triggerVvtRise_offset 484
#define TRIM_SIZE 4
#define TS_BURN_COMMAND 'B'
#define TS_BURN_COMMAND_char B
#define TS_CHUNK_WRITE_COMMAND 'C'
#define TS_CHUNK_WRITE_COMMAND_char C
#define TS_COMMAND_F 'F'
#define TS_COMMAND_F_char F
#define TS_COMPOSITE_DISABLE 2
#define TS_COMPOSITE_ENABLE 1
#define TS_CRC_CHECK_COMMAND 'k'
#define TS_CRC_CHECK_COMMAND_char k
#define TS_EXECUTE 'E'
#define TS_EXECUTE_char E
#define TS_FILE_VERSION 20200310
#define TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY '8'
#define TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY_char 8
#define TS_GET_CONFIG_ERROR 'e'
#define TS_GET_CONFIG_ERROR_char e
#define TS_GET_FIRMWARE_VERSION 'V'
#define TS_GET_FIRMWARE_VERSION_char V
#define TS_GET_LOGGER_GET_BUFFER 'L'
#define TS_GET_LOGGER_GET_BUFFER_char L
#define TS_GET_OUTPUTS_SIZE '4'
#define TS_GET_OUTPUTS_SIZE_char 4
#define TS_GET_STRUCT '9'
#define TS_GET_STRUCT_char 9
#define TS_GET_TEXT 'G'
#define TS_GET_TEXT_char G
#define TS_HELLO_COMMAND 'S'
#define TS_HELLO_COMMAND_char S
#define TS_IO_TEST_COMMAND 'Z'
#define TS_IO_TEST_COMMAND_char Z
#define TS_ONLINE_PROTOCOL 'z'
#define TS_ONLINE_PROTOCOL_char z
#define TS_OUTPUT_COMMAND 'O'
#define TS_OUTPUT_COMMAND_char O
#define TS_OUTPUT_SIZE 640
#define TS_PAGE_COMMAND 'P'
#define TS_PAGE_COMMAND_char P
#define TS_PERF_TRACE_BEGIN '_'
#define TS_PERF_TRACE_BEGIN_char _
#define TS_PERF_TRACE_GET_BUFFER 'b'
#define TS_PERF_TRACE_GET_BUFFER_char b
#define TS_PROTOCOL "001"
#define TS_QUERY_COMMAND 'Q'
#define TS_QUERY_COMMAND_char Q
#define TS_READ_COMMAND 'R'
#define TS_READ_COMMAND_char R
#define TS_RESPONSE_BURN_OK 4
#define TS_RESPONSE_COMMAND_OK 7
#define TS_RESPONSE_CRC_FAILURE 0x82
#define TS_RESPONSE_FRAMING_ERROR 0x8D
#define TS_RESPONSE_OK 0
#define TS_RESPONSE_OUT_OF_RANGE 0x84
#define TS_RESPONSE_UNDERRUN 0x80
#define TS_RESPONSE_UNRECOGNIZED_COMMAND 0x83
#define TS_SET_LOGGER_SWITCH 'l'
#define TS_SET_LOGGER_SWITCH_char l
#define ts_show_analog_divider true
#define ts_show_auxserial_pins true
#define ts_show_can2 true
#define ts_show_can_pins true
#define ts_show_cj125 true
#define ts_show_egt true
#define ts_show_etb true
#define ts_show_etb_pins true
#define ts_show_full_pinout true
#define ts_show_gps true
#define ts_show_hardware_simulator true
#define ts_show_hip9011 true
#define ts_show_joystick true
#define ts_show_lcd true
#define ts_show_main_relay true
#define ts_show_main_relay_microRusEFI_message false
#define ts_show_sd_card true
#define ts_show_sd_pins true
#define ts_show_software_knock false
#define ts_show_spi true
#define ts_show_trigger_comparator false
#define ts_show_tunerstudio_port true
#define TS_SINGLE_WRITE_COMMAND 'W'
#define TS_SINGLE_WRITE_COMMAND_char W
#define TS_TEST_COMMAND 't'
#define TS_TEST_COMMAND_char t
#define tsConfigVersion_offset 124
#define tuneCrc16_offset 244
#define turboSpeed_offset 318
#define unusedAtTheEnd10_offset 577
#define unusedAtTheEnd11_offset 578
#define unusedAtTheEnd12_offset 579
#define unusedAtTheEnd13_offset 580
#define unusedAtTheEnd14_offset 581
#define unusedAtTheEnd15_offset 582
#define unusedAtTheEnd16_offset 583
#define unusedAtTheEnd17_offset 584
#define unusedAtTheEnd18_offset 585
#define unusedAtTheEnd19_offset 586
#define unusedAtTheEnd1_offset 568
#define unusedAtTheEnd20_offset 587
#define unusedAtTheEnd21_offset 588
#define unusedAtTheEnd22_offset 589
#define unusedAtTheEnd23_offset 590
#define unusedAtTheEnd24_offset 591
#define unusedAtTheEnd25_offset 592
#define unusedAtTheEnd26_offset 593
#define unusedAtTheEnd27_offset 594
#define unusedAtTheEnd28_offset 595
#define unusedAtTheEnd29_offset 596
#define unusedAtTheEnd2_offset 569
#define unusedAtTheEnd30_offset 597
#define unusedAtTheEnd31_offset 598
#define unusedAtTheEnd32_offset 599
#define unusedAtTheEnd33_offset 600
#define unusedAtTheEnd34_offset 601
#define unusedAtTheEnd35_offset 602
#define unusedAtTheEnd36_offset 603
#define unusedAtTheEnd37_offset 604
#define unusedAtTheEnd38_offset 605
#define unusedAtTheEnd39_offset 606
#define unusedAtTheEnd3_offset 570
#define unusedAtTheEnd40_offset 607
#define unusedAtTheEnd41_offset 608
#define unusedAtTheEnd42_offset 609
#define unusedAtTheEnd43_offset 610
#define unusedAtTheEnd44_offset 611
#define unusedAtTheEnd45_offset 612
#define unusedAtTheEnd46_offset 613
#define unusedAtTheEnd47_offset 614
#define unusedAtTheEnd48_offset 615
#define unusedAtTheEnd49_offset 616
#define unusedAtTheEnd4_offset 571
#define unusedAtTheEnd50_offset 617
#define unusedAtTheEnd51_offset 618
#define unusedAtTheEnd52_offset 619
#define unusedAtTheEnd53_offset 620
#define unusedAtTheEnd54_offset 621
#define unusedAtTheEnd55_offset 622
#define unusedAtTheEnd56_offset 623
#define unusedAtTheEnd57_offset 624
#define unusedAtTheEnd58_offset 625
#define unusedAtTheEnd59_offset 626
#define unusedAtTheEnd5_offset 572
#define unusedAtTheEnd60_offset 627
#define unusedAtTheEnd61_offset 628
#define unusedAtTheEnd62_offset 629
#define unusedAtTheEnd63_offset 630
#define unusedAtTheEnd64_offset 631
#define unusedAtTheEnd65_offset 632
#define unusedAtTheEnd66_offset 633
#define unusedAtTheEnd67_offset 634
#define unusedAtTheEnd68_offset 635
#define unusedAtTheEnd69_offset 636
#define unusedAtTheEnd6_offset 573
#define unusedAtTheEnd70_offset 637
#define unusedAtTheEnd71_offset 638
#define unusedAtTheEnd72_offset 639
#define unusedAtTheEnd7_offset 574
#define unusedAtTheEnd8_offset 575
#define unusedAtTheEnd9_offset 576
#define unusedb4_offset 0
#define unusedBit_182_10_offset 376
#define unusedBit_182_11_offset 376
#define unusedBit_182_12_offset 376
#define unusedBit_182_13_offset 376
#define unusedBit_182_14_offset 376
#define unusedBit_182_15_offset 376
#define unusedBit_182_16_offset 376
#define unusedBit_182_17_offset 376
#define unusedBit_182_18_offset 376
#define unusedBit_182_19_offset 376
#define unusedBit_182_20_offset 376
#define unusedBit_182_21_offset 376
#define unusedBit_182_22_offset 376
#define unusedBit_182_23_offset 376
#define unusedBit_182_24_offset 376
#define unusedBit_182_25_offset 376
#define unusedBit_182_26_offset 376
#define unusedBit_182_27_offset 376
#define unusedBit_182_28_offset 376
#define unusedBit_182_29_offset 376
#define unusedBit_182_2_offset 376
#define unusedBit_182_30_offset 376
#define unusedBit_182_31_offset 376
#define unusedBit_182_3_offset 376
#define unusedBit_182_4_offset 376
#define unusedBit_182_5_offset 376
#define unusedBit_182_6_offset 376
#define unusedBit_182_7_offset 376
#define unusedBit_182_8_offset 376
#define unusedBit_182_9_offset 376
#define unusedBit_227_10_offset 408
#define unusedBit_227_11_offset 408
#define unusedBit_227_12_offset 408
#define unusedBit_227_13_offset 408
#define unusedBit_227_14_offset 408
#define unusedBit_227_15_offset 408
#define unusedBit_227_16_offset 408
#define unusedBit_227_17_offset 408
#define unusedBit_227_18_offset 408
#define unusedBit_227_19_offset 408
#define unusedBit_227_20_offset 408
#define unusedBit_227_21_offset 408
#define unusedBit_227_22_offset 408
#define unusedBit_227_23_offset 408
#define unusedBit_227_24_offset 408
#define unusedBit_227_25_offset 408
#define unusedBit_227_26_offset 408
#define unusedBit_227_27_offset 408
#define unusedBit_227_28_offset 408
#define unusedBit_227_29_offset 408
#define unusedBit_227_30_offset 408
#define unusedBit_227_31_offset 408
#define unusedBit_227_9_offset 408
#define unusedInTheMiddle10_offset 398
#define unusedInTheMiddle11_offset 399
#define unusedInTheMiddle12_offset 400
#define unusedInTheMiddle13_offset 401
#define unusedInTheMiddle14_offset 402
#define unusedInTheMiddle15_offset 403
#define unusedInTheMiddle1_offset 389
#define unusedInTheMiddle2_offset 390
#define unusedInTheMiddle3_offset 391
#define unusedInTheMiddle4_offset 392
#define unusedInTheMiddle5_offset 393
#define unusedInTheMiddle6_offset 394
#define unusedInTheMiddle7_offset 395
#define unusedInTheMiddle8_offset 396
#define unusedInTheMiddle9_offset 397
#define VBAT_INJECTOR_CURVE_SIZE 8
#define VBatt_offset 38
#define ve_override_e_enum "None", "MAP", "TPS"
#define VEHICLE_INFO_SIZE 32
#define vehicleSpeedKph_offset 10
#define veTableYAxis_offset 104
#define veValue_offset 57
#define VM_VVT_INACTIVE 0
#define VOLTAGE_1_BYTE_PACKING_DIV 0.02
#define VR_THRESHOLD_COUNT 2
#define VssAcceleration_offset 284
#define vvt_mode_e_enum "Inactive", "Single Tooth Second Half", "2GZ", "Miata NB2", "Single Tooth First Half", "Bosch Quick Start", "4/1", "ST 170", "Ford Barra 3+1", "Nissan VQ", "Honda K 4+1", "Nissan MR18", "Mitsu 3A92", "VTwin by MAP", "Mitsu 6G75", "vvt15", "vvt16", "vvt17"
#define vvtCounter_offset 437
#define vvtCurrentPosition_offset 428
#define vvtPositionB1E_offset 290
#define vvtPositionB1I_offset 42
#define vvtPositionB2E_offset 294
#define vvtPositionB2I_offset 292
#define vvtStateIndex_offset 439
#define vvtSyncCounter_offset 438
#define vvtSyncGapRatio_offset 424
#define vvtTargets1_offset 314
#define vvtTargets2_offset 315
#define vvtTargets3_offset 316
#define vvtTargets4_offset 317
#define wallFuelAmount_offset 72
#define wallFuelCorrection_offset 74
#define warningCounter_offset 148
#define wastegatePositionSensor_offset 268
#define wbHeaterDuty1_offset 324
#define wbHeaterDuty2_offset 325
#define wbTemperature1_offset 320
#define wbTemperature2_offset 322
#define WWAE_TABLE_SIZE 8
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
	/** total size 12*/
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
	 * @@GAUGE_NAME_FUEL_WALL_AMOUNT@@
	mg
	 * offset 72
	 */
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_CORRECTION@@
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
	dBv
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
	G
	 * offset 310
	 */
	scaled_channel<int16_t, 100, 1> accelerationRoll = (int16_t)0;
	/**
	 * @@GAUGE_NAME_ACCEL_YAW@@
	G
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
	 * @@GAUGE_NAME_BOOST_TARGET@@
	kPa
	 * offset 336
	 */
	scaled_channel<int16_t, 30, 1> boostControlTarget = (int16_t)0;
	/**
	 * "Spark Cut Code"
	code
	 * offset 338
	 */
	scaled_channel<int8_t, 1, 1> sparkCutReason = (int8_t)0;
	/**
	 * "Fuel Cut Code"
	code
	 * offset 339
	 */
	scaled_channel<int8_t, 1, 1> fuelCutReason = (int8_t)0;
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
	%
	 * offset 370
	 */
	uint8_t tpsAccelFrom = (uint8_t)0;
	/**
	%
	 * offset 371
	 */
	uint8_t tpsAccelTo = (uint8_t)0;
	/**
	 * offset 372
	 */
	scaled_channel<float, 1, 1> calibrationValue2 = (float)0;
	/**
	offset 376 bit 0 */
	bool isMainRelayOn : 1 {};
	/**
	 * Original reason for this is to check if USB is connected from Lua
	offset 376 bit 1 */
	bool isUsbConnected : 1 {};
	/**
	offset 376 bit 2 */
	bool unusedBit_182_2 : 1 {};
	/**
	offset 376 bit 3 */
	bool unusedBit_182_3 : 1 {};
	/**
	offset 376 bit 4 */
	bool unusedBit_182_4 : 1 {};
	/**
	offset 376 bit 5 */
	bool unusedBit_182_5 : 1 {};
	/**
	offset 376 bit 6 */
	bool unusedBit_182_6 : 1 {};
	/**
	offset 376 bit 7 */
	bool unusedBit_182_7 : 1 {};
	/**
	offset 376 bit 8 */
	bool unusedBit_182_8 : 1 {};
	/**
	offset 376 bit 9 */
	bool unusedBit_182_9 : 1 {};
	/**
	offset 376 bit 10 */
	bool unusedBit_182_10 : 1 {};
	/**
	offset 376 bit 11 */
	bool unusedBit_182_11 : 1 {};
	/**
	offset 376 bit 12 */
	bool unusedBit_182_12 : 1 {};
	/**
	offset 376 bit 13 */
	bool unusedBit_182_13 : 1 {};
	/**
	offset 376 bit 14 */
	bool unusedBit_182_14 : 1 {};
	/**
	offset 376 bit 15 */
	bool unusedBit_182_15 : 1 {};
	/**
	offset 376 bit 16 */
	bool unusedBit_182_16 : 1 {};
	/**
	offset 376 bit 17 */
	bool unusedBit_182_17 : 1 {};
	/**
	offset 376 bit 18 */
	bool unusedBit_182_18 : 1 {};
	/**
	offset 376 bit 19 */
	bool unusedBit_182_19 : 1 {};
	/**
	offset 376 bit 20 */
	bool unusedBit_182_20 : 1 {};
	/**
	offset 376 bit 21 */
	bool unusedBit_182_21 : 1 {};
	/**
	offset 376 bit 22 */
	bool unusedBit_182_22 : 1 {};
	/**
	offset 376 bit 23 */
	bool unusedBit_182_23 : 1 {};
	/**
	offset 376 bit 24 */
	bool unusedBit_182_24 : 1 {};
	/**
	offset 376 bit 25 */
	bool unusedBit_182_25 : 1 {};
	/**
	offset 376 bit 26 */
	bool unusedBit_182_26 : 1 {};
	/**
	offset 376 bit 27 */
	bool unusedBit_182_27 : 1 {};
	/**
	offset 376 bit 28 */
	bool unusedBit_182_28 : 1 {};
	/**
	offset 376 bit 29 */
	bool unusedBit_182_29 : 1 {};
	/**
	offset 376 bit 30 */
	bool unusedBit_182_30 : 1 {};
	/**
	offset 376 bit 31 */
	bool unusedBit_182_31 : 1 {};
	/**
	count
	 * offset 380
	 */
	int luaInvocationCounter = (int)0;
	/**
	nt
	 * offset 384
	 */
	int luaLastCycleDuration = (int)0;
	/**
	count
	 * offset 388
	 */
	uint8_t testBenchIter = (uint8_t)0;
	/**
	 * offset 389
	 */
	uint8_t unusedInTheMiddle[15];
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_1@@
	 * offset 404
	 */
	scaled_channel<int16_t, 100, 1> auxLinear1 = (int16_t)0;
	/**
	 * @@GAUGE_NAME_AUX_LINEAR_2@@
	 * offset 406
	 */
	scaled_channel<int16_t, 100, 1> auxLinear2 = (int16_t)0;
	/**
	offset 408 bit 0 */
	bool launchSpeedCondition : 1 {};
	/**
	offset 408 bit 1 */
	bool launchRpmCondition : 1 {};
	/**
	offset 408 bit 2 */
	bool launchTpsCondition : 1 {};
	/**
	offset 408 bit 3 */
	bool launchActivateSwitchCondition : 1 {};
	/**
	offset 408 bit 4 */
	bool launchIsLaunchCondition : 1 {};
	/**
	offset 408 bit 5 */
	bool launchCombinedConditions : 1 {};
	/**
	offset 408 bit 6 */
	bool launchActivatePinState : 1 {};
	/**
	offset 408 bit 7 */
	bool dfcoActive : 1 {};
	/**
	offset 408 bit 8 */
	bool tpsAccelActive : 1 {};
	/**
	offset 408 bit 9 */
	bool unusedBit_227_9 : 1 {};
	/**
	offset 408 bit 10 */
	bool unusedBit_227_10 : 1 {};
	/**
	offset 408 bit 11 */
	bool unusedBit_227_11 : 1 {};
	/**
	offset 408 bit 12 */
	bool unusedBit_227_12 : 1 {};
	/**
	offset 408 bit 13 */
	bool unusedBit_227_13 : 1 {};
	/**
	offset 408 bit 14 */
	bool unusedBit_227_14 : 1 {};
	/**
	offset 408 bit 15 */
	bool unusedBit_227_15 : 1 {};
	/**
	offset 408 bit 16 */
	bool unusedBit_227_16 : 1 {};
	/**
	offset 408 bit 17 */
	bool unusedBit_227_17 : 1 {};
	/**
	offset 408 bit 18 */
	bool unusedBit_227_18 : 1 {};
	/**
	offset 408 bit 19 */
	bool unusedBit_227_19 : 1 {};
	/**
	offset 408 bit 20 */
	bool unusedBit_227_20 : 1 {};
	/**
	offset 408 bit 21 */
	bool unusedBit_227_21 : 1 {};
	/**
	offset 408 bit 22 */
	bool unusedBit_227_22 : 1 {};
	/**
	offset 408 bit 23 */
	bool unusedBit_227_23 : 1 {};
	/**
	offset 408 bit 24 */
	bool unusedBit_227_24 : 1 {};
	/**
	offset 408 bit 25 */
	bool unusedBit_227_25 : 1 {};
	/**
	offset 408 bit 26 */
	bool unusedBit_227_26 : 1 {};
	/**
	offset 408 bit 27 */
	bool unusedBit_227_27 : 1 {};
	/**
	offset 408 bit 28 */
	bool unusedBit_227_28 : 1 {};
	/**
	offset 408 bit 29 */
	bool unusedBit_227_29 : 1 {};
	/**
	offset 408 bit 30 */
	bool unusedBit_227_30 : 1 {};
	/**
	offset 408 bit 31 */
	bool unusedBit_227_31 : 1 {};
	/**
	 * offset 412
	 */
	uint8_t TEMPLOG_MAP_AT_CYCLE_COUNT = (uint8_t)0;
	/**
	 * offset 413
	 */
	uint8_t TEMPLOG_map_peak = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_BOOST_OUTPUT@@
	%
	 * offset 414
	 */
	scaled_channel<uint8_t, 2, 1> boostControllerOutput = (uint8_t)0;
	/**
	 * @@GAUGE_NAME_BOOST_OPEN_LOOP@@
	%
	 * offset 415
	 */
	scaled_channel<uint8_t, 2, 1> boostControllerOpenLoopPart = (uint8_t)0;
	/**
	 * offset 416
	 */
	scaled_channel<float, 1, 1> TEMPLOG_MAP_AT_SPECIAL_POINT = (float)0;
	/**
	 * offset 420
	 */
	scaled_channel<float, 1, 1> TEMPLOG_MAP_AT_DIFF = (float)0;
	/**
	 * offset 424
	 */
	scaled_channel<float, 1, 1> vvtSyncGapRatio = (float)0;
	/**
	 * offset 428
	 */
	scaled_channel<float, 1, 1> vvtCurrentPosition = (float)0;
	/**
	 * @@GAUGE_NAME_TRG_GAP@@
	 * offset 432
	 */
	scaled_channel<float, 1, 1> triggerSyncGapRatio = (float)0;
	/**
	 * offset 436
	 */
	uint8_t triggerStateIndex = (uint8_t)0;
	/**
	 * offset 437
	 */
	uint8_t vvtCounter = (uint8_t)0;
	/**
	 * offset 438
	 */
	uint8_t vvtSyncCounter = (uint8_t)0;
	/**
	 * offset 439
	 */
	uint8_t vvtStateIndex = (uint8_t)0;
	/**
	kPa
	 * offset 440
	 */
	scaled_channel<uint16_t, 10, 1> fallbackMap = (uint16_t)0;
	/**
	 * @@GAUGE_NAME_BOOST_CLOSED_LOOP@@
	%
	 * offset 442
	 */
	scaled_channel<int8_t, 2, 1> boostControllerClosedLoopPart = (int8_t)0;
	/**
	 * @@GAUGE_NAME_DETECTED_GEAR@@
	 * offset 443
	 */
	uint8_t detectedGear = (uint8_t)0;
	/**
	deg
	 * offset 444
	 */
	scaled_channel<int16_t, 100, 1> timingCltCorrection = (int16_t)0;
	/**
	deg
	 * offset 446
	 */
	scaled_channel<int16_t, 100, 1> timingIatCorrection = (int16_t)0;
	/**
	deg
	 * offset 448
	 */
	scaled_channel<int16_t, 100, 1> timingPidCorrection = (int16_t)0;
	/**
	 * Instant MAP
	kPa
	 * offset 450
	 */
	scaled_channel<uint16_t, 30, 1> instantMAPValue = (uint16_t)0;
	/**
	 * offset 452
	 */
	uint16_t mostRecentTimeBetweenSparkEvents = (uint16_t)0;
	/**
	 * offset 454
	 */
	uint16_t mostRecentTimeBetweenIgnitionEvents = (uint16_t)0;
	/**
	 * offset 456
	 */
	uint16_t maxLockedDuration = (uint16_t)0;
	/**
	 * offset 458
	 */
	uint16_t maxTriggerReentrant = (uint16_t)0;
	/**
	 * offset 460
	 */
	uint16_t canWriteOk = (uint16_t)0;
	/**
	 * offset 462
	 */
	uint16_t canWriteNotOk = (uint16_t)0;
	/**
	 * offset 464
	 */
	int triggerPrimaryFall = (int)0;
	/**
	 * offset 468
	 */
	int triggerPrimaryRise = (int)0;
	/**
	 * offset 472
	 */
	int triggerSecondaryFall = (int)0;
	/**
	 * offset 476
	 */
	int triggerSecondaryRise = (int)0;
	/**
	 * offset 480
	 */
	int triggerVvtFall = (int)0;
	/**
	 * offset 484
	 */
	int triggerVvtRise = (int)0;
	/**
	 * offset 488
	 */
	uint8_t startStopStateToggleCounter = (uint8_t)0;
	/**
	 * offset 489
	 */
	uint8_t starterState = (uint8_t)0;
	/**
	 * offset 490
	 */
	uint8_t starterRelayDisable = (uint8_t)0;
	/**
	 * offset 491
	 */
	uint8_t multiSparkCounter = (uint8_t)0;
	/**
	 * offset 492
	 */
	int idleState = (int)0;
	/**
	 * offset 496
	 */
	float injectorFlowPressureRatio = (float)0;
	/**
	kPa
	 * offset 500
	 */
	float injectorFlowPressureDelta = (float)0;
	/**
	%
	 * offset 504
	 */
	float etbFeedForward = (float)0;
	/**
	 * offset 508
	 */
	float etbIntegralError = (float)0;
	/**
	%
	 * offset 512
	 */
	float etbCurrentTarget = (float)0;
	/**
	 * offset 516
	 */
	pid_status_s alternatorStatus;
	/**
	 * offset 528
	 */
	pid_status_s idleStatus;
	/**
	 * offset 540
	 */
	pid_status_s etbStatus;
	/**
	 * offset 552
	 */
	pid_status_s boostStatus;
	/**
	 * "aux speed 1"
	s
	 * offset 564
	 */
	uint16_t auxSpeed1 = (uint16_t)0;
	/**
	 * "aux speed 2"
	s
	 * offset 566
	 */
	uint16_t auxSpeed2 = (uint16_t)0;
	/**
	 * offset 568
	 */
	uint8_t unusedAtTheEnd[72];
	/** total size 640*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Thu Apr 14 15:30:56 EDT 2022
