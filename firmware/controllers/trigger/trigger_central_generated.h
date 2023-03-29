// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_central.txt Wed Mar 29 00:34:23 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
#define adc_channel_e_enum "Disabled", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5"
#define ADC_CHANNEL_NONE 0
#define AFTERSTART_DECAY_CURVE_SIZE 8
#define AFTERSTART_ENRICH_CURVE_SIZE 8
#define AFTERSTART_HOLD_CURVE_SIZE 8
#define antiLagActivationMode_e_enum "Switch Input", "Always Active"
#define AUX_DIGITAL_VALVE_COUNT 2
#define BANKS_COUNT 2
#define BARO_CORR_SIZE 4
#define BLOCKING_FACTOR 750
#define BOOST_LOAD_COUNT 8
#define BOOST_RPM_COUNT 8
#define brain_input_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15"
#define CAM_INPUTS_COUNT 4
#define CAMS_PER_BANK 2
#define can_baudrate_e_enum "50kbps", "83.33kbps", "100kbps", "125kbps", "250kbps", "500kbps", "1Mbps"
#define CAN_DEFAULT_BASE 0x200
#define CAN_ECU_HW_META 0xAB1234
#define CAN_ECU_SERIAL_RX_ID 0x100
#define CAN_ECU_SERIAL_TX_ID 0x102
#define CAN_RX_PREFIX "CAN_rx"
#define CanGpioType_enum "None", "DRT protocol", "MS protocol"
#define CLT_CRANKING_CURVE_SIZE 8
#define CLT_CURVE_SIZE 16
#define CLT_LIMITER_CURVE_SIZE 4
#define CLT_TIMING_CURVE_SIZE 8
#define CMD_AC_RELAY_BENCH "acrelaybench"
#define CMD_ALTERNATOR_PIN "set_alternator_pin"
#define CMD_BOOST_PIN "set_boost_pin"
#define CMD_BURNCONFIG "burnconfig"
#define CMD_DATE "date"
#define CMD_DISABLE "disable"
#define CMD_ECU_UNLOCK "unlock"
#define CMD_ENABLE "enable"
#define CMD_ENGINE_TYPE "engine_type"
#define CMD_ENGINESNIFFERRPMTHRESHOLD "engineSnifferRpmThreshold"
#define CMD_ETB_DUTY "set_etb_duty"
#define CMD_EXTERNAL_STIMULATION "ext_stimulation"
#define CMD_FAN2_BENCH "fan2bench"
#define CMD_FAN_BENCH "fanbench"
#define CMD_FUEL_BENCH "fuelbench"
#define CMD_FUEL_PUMP_BENCH "fuelpumpbench"
#define CMD_FUNCTIONAL_TEST_MODE "test_mode"
#define CMD_GET "get"
#define CMD_HPFP_BENCH "hpfpbench"
#define CMD_IDLE_BENCH "idlebench"
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
#define CMD_REBOOT_OPENBLT "reboot_openblt"
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
#define CMD_VSS_PIN "vss_pin"
#define CMD_WRITECONFIG "writeconfig"
#define COMPOSITE_PACKET_SIZE 5
#define CONSOLE_DATA_PROTOCOL_TAG " @"
#define CRANKING_ADVANCE_CURVE_SIZE 4
#define CRANKING_CLT_IDLE_CURVE_SIZE 8
#define CRANKING_CURVE_SIZE 8
#define CRITICAL_BUFFER_SIZE 120
#define CRITICAL_PREFIX "CRITICAL"
#define DC_PER_STEPPER 2
#define debug_mode_e_enum "INVALID", "TPS acceleration enrichment", "INVALID", "Stepper Idle Control", "Engine Load accl enrich", "Trigger Counters", "Soft Spark Cut", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "SD card", "sr5", "Knock", "INVALID", "Electronic Throttle", "Executor", "Bench Test / TS commands", "INVALID", "Analog inputs #1", "INSTANT_RPM", "INVALID", "Status", "CJ125", "INVALID", "MAP", "Metrics", "INVALID", "Ion Sense", "TLE8888", "Analog inputs #2", "Dwell Metric", "INVALID", "INVALID", "Boost Control", "INVALID", "INVALID", "ETB Autotune", "Composite Log", "INVALID", "INVALID", "INVALID", "Dyno_View", "Logic_Analyzer", "INVALID", "TCU", "Lua"
#define DIGIPOT_COUNT 4
#define DWELL_CURVE_SIZE 8
#define ego_sensor_e_enum "BPSX", "Innovate", "14Point7", "Narrow", "PLX", "Custom"
#define EGT_CHANNEL_COUNT 8
#define engine_load_mode_e_enum "Speed Density", "MAF Air Charge", "Alpha-N", "Lua"
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
#define ENGINE_NOISE_CURVE_SIZE 16
#define ENGINE_SNIFFER_UNIT_US 10
#define engineSyncCam_enum "Intake First Bank", "Exhaust First Bank", "Intake Second Bank", "Exhaust Second Bank"
#define ETB_BIAS_CURVE_LENGTH 8
#define ETB_COUNT 2
#define ETB_HW_MAX_FREQUENCY 3000
#define FLASH_DATA_VERSION 10020
#define FUEL_LEVEL_TABLE_COUNT 8
#define FUEL_LOAD_COUNT 16
#define FUEL_RPM_COUNT 16
#define GAP_TRACKING_LENGTH 18
#define GAUGE_CATEGORY_BOOST_CONTROL "Boost Control"
#define GAUGE_CATEGORY_ETB "ETB more"
#define GAUGE_CATEGORY_FUEL_MATH "Fuel: math"
#define GAUGE_CATEGORY_SYNC "Sync"
#define GAUGE_CATEGORY_TIMING "Timing"
#define GAUGE_COIL_DWELL_TIME "Ignition: coil charge time"
#define GAUGE_NAME_ACCEL_ROLL "Acceleration: Roll"
#define GAUGE_NAME_ACCEL_X "Acceleration: X"
#define GAUGE_NAME_ACCEL_Y "Acceleration: Y"
#define GAUGE_NAME_ACCEL_YAW "Acceleration: Yaw"
#define GAUGE_NAME_ACCEL_Z "Acceleration: Z"
#define GAUGE_NAME_AFR "Air/Fuel Ratio"
#define GAUGE_NAME_AFR2 "Air/Fuel Ratio 2"
#define GAUGE_NAME_AIR_FLOW_ESTIMATE "MAF estimate"
#define GAUGE_NAME_AIR_FLOW_MEASURED "MAF"
#define GAUGE_NAME_AIR_FLOW_MEASURED_2 "MAF #2"
#define GAUGE_NAME_AIR_MASS "air mass"
#define GAUGE_NAME_AUX_LINEAR_1 "Aux Linear #1"
#define GAUGE_NAME_AUX_LINEAR_2 "Aux Linear #2"
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
#define GAUGE_NAME_DWELL_DUTY "Ignition: coil duty cycle"
#define GAUGE_NAME_ECU_TEMPERATURE "ECU temperature"
#define GAUGE_NAME_ENGINE_CRC16 "Engine CRC16"
#define GAUGE_NAME_ETB_DUTY "ETB Duty"
#define GAUGE_NAME_ETB_ERROR "ETB position error"
#define GAUGE_NAME_ETB_TARGET "ETB position target"
#define GAUGE_NAME_FLEX "Flex Ethanol %"
#define GAUGE_NAME_FUEL_BARO_CORR "fuel: Barometric pressure mult"
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
#define GAUGE_NAME_FUEL_TPS_EXTRA "fuel: TPS acceleration add fuel ms"
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
#define GAUGE_NAME_IGNITION_LOAD "Ignition: load"
#define GAUGE_NAME_INJECTOR_LAG "fuel: injector lag"
#define GAUGE_NAME_ISS "Input Shaft Speed"
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
#define GAUGE_NAME_KNOCK_COUNTER "knock: count"
#define GAUGE_NAME_KNOCK_LEVEL "knock: current level"
#define GAUGE_NAME_KNOCK_RETARD "knock: retard"
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
#define GAUGE_NAME_TC_RATIO "Torque Converter Ratio"
#define GAUGE_NAME_TCHARGE "fuel: SD tCharge"
#define GAUGE_NAME_THROTTLE_PEDAL "Throttle pedal position"
#define GAUGE_NAME_TIME "Time"
#define GAUGE_NAME_TIMING_ADVANCE "timing"
#define GAUGE_NAME_TPS "TPS"
#define GAUGE_NAME_TPS2 "TPS2"
#define GAUGE_NAME_TRG_ERR "Trigger Error Counter"
#define GAUGE_NAME_TRG_GAP "Sync: Trigger Latest Ratio"
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
#define Gpio_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15"
#define gppwm_channel_e_enum "Zero", "TPS", "MAP", "CLT", "IAT", "Fuel Load", "Ignition Load", "Aux Temp 1", "Aux Temp 2", "Accel Pedal", "Battery Voltage", "VVT 1 I", "VVT 1 E", "VVT 2 I", "VVT 2 E", "Ethanol (Flex) %", "Aux Linear 1", "Aux Linear 2", "GPPWM Output 1", "GPPWM Output 2", "GPPWM Output 3", "GPPWM Output 4", "Lua Gauge 1", "Lua Gauge 2", "RPM"
#define GPPWM_CHANNELS 4
#define GPPWM_LOAD_COUNT 8
#define GPPWM_NOTE_SIZE 16
#define GPPWM_RPM_COUNT 8
#define HIGH_SPEED_COUNT 32
#define hpfp_cam_e_enum "NONE", "Intake 1", "Exhaust 1", "Intake 2", "Exhaust 2"
#define HPFP_COMPENSATION_SIZE 10
#define HPFP_DEADTIME_SIZE 8
#define HPFP_LOBE_PROFILE_SIZE 16
#define HPFP_TARGET_SIZE 10
#define HW_EVENT_TYPES 6
#define HW_MAX_ADC_INDEX 17
#define IAC_PID_MULT_SIZE 8
#define IAT_CURVE_SIZE 16
#define IDLE_ADVANCE_CURVE_SIZE 8
#define IDLE_VE_SIZE 4
#define IGN_BLEND_COUNT 4
#define IGN_LOAD_COUNT 16
#define IGN_RPM_COUNT 16
#define INDICATOR_NAME_AC_SWITCH "AC switch"
#define INDICATOR_NAME_BRAKE_DOWN "brake: down"
#define INDICATOR_NAME_CLUTCH_DOWN "clutch: down"
#define INDICATOR_NAME_CLUTCH_UP "clutch: up"
#define launchActivationMode_e_enum "Launch Button", "Clutch Down Switch", "Always Active(Disabled By Speed)"
#define load_override_e_enum "None", "MAP", "TPS", "Acc Pedal", "Cyl Filling %"
#define LOG_DELIMITER "`"
#define LOGIC_ANALYZER_CHANNEL_COUNT 4
#define LUA_ANALOG_INPUT_COUNT 8
#define LUA_DIGITAL_INPUT_COUNT 8
#define LUA_GAUGE_COUNT 2
#define LUA_PWM_COUNT 8
#define LUA_SCRIPT_SIZE 8000
#define MAF_DECODING_COUNT 256
#define maf_sensor_type_e_enum "v0", "v1", "v2", "v3"
#define MAIN_HELP_URL "http://www.rusefi.com/"
#define MAP_ANGLE_SIZE 8
#define MAP_WINDOW_SIZE 8
#define MAX_CYLINDER_COUNT 12
#define MLQ_FIELD_HEADER_SIZE 89
#define MLQ_HEADER_SIZE 24
#define MOCK_PPS_POSITION_COMMAND "mock_pps_position"
#define MOCK_PPS_VOLTAGE_COMMAND "mock_pps_voltage"
#define NARROW_BAND_WIDE_BAND_CONVERSION_SIZE 8
#define output_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15", "MC33972_1", "MC33972_2", "MC33972_3", "MC33972_4", "MC33972_5", "MC33972_6", "MC33972_7", "MC33972_8", "MC33972_9", "MC33972_10", "MC33972_11", "MC33972_12", "MC33972_13", "MC33972_14", "MC33972_15", "MC33972_16", "MC33972_17", "MC33972_18", "MC33972_19", "MC33972_20", "MC33972_21", "MC33972_22", "TLE8888_1", "TLE8888_2", "TLE8888_3", "TLE8888_4", "TLE8888_5", "TLE8888_6", "TLE8888_7", "TLE8888_8", "TLE8888_9", "TLE8888_10", "TLE8888_11", "TLE8888_12", "TLE8888_13", "TLE8888_14", "TLE8888_15", "TLE8888_16", "TLE8888_17", "TLE8888_18", "TLE8888_19", "TLE8888_20", "TLE8888_21", "TLE8888_22", "TLE8888_23", "TLE8888_24", "TLE8888_25", "TLE8888_26", "TLE8888_27", "TLE8888_28", "INVALID", "INVALID", "INVALID", "TLE6240_1", "TLE6240_2", "TLE6240_3", "TLE6240_4", "TLE6240_5", "TLE6240_6", "TLE6240_7", "TLE6240_8", "TLE6240_9", "TLE6240_10", "TLE6240_11", "TLE6240_12", "TLE6240_13", "TLE6240_14", "TLE6240_15", "TLE6240_16"
#define PACK_ADD_TEMPERATURE 40
#define PACK_MULT_AFR 1000
#define PACK_MULT_AFR_CFG 10
#define PACK_MULT_ANGLE 50
#define PACK_MULT_FUEL_FLOW 200
#define PACK_MULT_FUEL_MASS 100
#define PACK_MULT_HIGH_PRESSURE 10
#define PACK_MULT_LAMBDA 10000
#define PACK_MULT_LAMBDA_CFG 147
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
#define SCRIPT_CURVE_16 16
#define SCRIPT_CURVE_8 8
#define SCRIPT_CURVE_COUNT 6
#define SCRIPT_SETTING_COUNT 8
#define SCRIPT_TABLE_8 8
#define SCRIPT_TABLE_COUNT 4
#define sensor_chart_e_enum "none", "trigger", "INVALID", "RPM ACCEL", "DETAILED RPM", "Fast Aux1"
#define SENT_INPUT_COUNT 1
#define sent_input_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15"
#define SentEtbType_enum "None", "GM type 1", "Ford type 1", "Custom"
#define show_Frankenso_presets true
#define show_test_presets true
#define stepper_num_micro_steps_e_enum "Full-Step (Default)", "INVALID", "Half-Step", "INVALID", "1/4 Micro-Step", "INVALID", "INVALID", "INVALID", "1/8 Micro-Step"
#define STFT_BANK_COUNT 2
#define STFT_CELL_COUNT 4
#define switch_input_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15"
#define tChargeMode_e_enum "RPM+TPS (Default)", "Air Mass Interpolation", "Table"
#define TCU_GEAR_COUNT 10
#define TCU_MAGIC_SIZE 8
#define TCU_SOLENOID_COUNT 6
#define TOOTH_DATA_LENGTH 2000
#define TOOTH_PACKET_COUNT 1000
#define TOOTH_PACKET_SIZE 2
#define TOP_DEAD_CENTER_MESSAGE "r"
#define TORQUE_CURVE_SIZE 6
#define TPS_2_BYTE_PACKING_MULT 100
#define TPS_TPS_ACCEL_CLT_CORR_TABLE 4
#define TPS_TPS_ACCEL_TABLE 8
#define transmission_controller_e_enum "None", "Simple Transmission", "GM 4L6X"
#define TRIGGER_COMMENT "#"
#define TRIGGER_CYCLE_DURATION "cycleDuration"
#define TRIGGER_GAP_FROM "gapFrom"
#define TRIGGER_GAP_TO "gapTo"
#define TRIGGER_GAPS_COUNT "gapsCount"
#define TRIGGER_HARDCODED_OPERATION_MODE "knownOperationMode"
#define TRIGGER_HAS_SECOND_CHANNEL "hasSecondChannel"
#define TRIGGER_INPUT_PIN_COUNT 2
#define TRIGGER_IS_CRANK_KEY "crankBased"
#define TRIGGER_IS_SECOND_WHEEL_CAM "isSecondWheelCam"
#define TRIGGER_SIMULATOR_PIN_COUNT 2
#define trigger_type_e_enum "custom toothed wheel", "Ford Aspire", "Dodge Neon 1995", "Miata NA", "Mitsubishi Mess 4", "GM_7X", "Cooper R50", "Mazda SOHC 4", "60/2", "36/1", "Mercedes Two Segment", "Mitsubishi 4G93 11", "EZ30", "INVALID", "Dodge Neon 2003", "Mazda DOHC 1+4", "1+1", "INVALID", "Single Tooth", "Dodge Ram 1+16", "60/2 VW", "Benelli Tre", "Dodge Stratus", "36_2_2_2", "Nissan Primera", "dev 2JZ 3/34 simulator", "Rover K", "GM 24x 5 degree", "Honda CBR 600", "Mitsubishi 4G92/93/94 Cam 29", "Honda CBR 600 custom", "3/1 skipped", "Dodge Neon 2003 crank", "Miata NB", "Mitsubishi 4G63 Cam 34", "INVALID", "Subaru 7+6", "Jeep 18-2-2-2", "12 tooth crank", "Dodge Neon 1995 crank only", "Jeep XJ 4 cyl", "FiatIAQ_P8", "Mazda Z5", "INVALID", "Renix 44-2-2", "Renix 66-2-2-2", "Honda K 12+1", "INVALID", "36/2", "Subaru SVX", "1+16", "Subaru 7 without 6", "INVALID", "TriTach", "GM 60/2/2/2", "Skoda Favorit", "Barra 3+1 Cam", "Kawa KX450F", "Nissan VQ35", "INVALID", "Nissan VQ30", "Nissan QR25", "Mitsubishi 3A92", "Subaru SVX Crank 1", "Subaru SVX Cam VVT", "Ford PIP", "Suzuki G13B", "Honda K 4+1", "Nissan MR18 Crank", "32/2", "36-2-1", "36-2-1-1", "INVALID", "INVALID", "GM 24x 3 degree", "trg75", "trg76", "trg77", "trg78", "trg79"
#define TRIGGER_WITH_SYNC "isSynchronizationNeeded"
#define TRIGGERS_FILE_NAME "triggers.txt"
#define TRIM_SIZE 4
#define TS_BLOCK_READ_TIMEOUT 3000
#define TS_BURN_COMMAND 'B'
#define TS_BURN_COMMAND_char B
#define TS_CHUNK_WRITE_COMMAND 'C'
#define TS_CHUNK_WRITE_COMMAND_char C
#define TS_COMMAND_F 'F'
#define TS_COMMAND_F_char F
#define TS_COMPOSITE_DISABLE 2
#define TS_COMPOSITE_ENABLE 1
#define TS_COMPOSITE_READ 3
#define TS_CRC_CHECK_COMMAND 'k'
#define TS_CRC_CHECK_COMMAND_char k
#define TS_EXECUTE 'E'
#define TS_EXECUTE_char E
#define TS_FILE_VERSION 20210312
#define TS_FILE_VERSION_OFFSET 124
#define TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY '8'
#define TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY_char 8
#define TS_GET_CONFIG_ERROR 'e'
#define TS_GET_CONFIG_ERROR_char e
#define TS_GET_FIRMWARE_VERSION 'V'
#define TS_GET_FIRMWARE_VERSION_char V
#define TS_GET_OUTPUTS_SIZE '4'
#define TS_GET_OUTPUTS_SIZE_char 4
#define TS_GET_PROTOCOL_VERSION_COMMAND_F 'F'
#define TS_GET_PROTOCOL_VERSION_COMMAND_F_char F
#define TS_GET_SCATTERED_GET_COMMAND '9'
#define TS_GET_SCATTERED_GET_COMMAND_char 9
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
#define ts_show_clt_iat_pullup true
#define ts_show_critical_led false
#define ts_show_egt true
#define ts_show_etb true
#define ts_show_etb_pins true
#define ts_show_full_pinout true
#define ts_show_gps true
#define ts_show_hardware_simulator true
#define ts_show_hip9011 false
#define ts_show_ignitionPinMode true
#define ts_show_inj_diag false
#define ts_show_injectionPinMode true
#define ts_show_joystick true
#define ts_show_lcd true
#define ts_show_main_relay true
#define ts_show_main_relay_microRusEFI_message false
#define ts_show_sd_card true
#define ts_show_sd_pins true
#define ts_show_software_knock false
#define ts_show_spi true
#define ts_show_tps_sent false
#define ts_show_trigger_comparator false
#define ts_show_tunerstudio_port true
#define ts_show_vbatt true
#define ts_show_vr_threshold_2 true
#define ts_show_vr_threshold_all true
#define TS_SINGLE_WRITE_COMMAND 'W'
#define TS_SINGLE_WRITE_COMMAND_char W
#define TS_TEST_COMMAND 't'
#define TS_TEST_COMMAND_char t
#define TS_TRIGGER_SCOPE_CHANNEL_1_NAME "Channel 1"
#define TS_TRIGGER_SCOPE_CHANNEL_2_NAME "Channel 2"
#define TS_TRIGGER_SCOPE_DISABLE 5
#define TS_TRIGGER_SCOPE_ENABLE 4
#define TS_TRIGGER_SCOPE_READ 6
#define UiMode_enum "Full", "Installation", "Tuning"
#define VBAT_INJECTOR_CURVE_SIZE 8
#define VE_BLEND_COUNT 4
#define ve_override_e_enum "None", "MAP", "TPS"
#define VEHICLE_INFO_SIZE 32
#define VIN_NUMBER_SIZE 17
#define VM_VVT_INACTIVE 0
#define VOLTAGE_1_BYTE_PACKING_DIV 0.02
#define VR_THRESHOLD_COUNT 2
#define vvt_mode_e_enum "Inactive", "Single Tooth Second Half", "2JZ", "Miata NB2", "Single Tooth First Half", "Bosch Quick Start", "4/1", "ST 170", "Ford Barra 3+1", "Nissan VQ", "Honda K Intake", "Nissan MR18", "Mitsu 3A92", "VTwin by MAP", "Mitsu 6G75", "Mazda Skyactiv", "Honda K Exhaust", "Mitsubishi 4G92/93/94", "Mitsubishi 4G63"
#define WARNING_BUFFER_SIZE 120
#define WWAE_TABLE_SIZE 8
// start of trigger_central_s
struct trigger_central_s {
	/**
	 * Hardware events since boot
	 * offset 0
	 */
	uint32_t hwEventCounters[HW_EVENT_TYPES];
	/**
	 * Sync: total cams front counter
	 * offset 24
	 */
	uint32_t vvtCamCounter = (uint32_t)0;
	/**
	 * offset 28
	 */
	float mapVvt_MAP_AT_SPECIAL_POINT = (float)0;
	/**
	 * offset 32
	 */
	float mapVvt_MAP_AT_DIFF = (float)0;
	/**
	 * offset 36
	 */
	uint8_t mapVvt_MAP_AT_CYCLE_COUNT = (uint8_t)0;
	/**
	 * offset 37
	 */
	uint8_t mapVvt_map_peak = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 38
	 */
	uint8_t alignmentFill_at_38[2];
	/**
	 * Sync: Engine Phase
	deg
	 * offset 40
	 */
	float currentEngineDecodedPhase = (float)0;
	/**
	 * Sync: trigger angle error
	deg
	 * offset 44
	 */
	float triggerToothAngleError = (float)0;
	/**
	 * offset 48
	 */
	uint8_t triggerIgnoredToothCount = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 49
	 */
	uint8_t alignmentFill_at_49[3];
};
static_assert(sizeof(trigger_central_s) == 52);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_central.txt Wed Mar 29 00:34:23 UTC 2023
