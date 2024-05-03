//
// was generated automatically by rusEFI tool config_definition-all.jar based on gen_config.sh integration/rusefi_config.txt
//

#define adc_channel_e_auto_enum 0="EFI_ADC_NONE",1="EFI_ADC_0",2="EFI_ADC_1",11="EFI_ADC_10",12="EFI_ADC_11",13="EFI_ADC_12",14="EFI_ADC_13",15="EFI_ADC_14",16="EFI_ADC_15",17="EFI_ADC_16",18="EFI_ADC_17",19="EFI_ADC_18",20="EFI_ADC_19",3="EFI_ADC_2",21="EFI_ADC_20",22="EFI_ADC_21",23="EFI_ADC_22",24="EFI_ADC_23",25="EFI_ADC_24",26="EFI_ADC_25",27="EFI_ADC_26",28="EFI_ADC_27",29="EFI_ADC_28",30="EFI_ADC_29",4="EFI_ADC_3",31="EFI_ADC_30",32="EFI_ADC_31",5="EFI_ADC_4",6="EFI_ADC_5",7="EFI_ADC_6",8="EFI_ADC_7",9="EFI_ADC_8",10="EFI_ADC_9",50="EFI_ADC_ERROR",33="EFI_ADC_LAST_CHANNEL"
#define adc_channel_e_EFI_ADC_0 1
#define adc_channel_e_EFI_ADC_1 2
#define adc_channel_e_EFI_ADC_10 11
#define adc_channel_e_EFI_ADC_11 12
#define adc_channel_e_EFI_ADC_12 13
#define adc_channel_e_EFI_ADC_13 14
#define adc_channel_e_EFI_ADC_14 15
#define adc_channel_e_EFI_ADC_15 16
#define adc_channel_e_EFI_ADC_16 17
#define adc_channel_e_EFI_ADC_17 18
#define adc_channel_e_EFI_ADC_18 19
#define adc_channel_e_EFI_ADC_19 20
#define adc_channel_e_EFI_ADC_2 3
#define adc_channel_e_EFI_ADC_20 21
#define adc_channel_e_EFI_ADC_21 22
#define adc_channel_e_EFI_ADC_22 23
#define adc_channel_e_EFI_ADC_23 24
#define adc_channel_e_EFI_ADC_24 25
#define adc_channel_e_EFI_ADC_25 26
#define adc_channel_e_EFI_ADC_26 27
#define adc_channel_e_EFI_ADC_27 28
#define adc_channel_e_EFI_ADC_28 29
#define adc_channel_e_EFI_ADC_29 30
#define adc_channel_e_EFI_ADC_3 4
#define adc_channel_e_EFI_ADC_30 31
#define adc_channel_e_EFI_ADC_31 32
#define adc_channel_e_EFI_ADC_4 5
#define adc_channel_e_EFI_ADC_5 6
#define adc_channel_e_EFI_ADC_6 7
#define adc_channel_e_EFI_ADC_7 8
#define adc_channel_e_EFI_ADC_8 9
#define adc_channel_e_EFI_ADC_9 10
#define adc_channel_e_EFI_ADC_ERROR 50
#define adc_channel_e_EFI_ADC_LAST_CHANNEL 33
#define adc_channel_e_EFI_ADC_NONE 0
#define adc_channel_e_enum 0="NONE",2="3L - IGN_7 / AFR",7="3V - CAM (A19)",10="3Y - CRANK (A24)",16="4AE - EGR/MAP4 (A22)",9="4F - AC_PRES/AUX1 (A23)",8="4J - VTCS/AUX4 (A20)",14="4N - IAT (A14)",13="4P - CLT (A11)",6="4S/4T - Alternator voltage",12="4U - MAP2/Ign8 (A10)",5="4V - TPS (A17)",11="4X - MAF (A9)",15="5M - PPS2 OR TEMPERATURE SENSOR",1="5N - TPS2",4="5P - PPS1",3="intMAP (A15)"
#define adc_channel_e_fullenum "NONE","5N - TPS2","3L - IGN_7 / AFR","intMAP (A15)","5P - PPS1","4V - TPS (A17)","4S/4T - Alternator voltage","3V - CAM (A19)","4J - VTCS/AUX4 (A20)","4F - AC_PRES/AUX1 (A23)","3Y - CRANK (A24)","4X - MAF (A9)","4U - MAP2/Ign8 (A10)","4P - CLT (A11)","4N - IAT (A14)","5M - PPS2 OR TEMPERATURE SENSOR","4AE - EGR/MAP4 (A22)"
#define ADC_CHANNEL_NONE 0
#define afr_sensor_s_size 20
#define air_pressure_sensor_config_s_size 12
#define air_pressure_sensor_type_e_auto_enum 0="MT_CUSTOM",10="MT_BOSCH_2_5",1="MT_DENSO183",4="MT_DODGE_NEON_2003",13="MT_GM_1_BAR",12="MT_GM_2_BAR",6="MT_GM_3_BAR",3="MT_HONDA3BAR",11="MT_MAZDA_1_BAR",7="MT_MPX4100",2="MT_MPX4250",9="MT_MPX4250A",15="MT_MPXH6300",14="MT_MPXH6400",5="MT_SUBY_DENSO",8="MT_TOYOTA_89420_02010"
#define air_pressure_sensor_type_e_MT_BOSCH_2_5 10
#define air_pressure_sensor_type_e_MT_CUSTOM 0
#define air_pressure_sensor_type_e_MT_DENSO183 1
#define air_pressure_sensor_type_e_MT_DODGE_NEON_2003 4
#define air_pressure_sensor_type_e_MT_GM_1_BAR 13
#define air_pressure_sensor_type_e_MT_GM_2_BAR 12
#define air_pressure_sensor_type_e_MT_GM_3_BAR 6
#define air_pressure_sensor_type_e_MT_HONDA3BAR 3
#define air_pressure_sensor_type_e_MT_MAZDA_1_BAR 11
#define air_pressure_sensor_type_e_MT_MPX4100 7
#define air_pressure_sensor_type_e_MT_MPX4250 2
#define air_pressure_sensor_type_e_MT_MPX4250A 9
#define air_pressure_sensor_type_e_MT_MPXH6300 15
#define air_pressure_sensor_type_e_MT_MPXH6400 14
#define air_pressure_sensor_type_e_MT_SUBY_DENSO 5
#define air_pressure_sensor_type_e_MT_TOYOTA_89420_02010 8
#define antiLagActivationMode_e_ALWAYS_ON_ANTILAG 1
#define antiLagActivationMode_e_auto_enum 0="SWITCH_INPUT_ANTILAG",1="ALWAYS_ON_ANTILAG"
#define antiLagActivationMode_e_enum "Switch Input", "Always Active"
#define antiLagActivationMode_e_SWITCH_INPUT_ANTILAG 0
#define AUX_DIGITAL_VALVE_COUNT 2
#define AUX_SPEED_SENSOR_COUNT 2
#define BANKS_COUNT 2
#define BARO_CORR_SIZE 4
#define BENCH_AC_RELAY_DURATION 800
#define BENCH_FAN_DURATION 2000
#define BENCH_FUEL_PUMP_DURATION 3000
#define BENCH_MAIN_RELAY_DURATION 1000
#define bench_mode_e_BENCH_AC_COMPRESSOR_RELAY 6
#define bench_mode_e_BENCH_AUXOUT0 20
#define bench_mode_e_BENCH_AUXOUT1 21
#define bench_mode_e_BENCH_AUXOUT2 22
#define bench_mode_e_BENCH_AUXOUT3 23
#define bench_mode_e_BENCH_AUXOUT4 24
#define bench_mode_e_BENCH_AUXOUT5 25
#define bench_mode_e_BENCH_AUXOUT6 26
#define bench_mode_e_BENCH_AUXOUT7 27
#define bench_mode_e_BENCH_CANCEL 15
#define bench_mode_e_BENCH_CHECK_ENGINE_LIGHT 7
#define bench_mode_e_BENCH_FAN_RELAY 4
#define bench_mode_e_BENCH_FAN_RELAY_2 5
#define bench_mode_e_BENCH_FUEL_PUMP 1
#define bench_mode_e_BENCH_GPPWM1_VALVE 10
#define bench_mode_e_BENCH_GPPWM2_VALVE 11
#define bench_mode_e_BENCH_GPPWM3_VALVE 12
#define bench_mode_e_BENCH_GPPWM4_VALVE 13
#define bench_mode_e_BENCH_HPFP_VALVE 9
#define bench_mode_e_BENCH_IDLE_VALVE 8
#define bench_mode_e_BENCH_MAIN_RELAY 0
#define bench_mode_e_BENCH_SECOND_IDLE_VALVE 14
#define bench_mode_e_BENCH_STARTER_DISABLE_RELAY 3
#define bench_mode_e_BENCH_STARTER_ENABLE_RELAY 2
#define bench_mode_e_BENCH_VVT0_VALVE 16
#define bench_mode_e_BENCH_VVT1_VALVE 17
#define bench_mode_e_BENCH_VVT2_VALVE 18
#define bench_mode_e_BENCH_VVT3_VALVE 19
#define bench_mode_e_HD_ACR 28
#define bench_mode_e_HD_ACR2 29
#define BENCH_STARTER_DURATION 4000
#define BENCH_VVT_DURATION 300
#define blend_table_s_size 188
#define BLOCKING_FACTOR 750
#define BOARD_CONFIG_FROM_FILE 
#define BOARD_MENU_FROM_FILE 
#define BOARD_OPTIONS_FROM_FILE 
#define BOARD_TABLES_FROM_FILE 
#define BOOST_BLEND_COUNT 2
#define BOOST_LOAD_COUNT 8
#define BOOST_RPM_COUNT 8
#define boostType_e_auto_enum 0="OPEN_LOOP",1="CLOSED_LOOP"
#define boostType_e_CLOSED_LOOP 1
#define boostType_e_OPEN_LOOP 0
#define brain_input_pin_e_enum 0="NONE",93="3T - VSS (D5)",8="3V - CAM (A19)",19="3Y - CRANK (A24)",80="4H - Neutral",81="5C - Digital Input",78="5I - Digital Input"
#define brain_input_pin_e_fullenum "NONE","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","3V - CAM (A19)","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","3Y - CRANK (A24)","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","5I - Digital Input","INVALID","4H - Neutral","5C - Digital Input","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","3T - VSS (D5)"
#define brain_pin_diag_e_PIN_DRIVER_OFF 0x20
#define brain_pin_diag_e_PIN_DRIVER_OVERTEMP 0x10
#define brain_pin_diag_e_PIN_OK 0
#define brain_pin_diag_e_PIN_OPEN 0x01
#define brain_pin_diag_e_PIN_OVERLOAD 0x08
#define brain_pin_diag_e_PIN_SHORT_TO_BAT 0x04
#define brain_pin_diag_e_PIN_SHORT_TO_GND 0x02
#define brain_pin_diag_e_PIN_UNKNOWN 0x80
#define C_MC33810_COUNT 1
#define CAM_1_MODE_NAME "Cam mode (intake)"
#define CAM_2_MODE_NAME "Cam mode (exhaust)"
#define CAM_INPUTS_COUNT 4
#define CAMS_PER_BANK 2
#define can_baudrate_e_auto_enum 0="B50KBPS",2="B100KBPS",3="B125KBPS",6="B1MBPS",4="B250KBPS",5="B500KBPS",1="B83KBPS"
#define can_baudrate_e_B100KBPS 2
#define can_baudrate_e_B125KBPS 3
#define can_baudrate_e_B1MBPS 6
#define can_baudrate_e_B250KBPS 4
#define can_baudrate_e_B500KBPS 5
#define can_baudrate_e_B50KBPS 0
#define can_baudrate_e_B83KBPS 1
#define can_baudrate_e_enum "50kbps", "83.33kbps", "100kbps", "125kbps", "250kbps", "500kbps", "1Mbps"
#define CAN_BENCH_HEADER2 0x66
#define CAN_DEFAULT_BASE 0x200
#define CAN_ECU_SERIAL_RX_ID 0x100
#define CAN_ECU_SERIAL_TX_ID 0x102
#define can_nbc_e_auto_enum 0="CAN_BUS_NBC_NONE",12="CAN_AIM_DASH",4="CAN_BUS_BMW_E46",6="CAN_BUS_BMW_E90",10="CAN_BUS_GENESIS_COUPE",11="CAN_BUS_HONDA_K",7="CAN_BUS_Haltech",3="CAN_BUS_MAZDA_RX8",8="CAN_BUS_MQB",13="CAN_BUS_MS_SIMPLE_BROADCAST",1="CAN_BUS_NBC_FIAT",2="CAN_BUS_NBC_VAG",9="CAN_BUS_NISSAN_VQ",5="CAN_BUS_W202_C180"
#define can_nbc_e_CAN_AIM_DASH 12
#define can_nbc_e_CAN_BUS_BMW_E46 4
#define can_nbc_e_CAN_BUS_BMW_E90 6
#define can_nbc_e_CAN_BUS_GENESIS_COUPE 10
#define can_nbc_e_CAN_BUS_Haltech 7
#define can_nbc_e_CAN_BUS_HONDA_K 11
#define can_nbc_e_CAN_BUS_MAZDA_RX8 3
#define can_nbc_e_CAN_BUS_MQB 8
#define can_nbc_e_CAN_BUS_MS_SIMPLE_BROADCAST 13
#define can_nbc_e_CAN_BUS_NBC_FIAT 1
#define can_nbc_e_CAN_BUS_NBC_NONE 0
#define can_nbc_e_CAN_BUS_NBC_VAG 2
#define can_nbc_e_CAN_BUS_NISSAN_VQ 9
#define can_nbc_e_CAN_BUS_W202_C180 5
#define CAN_RX_PREFIX "CAN_rx"
#define can_vss_nbc_e_auto_enum 0="BMW_e46",2="BMW_e90",5="HONDA_CIVIC9",4="HYUNDAI_PB",3="NISSAN_350",1="W202"
#define can_vss_nbc_e_BMW_e46 0
#define can_vss_nbc_e_BMW_e90 2
#define can_vss_nbc_e_HONDA_CIVIC9 5
#define can_vss_nbc_e_HYUNDAI_PB 4
#define can_vss_nbc_e_NISSAN_350 3
#define can_vss_nbc_e_W202 1
#define CanGpioType_auto_enum 0="NONE",1="DRT",2="MS"
#define CanGpioType_DRT 1
#define CanGpioType_enum "None", "DRT protocol", "MS protocol"
#define CanGpioType_MS 2
#define CanGpioType_NONE 0
#define CLT_CRANKING_CURVE_SIZE 8
#define CLT_CURVE_SIZE 16
#define CLT_LIMITER_CURVE_SIZE 4
#define CLT_TIMING_CURVE_SIZE 8
#define CLT_UPPER_LIMIT 250
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
#define CMD_INDIVIDUAL_INJECTION "set_individual_coils_ignition"
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
#define CMD_RESET_SENSOR_MOCKS "reset_sensor_mocks"
#define CMD_RESET_SIMULATOR "reset_simulator"
#define CMD_RPM "rpm"
#define CMD_SELF_STIMULATION "self_stimulation"
#define CMD_SET "set"
#define CMD_SET_SENSOR_MOCK "set_sensor_mock"
#define CMD_SPARK_BENCH "sparkbench"
#define CMD_STARTER_BENCH "starterbench"
#define CMD_TRIGGER_HW_INPUT "trigger_hw_input"
#define CMD_TRIGGER_PIN "set_trigger_input_pin"
#define CMD_TRIGGER_SIMULATOR_PIN "set_trigger_simulator_pin"
#define CMD_TRIGGERINFO "triggerinfo"
#define CMD_VSS_PIN "vss_pin"
#define CMD_WRITECONFIG "writeconfig"
#define COIL_1_NAME "Spark #1"
#define COIL_2_NAME "Spark #2"
#define COMPOSITE_PACKET_SIZE 5
#define CONSOLE_DATA_PROTOCOL_TAG " @"
#define CRANKING_ADVANCE_CURVE_SIZE 4
#define CRANKING_CLT_IDLE_CURVE_SIZE 8
#define CRANKING_CURVE_SIZE 8
#define CRANKING_ENRICH_COUNT 6
#define cranking_parameters_s_size 8
#define CRITICAL_BUFFER_SIZE 120
#define CRITICAL_PREFIX "CRITICAL"
#define DAC_OUTPUT_COUNT 2
#define dc_function_e_auto_enum 0="DC_None",3="DC_IdleValve",1="DC_Throttle1",2="DC_Throttle2",4="DC_Wastegate"
#define dc_function_e_DC_IdleValve 3
#define dc_function_e_DC_None 0
#define dc_function_e_DC_Throttle1 1
#define dc_function_e_DC_Throttle2 2
#define dc_function_e_DC_Wastegate 4
#define dc_io_size 8
#define DC_PER_STEPPER 2
#define debug_mode_e_auto_enum 0="DBG_0",10="DBG_10",11="DBG_11",12="DBG_12",13="DBG_13",16="DBG_16",19="DBG_19",2="DBG_2",20="DBG_20",21="DBG_21",22="DBG_22",24="DBG_24",25="DBG_25",26="DBG_26",29="DBG_29",32="DBG_32",34="DBG_34",35="DBG_35",36="DBG_36",37="DBG_37",38="DBG_38",43="DBG_43",46="DBG_46",5="DBG_5",6="DBG_6",7="DBG_7",8="DBG_8",9="DBG_9",40="DBG_COMPOSITE_LOG",33="DBG_DWELL_METRIC",44="DBG_DYNO_VIEW",17="DBG_ELECTRONIC_THROTTLE_PID",4="DBG_EL_ACCEL",39="DBG_ETB_AUTOTUNE",18="DBG_EXECUTOR",30="DBG_ION",15="DBG_KNOCK",45="DBG_LOGIC_ANALYZER",48="DBG_LUA",27="DBG_MAP",28="DBG_METRICS",14="DBG_SR5_PROTOCOL",3="DBG_STEPPER_IDLE_CONTROL",47="DBG_TCU",31="DBG_TLE8888",1="DBG_TPS_ACCEL",41="DBG_UNUSED41",42="DBG_UNUSED_42",23="UNUSED23"
#define debug_mode_e_DBG_0 0
#define debug_mode_e_DBG_10 10
#define debug_mode_e_DBG_11 11
#define debug_mode_e_DBG_12 12
#define debug_mode_e_DBG_13 13
#define debug_mode_e_DBG_16 16
#define debug_mode_e_DBG_19 19
#define debug_mode_e_DBG_2 2
#define debug_mode_e_DBG_20 20
#define debug_mode_e_DBG_21 21
#define debug_mode_e_DBG_22 22
#define debug_mode_e_DBG_24 24
#define debug_mode_e_DBG_25 25
#define debug_mode_e_DBG_26 26
#define debug_mode_e_DBG_29 29
#define debug_mode_e_DBG_32 32
#define debug_mode_e_DBG_34 34
#define debug_mode_e_DBG_35 35
#define debug_mode_e_DBG_36 36
#define debug_mode_e_DBG_37 37
#define debug_mode_e_DBG_38 38
#define debug_mode_e_DBG_43 43
#define debug_mode_e_DBG_46 46
#define debug_mode_e_DBG_5 5
#define debug_mode_e_DBG_6 6
#define debug_mode_e_DBG_7 7
#define debug_mode_e_DBG_8 8
#define debug_mode_e_DBG_9 9
#define debug_mode_e_DBG_COMPOSITE_LOG 40
#define debug_mode_e_DBG_DWELL_METRIC 33
#define debug_mode_e_DBG_DYNO_VIEW 44
#define debug_mode_e_DBG_EL_ACCEL 4
#define debug_mode_e_DBG_ELECTRONIC_THROTTLE_PID 17
#define debug_mode_e_DBG_ETB_AUTOTUNE 39
#define debug_mode_e_DBG_EXECUTOR 18
#define debug_mode_e_DBG_ION 30
#define debug_mode_e_DBG_KNOCK 15
#define debug_mode_e_DBG_LOGIC_ANALYZER 45
#define debug_mode_e_DBG_LUA 48
#define debug_mode_e_DBG_MAP 27
#define debug_mode_e_DBG_METRICS 28
#define debug_mode_e_DBG_SR5_PROTOCOL 14
#define debug_mode_e_DBG_STEPPER_IDLE_CONTROL 3
#define debug_mode_e_DBG_TCU 47
#define debug_mode_e_DBG_TLE8888 31
#define debug_mode_e_DBG_TPS_ACCEL 1
#define debug_mode_e_DBG_UNUSED41 41
#define debug_mode_e_DBG_UNUSED_42 42
#define debug_mode_e_enum "INVALID", "TPS acceleration enrichment", "INVALID", "Stepper Idle Control", "Engine Load accl enrich", "Trigger Counters", "Soft Spark Cut", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "SD card", "sr5", "Knock", "INVALID", "Electronic Throttle", "Executor", "Bench Test / TS commands", "INVALID", "Analog inputs #1", "INSTANT_RPM", "INVALID", "Status", "INVALID", "INVALID", "MAP", "Metrics", "INVALID", "Ion Sense", "TLE8888", "Analog inputs #2", "Dwell Metric", "INVALID", "INVALID", "Boost Control", "INVALID", "INVALID", "ETB Autotune", "Composite Log", "INVALID", "INVALID", "INVALID", "Dyno_View", "Logic_Analyzer", "INVALID", "TCU", "Lua"
#define debug_mode_e_UNUSED23 23
#define DEFAULT_SELT_STIM_RPM 1200
#define DEFAULT_SELT_STIM_VVT0 23
#define DEFAULT_SOLENOID_FREQUENCY 300
#define DIGIPOT_COUNT 4
#define display_mode_e_DM_HD44780 1
#define display_mode_e_DM_HD44780_OVER_PCF8574 2
#define display_mode_e_DM_NONE 0
#define DWELL_CURVE_SIZE 8
#define ego_sensor_e_auto_enum 0="ES_BPSX_D1",2="ES_14Point7_Free",6="ES_AEM",5="ES_Custom",1="ES_Innovate_MTX_L",4="ES_PLX"
#define ego_sensor_e_enum "BPSX", "Innovate", "14Point7", "INVALID", "PLX", "Custom", "AEM"
#define ego_sensor_e_ES_14Point7_Free 2
#define ego_sensor_e_ES_AEM 6
#define ego_sensor_e_ES_BPSX_D1 0
#define ego_sensor_e_ES_Custom 5
#define ego_sensor_e_ES_Innovate_MTX_L 1
#define ego_sensor_e_ES_PLX 4
#define EGT_CHANNEL_COUNT 8
#define END_OF_CALIBRATION_PADDING 200
#define engine_configuration_s_size 4220
#define engine_load_mode_e_auto_enum 0="LM_SPEED_DENSITY",2="LM_ALPHA_N",3="LM_LUA",100="LM_MOCK",1="LM_REAL_MAF"
#define engine_load_mode_e_enum "Speed Density", "MAF Air Charge", "Alpha-N", "Lua"
#define engine_load_mode_e_LM_ALPHA_N 2
#define engine_load_mode_e_LM_LUA 3
#define engine_load_mode_e_LM_MOCK 100
#define engine_load_mode_e_LM_REAL_MAF 1
#define engine_load_mode_e_LM_SPEED_DENSITY 0
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
#define engine_type_e_auto_enum 0="DEFAULT_FRANKENSO",22="BMW_M52",50="BMW_M73_MRE",51="BMW_M73_MRE_SLAVE",38="DISCOVERY_PDM",2="DODGE_NEON_1995",46="DODGE_NEON_2003_CRANK",64="DODGE_RAM",61="EEPROM_BENCH_ENGINE",58="ETB_BENCH_ENGINE",21="ET_BOSCH_QUICK_START",24="ET_TEST_WITH_BOSCH_QUICK_START",35="ET_UNUSED_35",55="ET_UNUSED_55",105="FERRARI_F136",3="FORD_ASPIRE_1996",11="FORD_COYOTE",14="FORD_ESCORT_GT",7="FORD_INLINE_6_1995",40="FRANKENSO_BMW_M73_F",41="FRANKENSO_MIATA_NA6_MAP",49="FRANKENSO_QA_ENGINE",59="FRANKENSO_TEST_33810",56="FRANKENSO_TEST_TLE9104",15="FUEL_BENCH",98="GM_SBC",8="GY6_139QMB",6="HARLEY",84="HELLEN_121_NISSAN_4_CYL",72="HELLEN_121_NISSAN_6_CYL",85="HELLEN_121_NISSAN_8_CYL",87="HELLEN_121_NISSAN_ALMERA_N16",71="HELLEN_121_VAG_4_CYL",77="HELLEN_121_VAG_5_CYL",81="HELLEN_121_VAG_8_CYL",78="HELLEN_121_VAG_V6_CYL",79="HELLEN_121_VAG_VR6_CYL",36="HELLEN_128_MERCEDES_4_CYL",88="HELLEN_128_MERCEDES_6_CYL",89="HELLEN_128_MERCEDES_8_CYL",82="HELLEN_154_HYUNDAI_COUPE_BK1",95="HELLEN_154_HYUNDAI_COUPE_BK2",76="HELLEN_154_VAG",75="HELLEN_2CHAN_STIM_QC",74="HELLEN_4CHAN_STIM_QC",57="HELLEN_HONDA_BCM",43="HONDA_600",90="HONDA_K",66="HONDA_OBD1",91="HONDA_OBD2A",104="HYUNDAI_PB",60="L9779_BENCH_ENGINE",54="MAVERICK_X3",34="MAZDA_MIATA_NA6",80="MAZDA_MIATA_NA94",10="MAZDA_MIATA_NA96",83="MAZDA_MIATA_NB1",69="MAZDA_MIATA_NB2",86="MAZDA_MIATA_NB2_36",20="MAZDA_MIATA_NC",33="ME17_9_MISC",68="MERCEDES_M111",1="MIATA_PROTEUS_TCU",99="MINIMAL_PINS",12="MITSUBISHI_3A92",16="MITSUBISHI_4G93",31="MRE_BOARD_NEW_TEST",23="MRE_BODY_CONTROL",4="MRE_SECONDARY_CAN",37="MRE_SUBARU_EJ18",62="MRE_VW_B6",5="NISSAN_PRIMERA",9="POLARIS_RZR",30="PROTEUS_ANALOG_PWM_TEST",63="PROTEUS_BMW_M73",27="PROTEUS_GM_LS_4",25="PROTEUS_LUA_DEMO",103="PROTEUS_NISSAN_VQ35",42="PROTEUS_QC_TEST_BOARD",73="PROTEUS_STIM_QC",39="PROTEUS_VW_B6",29="SACHS",92="SIMULATOR_CONFIG",70="SUBARU_EG33",17="TCU_4R70W",100="TEST_100",101="TEST_101",18="TEST_33816",28="TEST_CRANK_ENGINE",48="TEST_DC_WASTEGATE_DISCOVERY",26="TEST_ENGINE",45="TEST_ENGINE_VVT",52="TEST_ISSUE_366_BOTH",53="TEST_ISSUE_366_RISE",19="TEST_ROTARY",13="TOYOTA_1NZ_FE",44="TOYOTA_2JZ_GTE_VVTi",102="UNUSED102",47="UNUSED47",67="UNUSED67",93="UNUSED93",94="UNUSED94",65="UNUSED_65",97="UNUSED_97",32="VW_ABA",96="WASTEGATE_PROTEUS_TEST"
#define engine_type_e_BMW_M52 22
#define engine_type_e_BMW_M73_MRE 50
#define engine_type_e_BMW_M73_MRE_SLAVE 51
#define engine_type_e_DEFAULT_FRANKENSO 0
#define engine_type_e_DISCOVERY_PDM 38
#define engine_type_e_DODGE_NEON_1995 2
#define engine_type_e_DODGE_NEON_2003_CRANK 46
#define engine_type_e_DODGE_RAM 64
#define engine_type_e_EEPROM_BENCH_ENGINE 61
#define engine_type_e_ET_BOSCH_QUICK_START 21
#define engine_type_e_ET_TEST_WITH_BOSCH_QUICK_START 24
#define engine_type_e_ET_UNUSED_35 35
#define engine_type_e_ET_UNUSED_55 55
#define engine_type_e_ETB_BENCH_ENGINE 58
#define engine_type_e_FERRARI_F136 105
#define engine_type_e_FORD_ASPIRE_1996 3
#define engine_type_e_FORD_COYOTE 11
#define engine_type_e_FORD_ESCORT_GT 14
#define engine_type_e_FORD_INLINE_6_1995 7
#define engine_type_e_FRANKENSO_BMW_M73_F 40
#define engine_type_e_FRANKENSO_MIATA_NA6_MAP 41
#define engine_type_e_FRANKENSO_QA_ENGINE 49
#define engine_type_e_FRANKENSO_TEST_33810 59
#define engine_type_e_FRANKENSO_TEST_TLE9104 56
#define engine_type_e_FUEL_BENCH 15
#define engine_type_e_GM_SBC 98
#define engine_type_e_GY6_139QMB 8
#define engine_type_e_HARLEY 6
#define engine_type_e_HELLEN_121_NISSAN_4_CYL 84
#define engine_type_e_HELLEN_121_NISSAN_6_CYL 72
#define engine_type_e_HELLEN_121_NISSAN_8_CYL 85
#define engine_type_e_HELLEN_121_NISSAN_ALMERA_N16 87
#define engine_type_e_HELLEN_121_VAG_4_CYL 71
#define engine_type_e_HELLEN_121_VAG_5_CYL 77
#define engine_type_e_HELLEN_121_VAG_8_CYL 81
#define engine_type_e_HELLEN_121_VAG_V6_CYL 78
#define engine_type_e_HELLEN_121_VAG_VR6_CYL 79
#define engine_type_e_HELLEN_128_MERCEDES_4_CYL 36
#define engine_type_e_HELLEN_128_MERCEDES_6_CYL 88
#define engine_type_e_HELLEN_128_MERCEDES_8_CYL 89
#define engine_type_e_HELLEN_154_HYUNDAI_COUPE_BK1 82
#define engine_type_e_HELLEN_154_HYUNDAI_COUPE_BK2 95
#define engine_type_e_HELLEN_154_VAG 76
#define engine_type_e_HELLEN_2CHAN_STIM_QC 75
#define engine_type_e_HELLEN_4CHAN_STIM_QC 74
#define engine_type_e_HELLEN_HONDA_BCM 57
#define engine_type_e_HONDA_600 43
#define engine_type_e_HONDA_K 90
#define engine_type_e_HONDA_OBD1 66
#define engine_type_e_HONDA_OBD2A 91
#define engine_type_e_HYUNDAI_PB 104
#define engine_type_e_L9779_BENCH_ENGINE 60
#define engine_type_e_MAVERICK_X3 54
#define engine_type_e_MAZDA_MIATA_NA6 34
#define engine_type_e_MAZDA_MIATA_NA94 80
#define engine_type_e_MAZDA_MIATA_NA96 10
#define engine_type_e_MAZDA_MIATA_NB1 83
#define engine_type_e_MAZDA_MIATA_NB2 69
#define engine_type_e_MAZDA_MIATA_NB2_36 86
#define engine_type_e_MAZDA_MIATA_NC 20
#define engine_type_e_ME17_9_MISC 33
#define engine_type_e_MERCEDES_M111 68
#define engine_type_e_MIATA_PROTEUS_TCU 1
#define engine_type_e_MINIMAL_PINS 99
#define engine_type_e_MITSUBISHI_3A92 12
#define engine_type_e_MITSUBISHI_4G93 16
#define engine_type_e_MRE_BOARD_NEW_TEST 31
#define engine_type_e_MRE_BODY_CONTROL 23
#define engine_type_e_MRE_SECONDARY_CAN 4
#define engine_type_e_MRE_SUBARU_EJ18 37
#define engine_type_e_MRE_VW_B6 62
#define engine_type_e_NISSAN_PRIMERA 5
#define engine_type_e_POLARIS_RZR 9
#define engine_type_e_PROTEUS_ANALOG_PWM_TEST 30
#define engine_type_e_PROTEUS_BMW_M73 63
#define engine_type_e_PROTEUS_GM_LS_4 27
#define engine_type_e_PROTEUS_LUA_DEMO 25
#define engine_type_e_PROTEUS_NISSAN_VQ35 103
#define engine_type_e_PROTEUS_QC_TEST_BOARD 42
#define engine_type_e_PROTEUS_STIM_QC 73
#define engine_type_e_PROTEUS_VW_B6 39
#define engine_type_e_SACHS 29
#define engine_type_e_SIMULATOR_CONFIG 92
#define engine_type_e_SUBARU_EG33 70
#define engine_type_e_TCU_4R70W 17
#define engine_type_e_TEST_100 100
#define engine_type_e_TEST_101 101
#define engine_type_e_TEST_33816 18
#define engine_type_e_TEST_CRANK_ENGINE 28
#define engine_type_e_TEST_DC_WASTEGATE_DISCOVERY 48
#define engine_type_e_TEST_ENGINE 26
#define engine_type_e_TEST_ENGINE_VVT 45
#define engine_type_e_TEST_ISSUE_366_BOTH 52
#define engine_type_e_TEST_ISSUE_366_RISE 53
#define engine_type_e_TEST_ROTARY 19
#define engine_type_e_TOYOTA_1NZ_FE 13
#define engine_type_e_TOYOTA_2JZ_GTE_VVTi 44
#define engine_type_e_UNUSED102 102
#define engine_type_e_UNUSED47 47
#define engine_type_e_UNUSED67 67
#define engine_type_e_UNUSED93 93
#define engine_type_e_UNUSED94 94
#define engine_type_e_UNUSED_65 65
#define engine_type_e_UNUSED_97 97
#define engine_type_e_VW_ABA 32
#define engine_type_e_WASTEGATE_PROTEUS_TEST 96
#define engineSyncCam_enum "Intake First Bank", "Exhaust First Bank", "Intake Second Bank", "Exhaust Second Bank"
#define ETB2_TRIM_SIZE 6
#define ETB_BIAS_CURVE_LENGTH 8
#define ETB_COUNT 2
#define ETB_HW_MAX_FREQUENCY 3000
#define FANCY_OPTIONS_COUNT 16
#define FLASH_DATA_VERSION 20016
#define fuel_cyl_trim_s_size 16
#define FUEL_LEVEL_TABLE_COUNT 8
#define FUEL_LOAD_COUNT 16
#define fuel_pressure_sensor_mode_e_auto_enum 0="FPM_Absolute",2="FPM_Differential",1="FPM_Gauge"
#define fuel_pressure_sensor_mode_e_enum "Absolute", "Gauge", "Differential", "INVALID"
#define fuel_pressure_sensor_mode_e_FPM_Absolute 0
#define fuel_pressure_sensor_mode_e_FPM_Differential 2
#define fuel_pressure_sensor_mode_e_FPM_Gauge 1
#define FUEL_RPM_COUNT 16
#define FUEL_TRIM_SIZE 4
#define GAP_TRACKING_LENGTH 18
#define GAUGE_CATEGORY_BOOST_CONTROL "Boost Control"
#define GAUGE_CATEGORY_ETB "ETB more"
#define GAUGE_CATEGORY_FUEL_MATH "Fuel: math"
#define GAUGE_CATEGORY_SYNC "Sync"
#define GAUGE_CATEGORY_TIMING "Timing"
#define GAUGE_COIL_DWELL_TIME "Ignition: coil charge time"
#define GAUGE_LUA_FROM_0 -30000
#define GAUGE_LUA_FROM_1 -30000
#define GAUGE_LUA_FROM_2 -30000
#define GAUGE_LUA_FROM_3 -30000
#define GAUGE_LUA_FROM_4 -30000
#define GAUGE_LUA_FROM_5 -30000
#define GAUGE_LUA_FROM_6 -30000
#define GAUGE_LUA_FROM_7 -30000
#define GAUGE_LUA_TO_0 30000
#define GAUGE_LUA_TO_1 30000
#define GAUGE_LUA_TO_2 30000
#define GAUGE_LUA_TO_3 30000
#define GAUGE_LUA_TO_4 30000
#define GAUGE_LUA_TO_5 30000
#define GAUGE_LUA_TO_6 30000
#define GAUGE_LUA_TO_7 30000
#define GAUGE_NAME_ACCEL_LAT "Accel: Lateral"
#define GAUGE_NAME_ACCEL_LON "Accel: Longitudinal"
#define GAUGE_NAME_ACCEL_ROLL "Acceleration: Roll"
#define GAUGE_NAME_ACCEL_VERT "Accel: Vertical"
#define GAUGE_NAME_ACCEL_YAW "Acceleration: Yaw"
#define GAUGE_NAME_ADJUSTED_TIMING "Timing: ignition"
#define GAUGE_NAME_AFR "Air/Fuel Ratio"
#define GAUGE_NAME_AFR2 "Air/Fuel Ratio 2"
#define GAUGE_NAME_AFR2_GAS_SCALE "Air/Fuel Ratio 2 (Gas Scale)"
#define GAUGE_NAME_AFR_GAS_SCALE "Air/Fuel Ratio (Gas Scale)"
#define GAUGE_NAME_AIR_FLOW_ESTIMATE "Air: Flow estimate"
#define GAUGE_NAME_AIR_FLOW_MEASURED "MAF"
#define GAUGE_NAME_AIR_FLOW_MEASURED_2 "MAF #2"
#define GAUGE_NAME_AIR_MASS "Air: Cylinder airmass"
#define GAUGE_NAME_AUX_LINEAR_1 "Aux linear #1"
#define GAUGE_NAME_AUX_LINEAR_2 "Aux linear #2"
#define GAUGE_NAME_AUX_TEMP1 "Aux temp 1"
#define GAUGE_NAME_AUX_TEMP2 "Aux temp 2"
#define GAUGE_NAME_BARO_PRESSURE "Barometric pressure"
#define GAUGE_NAME_BOOST_CLOSED_LOOP "Boost: Closed loop"
#define GAUGE_NAME_BOOST_OPEN_LOOP "Boost: Open loop"
#define GAUGE_NAME_BOOST_OUTPUT "Boost: Output"
#define GAUGE_NAME_BOOST_TARGET "Boost: Target"
#define GAUGE_NAME_CAN_READ_OK "CAN: Rx"
#define GAUGE_NAME_CAN_WRITE_ERR "CAN: Tx err"
#define GAUGE_NAME_CAN_WRITE_OK "CAN: Tx OK"
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
#define GAUGE_NAME_DESIRED_GEAR "TCU: Desired Gear"
#define GAUGE_NAME_DETECTED_GEAR "Detected Gear"
#define GAUGE_NAME_DWELL_DUTY "Ignition: coil duty cycle"
#define GAUGE_NAME_ECU_TEMPERATURE "ECU temperature"
#define GAUGE_NAME_ENGINE_CRC16 "Engine CRC16"
#define GAUGE_NAME_ETB_DUTY "ETB: Duty"
#define GAUGE_NAME_ETB_ERROR "ETB: position error"
#define GAUGE_NAME_ETB_TARGET "ETB: position target"
#define GAUGE_NAME_FLEX "Flex Ethanol %"
#define GAUGE_NAME_FUEL_BARO_CORR "Fuel: Barometric pressure mult"
#define GAUGE_NAME_FUEL_BASE "Fuel: base cycle mass"
#define GAUGE_NAME_FUEL_CHARGE_TEMP "Fuel: Estimated charge temperature"
#define GAUGE_NAME_FUEL_CLT_CORR "Fuel: CLT correction"
#define GAUGE_NAME_FUEL_CONSUMPTION "Fuel: Total consumed"
#define GAUGE_NAME_FUEL_CRANKING "Fuel: cranking"
#define GAUGE_NAME_FUEL_FLOW "Fuel: Flow rate"
#define GAUGE_NAME_FUEL_IAT_CORR "Fuel: IAT correction"
#define GAUGE_NAME_FUEL_INJ_DUTY "Fuel: injector duty cycle"
#define GAUGE_NAME_FUEL_INJ_DUTY_STAGE_2 "Fuel: injector duty cycle stage 2"
#define GAUGE_NAME_FUEL_INJECTION_TIMING "Fuel: Injection timing SOI"
#define GAUGE_NAME_FUEL_LAST_INJECTION "Fuel: Last inj pulse width"
#define GAUGE_NAME_FUEL_LAST_INJECTION_STAGE_2 "Fuel: Last inj pulse width stg 2"
#define GAUGE_NAME_FUEL_LEVEL "Fuel level"
#define GAUGE_NAME_FUEL_LOAD "Fuel: Load"
#define GAUGE_NAME_FUEL_PRESSURE_HIGH "Fuel pressure (high)"
#define GAUGE_NAME_FUEL_PRESSURE_HIGH_UNITS "bar"
#define GAUGE_NAME_FUEL_PRESSURE_LOW "Fuel pressure (low)"
#define GAUGE_NAME_FUEL_PRESSURE_LOW_UNITS "kPa"
#define GAUGE_NAME_FUEL_RUNNING "Fuel: running"
#define GAUGE_NAME_FUEL_TEMPERATURE "Fuel Temperature"
#define GAUGE_NAME_FUEL_TPS_EXTRA "Fuel: TPS AE add fuel ms"
#define GAUGE_NAME_FUEL_TPS_ROC "Fuel: TPS change"
#define GAUGE_NAME_FUEL_TRIM "Fuel: fuel trim"
#define GAUGE_NAME_FUEL_TRIM_2 "Fuel: fuel trim 2"
#define GAUGE_NAME_FUEL_VE "Fuel: VE"
#define GAUGE_NAME_FUEL_WALL_AMOUNT "Fuel: wall amount"
#define GAUGE_NAME_FUEL_WALL_CORRECTION "Fuel: wall correction"
#define GAUGE_NAME_FW_VERSION "ECU Software Version"
#define GAUGE_NAME_GEAR_RATIO "Gearbox Ratio"
#define GAUGE_NAME_GYRO_YAW "Gyro: Yaw rate"
#define GAUGE_NAME_IAC "Idle: Position"
#define GAUGE_NAME_IAT "IAT"
#define GAUGE_NAME_IDLE_POSITION "Idle: Position sensor"
#define GAUGE_NAME_IGNITION_LOAD "Ignition: load"
#define GAUGE_NAME_IGNITION_MODE "Ignition: Mode"
#define GAUGE_NAME_INJECTION_MODE "Fuel: Injection mode"
#define GAUGE_NAME_INJECTOR_LAG "Fuel: injector lag"
#define GAUGE_NAME_ISS "TCU: Input Shaft Speed"
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
#define GAUGE_NAME_KNOCK_COUNTER "Knock: Count"
#define GAUGE_NAME_KNOCK_FREQ "knock: frequency"
#define GAUGE_NAME_KNOCK_LEVEL "Knock: Current level"
#define GAUGE_NAME_KNOCK_RETARD "Knock: Retard"
#define GAUGE_NAME_LAMBDA "Lambda"
#define GAUGE_NAME_LAMBDA2 "Lambda 2"
#define GAUGE_NAME_LAST_ERROR "Warning: last"
#define GAUGE_NAME_LUA_0 "Lua Gauge 1"
#define GAUGE_NAME_LUA_1 "Lua Gauge 2"
#define GAUGE_NAME_LUA_2 "Lua Gauge 3"
#define GAUGE_NAME_LUA_3 "Lua Gauge 4"
#define GAUGE_NAME_LUA_4 "Lua Gauge 5"
#define GAUGE_NAME_LUA_5 "Lua Gauge 6"
#define GAUGE_NAME_LUA_6 "Lua Gauge 7"
#define GAUGE_NAME_LUA_7 "Lua Gauge 8"
#define GAUGE_NAME_MAF "MAF"
#define GAUGE_NAME_MAP "MAP"
#define GAUGE_NAME_OIL_PRESSURE "Oil Pressure"
#define GAUGE_NAME_OIL_PRESSURE_UNITS "kPa"
#define GAUGE_NAME_RAW_FUEL_PRESSURE_HIGH "Fuel pressure raw (high)"
#define GAUGE_NAME_RAW_FUEL_PRESSURE_LOW "Fuel pressure raw (low)"
#define GAUGE_NAME_RPM "RPM"
#define GAUGE_NAME_TARGET_AFR "Fuel: target AFR"
#define GAUGE_NAME_TARGET_LAMBDA "Fuel: target lambda"
#define GAUGE_NAME_TC_RATIO "TCU: Torque Converter Ratio"
#define GAUGE_NAME_TCHARGE "Air: SD tCharge"
#define GAUGE_NAME_THROTTLE_PEDAL "Throttle pedal position"
#define GAUGE_NAME_TIME "Time"
#define GAUGE_NAME_TIMING_ADVANCE "Ignition: Timing"
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
#define GAUGE_NAME_WARNING_COUNTER "Warning: counter"
#define GAUGE_NAME_WARNING_LAST "Warning: last"
#define GAUGE_NAME_WG_POSITION "Wastegate position sensor"
#define gear_controller_e_enum "None", "Button Shift", "Automatic", "Generic"
#define gear_e_GEAR_1 1
#define gear_e_GEAR_2 2
#define gear_e_GEAR_3 3
#define gear_e_GEAR_4 4
#define gear_e_NEUTRAL 0
#define gear_e_REVERSE -1
#define GearControllerMode_auto_enum 0="None",2="Automatic",1="ButtonShift",3="Generic"
#define GearControllerMode_Automatic 2
#define GearControllerMode_ButtonShift 1
#define GearControllerMode_Generic 3
#define GearControllerMode_None 0
#define Gpio_A0 2
#define Gpio_A1 3
#define Gpio_A10 12
#define Gpio_A11 13
#define Gpio_A12 14
#define Gpio_A13 15
#define Gpio_A14 16
#define Gpio_A15 17
#define Gpio_A2 4
#define Gpio_A3 5
#define Gpio_A4 6
#define Gpio_A5 7
#define Gpio_A6 8
#define Gpio_A7 9
#define Gpio_A8 10
#define Gpio_A9 11
#define Gpio_auto_enum 0="Unassigned",2="A0",3="A1",12="A10",13="A11",14="A12",15="A13",16="A14",17="A15",4="A2",5="A3",6="A4",7="A5",8="A6",9="A7",10="A8",11="A9",18="B0",19="B1",28="B10",29="B11",30="B12",31="B13",32="B14",33="B15",20="B2",21="B3",22="B4",23="B5",24="B6",25="B7",26="B8",27="B9",34="C0",35="C1",44="C10",45="C11",46="C12",47="C13",48="C14",49="C15",36="C2",37="C3",38="C4",39="C5",40="C6",41="C7",42="C8",43="C9",249="CAN_PIN_0",250="CAN_PIN_1",251="CAN_PIN_2",252="CAN_PIN_3",253="CAN_PIN_4",254="CAN_PIN_5",255="CAN_PIN_6",256="CAN_PIN_7",50="D0",51="D1",60="D10",61="D11",62="D12",63="D13",64="D14",65="D15",52="D2",53="D3",54="D4",55="D5",56="D6",57="D7",58="D8",59="D9",66="E0",67="E1",76="E10",77="E11",78="E12",79="E13",80="E14",81="E15",68="E2",69="E3",70="E4",71="E5",72="E6",73="E7",74="E8",75="E9",82="F0",83="F1",92="F10",93="F11",94="F12",95="F13",96="F14",97="F15",84="F2",85="F3",86="F4",87="F5",88="F6",89="F7",90="F8",91="F9",98="G0",99="G1",108="G10",109="G11",110="G12",111="G13",112="G14",113="G15",100="G2",101="G3",102="G4",103="G5",104="G6",105="G7",106="G8",107="G9",114="H0",115="H1",124="H10",125="H11",126="H12",127="H13",128="H14",129="H15",116="H2",117="H3",118="H4",119="H5",120="H6",121="H7",122="H8",123="H9",130="I0",131="I1",140="I10",141="I11",142="I12",143="I13",144="I14",145="I15",132="I2",133="I3",134="I4",135="I5",136="I6",137="I7",138="I8",139="I9",1="Invalid",215="L9779_IGN_1",216="L9779_IGN_2",217="L9779_IGN_3",218="L9779_IGN_4",219="L9779_OUT_1",228="L9779_OUT_10",229="L9779_OUT_11",230="L9779_OUT_12",231="L9779_OUT_13",232="L9779_OUT_14",233="L9779_OUT_15",234="L9779_OUT_16",235="L9779_OUT_17",236="L9779_OUT_18",237="L9779_OUT_19",220="L9779_OUT_2",238="L9779_OUT_20",243="L9779_OUT_25",244="L9779_OUT_26",245="L9779_OUT_27",246="L9779_OUT_28",221="L9779_OUT_3",222="L9779_OUT_4",223="L9779_OUT_5",224="L9779_OUT_6",225="L9779_OUT_7",226="L9779_OUT_8",227="L9779_OUT_9",239="L9779_OUT_A",240="L9779_OUT_B",241="L9779_OUT_C",242="L9779_OUT_D",247="L9779_OUT_MRD",248="L9779_PIN_KEY",284="MC33810_0_GD_0",285="MC33810_0_GD_1",286="MC33810_0_GD_2",287="MC33810_0_GD_3",280="MC33810_0_OUT_0",281="MC33810_0_OUT_1",282="MC33810_0_OUT_2",283="MC33810_0_OUT_3",294="MC33810_1_GD_0",295="MC33810_1_GD_1",296="MC33810_1_GD_2",297="MC33810_1_GD_3",290="MC33810_1_OUT_0",291="MC33810_1_OUT_1",292="MC33810_1_OUT_2",293="MC33810_1_OUT_3",146="MC33972_PIN_1",155="MC33972_PIN_10",156="MC33972_PIN_11",157="MC33972_PIN_12",158="MC33972_PIN_13",159="MC33972_PIN_14",160="MC33972_PIN_15",161="MC33972_PIN_16",162="MC33972_PIN_17",163="MC33972_PIN_18",164="MC33972_PIN_19",147="MC33972_PIN_2",165="MC33972_PIN_20",166="MC33972_PIN_21",167="MC33972_PIN_22",148="MC33972_PIN_3",149="MC33972_PIN_4",150="MC33972_PIN_5",151="MC33972_PIN_6",152="MC33972_PIN_7",153="MC33972_PIN_8",154="MC33972_PIN_9",324="MSIOBOX_0_OUT_1",325="MSIOBOX_0_OUT_2",326="MSIOBOX_0_OUT_3",327="MSIOBOX_0_OUT_4",328="MSIOBOX_0_OUT_5",329="MSIOBOX_0_OUT_6",330="MSIOBOX_0_OUT_7",336="MSIOBOX_0_SW_1",337="MSIOBOX_0_SW_2",338="MSIOBOX_0_SW_3",332="MSIOBOX_0_VSS_1",333="MSIOBOX_0_VSS_2",334="MSIOBOX_0_VSS_3",335="MSIOBOX_0_VSS_4",257="PROTECTED_PIN_0",258="PROTECTED_PIN_1",267="PROTECTED_PIN_10",268="PROTECTED_PIN_11",269="PROTECTED_PIN_12",270="PROTECTED_PIN_13",271="PROTECTED_PIN_14",272="PROTECTED_PIN_15",259="PROTECTED_PIN_2",260="PROTECTED_PIN_3",261="PROTECTED_PIN_4",262="PROTECTED_PIN_5",263="PROTECTED_PIN_6",264="PROTECTED_PIN_7",265="PROTECTED_PIN_8",266="PROTECTED_PIN_9",199="TLE6240_PIN_1",208="TLE6240_PIN_10",209="TLE6240_PIN_11",210="TLE6240_PIN_12",211="TLE6240_PIN_13",212="TLE6240_PIN_14",213="TLE6240_PIN_15",214="TLE6240_PIN_16",200="TLE6240_PIN_2",201="TLE6240_PIN_3",202="TLE6240_PIN_4",203="TLE6240_PIN_5",204="TLE6240_PIN_6",205="TLE6240_PIN_7",206="TLE6240_PIN_8",207="TLE6240_PIN_9",168="TLE8888_PIN_1",177="TLE8888_PIN_10",178="TLE8888_PIN_11",179="TLE8888_PIN_12",180="TLE8888_PIN_13",181="TLE8888_PIN_14",182="TLE8888_PIN_15",183="TLE8888_PIN_16",184="TLE8888_PIN_17",185="TLE8888_PIN_18",186="TLE8888_PIN_19",169="TLE8888_PIN_2",187="TLE8888_PIN_20",188="TLE8888_PIN_21",189="TLE8888_PIN_22",190="TLE8888_PIN_23",191="TLE8888_PIN_24",192="TLE8888_PIN_25",193="TLE8888_PIN_26",194="TLE8888_PIN_27",195="TLE8888_PIN_28",170="TLE8888_PIN_3",171="TLE8888_PIN_4",172="TLE8888_PIN_5",173="TLE8888_PIN_6",174="TLE8888_PIN_7",175="TLE8888_PIN_8",176="TLE8888_PIN_9",197="TLE8888_PIN_KEY",196="TLE8888_PIN_MR",198="TLE8888_PIN_WAKE",300="TLE9104_0_OUT_0",301="TLE9104_0_OUT_1",302="TLE9104_0_OUT_2",303="TLE9104_0_OUT_3",304="TLE9104_1_OUT_0",305="TLE9104_1_OUT_1",306="TLE9104_1_OUT_2",307="TLE9104_1_OUT_3",308="TLE9104_2_OUT_0",309="TLE9104_2_OUT_1",310="TLE9104_2_OUT_2",311="TLE9104_2_OUT_3",312="TLE9104_3_OUT_0",313="TLE9104_3_OUT_1",314="TLE9104_3_OUT_2",315="TLE9104_3_OUT_3",316="TLE9104_4_OUT_0",317="TLE9104_4_OUT_1",318="TLE9104_4_OUT_2",319="TLE9104_4_OUT_3",320="TLE9104_5_OUT_0",321="TLE9104_5_OUT_1",322="TLE9104_5_OUT_2",323="TLE9104_5_OUT_3"
#define Gpio_B0 18
#define Gpio_B1 19
#define Gpio_B10 28
#define Gpio_B11 29
#define Gpio_B12 30
#define Gpio_B13 31
#define Gpio_B14 32
#define Gpio_B15 33
#define Gpio_B2 20
#define Gpio_B3 21
#define Gpio_B4 22
#define Gpio_B5 23
#define Gpio_B6 24
#define Gpio_B7 25
#define Gpio_B8 26
#define Gpio_B9 27
#define Gpio_C0 34
#define Gpio_C1 35
#define Gpio_C10 44
#define Gpio_C11 45
#define Gpio_C12 46
#define Gpio_C13 47
#define Gpio_C14 48
#define Gpio_C15 49
#define Gpio_C2 36
#define Gpio_C3 37
#define Gpio_C4 38
#define Gpio_C5 39
#define Gpio_C6 40
#define Gpio_C7 41
#define Gpio_C8 42
#define Gpio_C9 43
#define Gpio_CAN_PIN_0 249
#define Gpio_CAN_PIN_1 250
#define Gpio_CAN_PIN_2 251
#define Gpio_CAN_PIN_3 252
#define Gpio_CAN_PIN_4 253
#define Gpio_CAN_PIN_5 254
#define Gpio_CAN_PIN_6 255
#define Gpio_CAN_PIN_7 256
#define Gpio_D0 50
#define Gpio_D1 51
#define Gpio_D10 60
#define Gpio_D11 61
#define Gpio_D12 62
#define Gpio_D13 63
#define Gpio_D14 64
#define Gpio_D15 65
#define Gpio_D2 52
#define Gpio_D3 53
#define Gpio_D4 54
#define Gpio_D5 55
#define Gpio_D6 56
#define Gpio_D7 57
#define Gpio_D8 58
#define Gpio_D9 59
#define Gpio_E0 66
#define Gpio_E1 67
#define Gpio_E10 76
#define Gpio_E11 77
#define Gpio_E12 78
#define Gpio_E13 79
#define Gpio_E14 80
#define Gpio_E15 81
#define Gpio_E2 68
#define Gpio_E3 69
#define Gpio_E4 70
#define Gpio_E5 71
#define Gpio_E6 72
#define Gpio_E7 73
#define Gpio_E8 74
#define Gpio_E9 75
#define Gpio_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15"
#define Gpio_F0 82
#define Gpio_F1 83
#define Gpio_F10 92
#define Gpio_F11 93
#define Gpio_F12 94
#define Gpio_F13 95
#define Gpio_F14 96
#define Gpio_F15 97
#define Gpio_F2 84
#define Gpio_F3 85
#define Gpio_F4 86
#define Gpio_F5 87
#define Gpio_F6 88
#define Gpio_F7 89
#define Gpio_F8 90
#define Gpio_F9 91
#define Gpio_G0 98
#define Gpio_G1 99
#define Gpio_G10 108
#define Gpio_G11 109
#define Gpio_G12 110
#define Gpio_G13 111
#define Gpio_G14 112
#define Gpio_G15 113
#define Gpio_G2 100
#define Gpio_G3 101
#define Gpio_G4 102
#define Gpio_G5 103
#define Gpio_G6 104
#define Gpio_G7 105
#define Gpio_G8 106
#define Gpio_G9 107
#define Gpio_H0 114
#define Gpio_H1 115
#define Gpio_H10 124
#define Gpio_H11 125
#define Gpio_H12 126
#define Gpio_H13 127
#define Gpio_H14 128
#define Gpio_H15 129
#define Gpio_H2 116
#define Gpio_H3 117
#define Gpio_H4 118
#define Gpio_H5 119
#define Gpio_H6 120
#define Gpio_H7 121
#define Gpio_H8 122
#define Gpio_H9 123
#define Gpio_I0 130
#define Gpio_I1 131
#define Gpio_I10 140
#define Gpio_I11 141
#define Gpio_I12 142
#define Gpio_I13 143
#define Gpio_I14 144
#define Gpio_I15 145
#define Gpio_I2 132
#define Gpio_I3 133
#define Gpio_I4 134
#define Gpio_I5 135
#define Gpio_I6 136
#define Gpio_I7 137
#define Gpio_I8 138
#define Gpio_I9 139
#define Gpio_Invalid 1
#define Gpio_L9779_IGN_1 215
#define Gpio_L9779_IGN_2 216
#define Gpio_L9779_IGN_3 217
#define Gpio_L9779_IGN_4 218
#define Gpio_L9779_OUT_1 219
#define Gpio_L9779_OUT_10 228
#define Gpio_L9779_OUT_11 229
#define Gpio_L9779_OUT_12 230
#define Gpio_L9779_OUT_13 231
#define Gpio_L9779_OUT_14 232
#define Gpio_L9779_OUT_15 233
#define Gpio_L9779_OUT_16 234
#define Gpio_L9779_OUT_17 235
#define Gpio_L9779_OUT_18 236
#define Gpio_L9779_OUT_19 237
#define Gpio_L9779_OUT_2 220
#define Gpio_L9779_OUT_20 238
#define Gpio_L9779_OUT_25 243
#define Gpio_L9779_OUT_26 244
#define Gpio_L9779_OUT_27 245
#define Gpio_L9779_OUT_28 246
#define Gpio_L9779_OUT_3 221
#define Gpio_L9779_OUT_4 222
#define Gpio_L9779_OUT_5 223
#define Gpio_L9779_OUT_6 224
#define Gpio_L9779_OUT_7 225
#define Gpio_L9779_OUT_8 226
#define Gpio_L9779_OUT_9 227
#define Gpio_L9779_OUT_A 239
#define Gpio_L9779_OUT_B 240
#define Gpio_L9779_OUT_C 241
#define Gpio_L9779_OUT_D 242
#define Gpio_L9779_OUT_MRD 247
#define Gpio_L9779_PIN_KEY 248
#define Gpio_MC33810_0_GD_0 284
#define Gpio_MC33810_0_GD_1 285
#define Gpio_MC33810_0_GD_2 286
#define Gpio_MC33810_0_GD_3 287
#define Gpio_MC33810_0_OUT_0 280
#define Gpio_MC33810_0_OUT_1 281
#define Gpio_MC33810_0_OUT_2 282
#define Gpio_MC33810_0_OUT_3 283
#define Gpio_MC33810_1_GD_0 294
#define Gpio_MC33810_1_GD_1 295
#define Gpio_MC33810_1_GD_2 296
#define Gpio_MC33810_1_GD_3 297
#define Gpio_MC33810_1_OUT_0 290
#define Gpio_MC33810_1_OUT_1 291
#define Gpio_MC33810_1_OUT_2 292
#define Gpio_MC33810_1_OUT_3 293
#define Gpio_MC33972_PIN_1 146
#define Gpio_MC33972_PIN_10 155
#define Gpio_MC33972_PIN_11 156
#define Gpio_MC33972_PIN_12 157
#define Gpio_MC33972_PIN_13 158
#define Gpio_MC33972_PIN_14 159
#define Gpio_MC33972_PIN_15 160
#define Gpio_MC33972_PIN_16 161
#define Gpio_MC33972_PIN_17 162
#define Gpio_MC33972_PIN_18 163
#define Gpio_MC33972_PIN_19 164
#define Gpio_MC33972_PIN_2 147
#define Gpio_MC33972_PIN_20 165
#define Gpio_MC33972_PIN_21 166
#define Gpio_MC33972_PIN_22 167
#define Gpio_MC33972_PIN_3 148
#define Gpio_MC33972_PIN_4 149
#define Gpio_MC33972_PIN_5 150
#define Gpio_MC33972_PIN_6 151
#define Gpio_MC33972_PIN_7 152
#define Gpio_MC33972_PIN_8 153
#define Gpio_MC33972_PIN_9 154
#define Gpio_MSIOBOX_0_OUT_1 324
#define Gpio_MSIOBOX_0_OUT_2 325
#define Gpio_MSIOBOX_0_OUT_3 326
#define Gpio_MSIOBOX_0_OUT_4 327
#define Gpio_MSIOBOX_0_OUT_5 328
#define Gpio_MSIOBOX_0_OUT_6 329
#define Gpio_MSIOBOX_0_OUT_7 330
#define Gpio_MSIOBOX_0_SW_1 336
#define Gpio_MSIOBOX_0_SW_2 337
#define Gpio_MSIOBOX_0_SW_3 338
#define Gpio_MSIOBOX_0_VSS_1 332
#define Gpio_MSIOBOX_0_VSS_2 333
#define Gpio_MSIOBOX_0_VSS_3 334
#define Gpio_MSIOBOX_0_VSS_4 335
#define Gpio_PROTECTED_PIN_0 257
#define Gpio_PROTECTED_PIN_1 258
#define Gpio_PROTECTED_PIN_10 267
#define Gpio_PROTECTED_PIN_11 268
#define Gpio_PROTECTED_PIN_12 269
#define Gpio_PROTECTED_PIN_13 270
#define Gpio_PROTECTED_PIN_14 271
#define Gpio_PROTECTED_PIN_15 272
#define Gpio_PROTECTED_PIN_2 259
#define Gpio_PROTECTED_PIN_3 260
#define Gpio_PROTECTED_PIN_4 261
#define Gpio_PROTECTED_PIN_5 262
#define Gpio_PROTECTED_PIN_6 263
#define Gpio_PROTECTED_PIN_7 264
#define Gpio_PROTECTED_PIN_8 265
#define Gpio_PROTECTED_PIN_9 266
#define Gpio_TLE6240_PIN_1 199
#define Gpio_TLE6240_PIN_10 208
#define Gpio_TLE6240_PIN_11 209
#define Gpio_TLE6240_PIN_12 210
#define Gpio_TLE6240_PIN_13 211
#define Gpio_TLE6240_PIN_14 212
#define Gpio_TLE6240_PIN_15 213
#define Gpio_TLE6240_PIN_16 214
#define Gpio_TLE6240_PIN_2 200
#define Gpio_TLE6240_PIN_3 201
#define Gpio_TLE6240_PIN_4 202
#define Gpio_TLE6240_PIN_5 203
#define Gpio_TLE6240_PIN_6 204
#define Gpio_TLE6240_PIN_7 205
#define Gpio_TLE6240_PIN_8 206
#define Gpio_TLE6240_PIN_9 207
#define Gpio_TLE8888_PIN_1 168
#define Gpio_TLE8888_PIN_10 177
#define Gpio_TLE8888_PIN_11 178
#define Gpio_TLE8888_PIN_12 179
#define Gpio_TLE8888_PIN_13 180
#define Gpio_TLE8888_PIN_14 181
#define Gpio_TLE8888_PIN_15 182
#define Gpio_TLE8888_PIN_16 183
#define Gpio_TLE8888_PIN_17 184
#define Gpio_TLE8888_PIN_18 185
#define Gpio_TLE8888_PIN_19 186
#define Gpio_TLE8888_PIN_2 169
#define Gpio_TLE8888_PIN_20 187
#define Gpio_TLE8888_PIN_21 188
#define Gpio_TLE8888_PIN_22 189
#define Gpio_TLE8888_PIN_23 190
#define Gpio_TLE8888_PIN_24 191
#define Gpio_TLE8888_PIN_25 192
#define Gpio_TLE8888_PIN_26 193
#define Gpio_TLE8888_PIN_27 194
#define Gpio_TLE8888_PIN_28 195
#define Gpio_TLE8888_PIN_3 170
#define Gpio_TLE8888_PIN_4 171
#define Gpio_TLE8888_PIN_5 172
#define Gpio_TLE8888_PIN_6 173
#define Gpio_TLE8888_PIN_7 174
#define Gpio_TLE8888_PIN_8 175
#define Gpio_TLE8888_PIN_9 176
#define Gpio_TLE8888_PIN_KEY 197
#define Gpio_TLE8888_PIN_MR 196
#define Gpio_TLE8888_PIN_WAKE 198
#define Gpio_TLE9104_0_OUT_0 300
#define Gpio_TLE9104_0_OUT_1 301
#define Gpio_TLE9104_0_OUT_2 302
#define Gpio_TLE9104_0_OUT_3 303
#define Gpio_TLE9104_1_OUT_0 304
#define Gpio_TLE9104_1_OUT_1 305
#define Gpio_TLE9104_1_OUT_2 306
#define Gpio_TLE9104_1_OUT_3 307
#define Gpio_TLE9104_2_OUT_0 308
#define Gpio_TLE9104_2_OUT_1 309
#define Gpio_TLE9104_2_OUT_2 310
#define Gpio_TLE9104_2_OUT_3 311
#define Gpio_TLE9104_3_OUT_0 312
#define Gpio_TLE9104_3_OUT_1 313
#define Gpio_TLE9104_3_OUT_2 314
#define Gpio_TLE9104_3_OUT_3 315
#define Gpio_TLE9104_4_OUT_0 316
#define Gpio_TLE9104_4_OUT_1 317
#define Gpio_TLE9104_4_OUT_2 318
#define Gpio_TLE9104_4_OUT_3 319
#define Gpio_TLE9104_5_OUT_0 320
#define Gpio_TLE9104_5_OUT_1 321
#define Gpio_TLE9104_5_OUT_2 322
#define Gpio_TLE9104_5_OUT_3 323
#define Gpio_Unassigned 0
#define GPPWM_1_NAME "General Purpose PWM 1"
#define GPPWM_2_NAME "General Purpose PWM 2"
#define GPPWM_3_NAME "General Purpose PWM 3"
#define GPPWM_4_NAME "General Purpose PWM 4"
#define gppwm_channel_e_auto_enum 0="GPPWM_Zero",9="GPPWM_AccelPedal",16="GPPWM_AuxLinear1",17="GPPWM_AuxLinear2",7="GPPWM_AuxTemp1",8="GPPWM_AuxTemp2",26="GPPWM_BaroPressure",3="GPPWM_Clt",25="GPPWM_DetectedGear",15="GPPWM_EthanolPercent",5="GPPWM_FuelLoad",18="GPPWM_GppwmOutput1",19="GPPWM_GppwmOutput2",20="GPPWM_GppwmOutput3",21="GPPWM_GppwmOutput4",4="GPPWM_Iat",6="GPPWM_IgnLoad",22="GPPWM_LuaGauge1",23="GPPWM_LuaGauge2",2="GPPWM_Map",24="GPPWM_Rpm",1="GPPWM_Tps",12="GPPWM_VVT_1E",11="GPPWM_VVT_1I",14="GPPWM_VVT_2E",13="GPPWM_VVT_2I",10="GPPWM_Vbatt"
#define gppwm_channel_e_enum "Zero", "TPS", "MAP", "CLT", "IAT", "Fuel Load", "Ignition Load", "Aux Temp 1", "Aux Temp 2", "Accel Pedal", "Battery Voltage", "VVT 1 I", "VVT 1 E", "VVT 2 I", "VVT 2 E", "Ethanol (Flex) %", "Aux Linear 1", "Aux Linear 2", "GPPWM Output 1", "GPPWM Output 2", "GPPWM Output 3", "GPPWM Output 4", "Lua Gauge 1", "Lua Gauge 2", "RPM", "Gear (detected)", "Baro pressure"
#define gppwm_channel_e_GPPWM_AccelPedal 9
#define gppwm_channel_e_GPPWM_AuxLinear1 16
#define gppwm_channel_e_GPPWM_AuxLinear2 17
#define gppwm_channel_e_GPPWM_AuxTemp1 7
#define gppwm_channel_e_GPPWM_AuxTemp2 8
#define gppwm_channel_e_GPPWM_BaroPressure 26
#define gppwm_channel_e_GPPWM_Clt 3
#define gppwm_channel_e_GPPWM_DetectedGear 25
#define gppwm_channel_e_GPPWM_EthanolPercent 15
#define gppwm_channel_e_GPPWM_FuelLoad 5
#define gppwm_channel_e_GPPWM_GppwmOutput1 18
#define gppwm_channel_e_GPPWM_GppwmOutput2 19
#define gppwm_channel_e_GPPWM_GppwmOutput3 20
#define gppwm_channel_e_GPPWM_GppwmOutput4 21
#define gppwm_channel_e_GPPWM_Iat 4
#define gppwm_channel_e_GPPWM_IgnLoad 6
#define gppwm_channel_e_GPPWM_LuaGauge1 22
#define gppwm_channel_e_GPPWM_LuaGauge2 23
#define gppwm_channel_e_GPPWM_Map 2
#define gppwm_channel_e_GPPWM_Rpm 24
#define gppwm_channel_e_GPPWM_Tps 1
#define gppwm_channel_e_GPPWM_Vbatt 10
#define gppwm_channel_e_GPPWM_VVT_1E 12
#define gppwm_channel_e_GPPWM_VVT_1I 11
#define gppwm_channel_e_GPPWM_VVT_2E 14
#define gppwm_channel_e_GPPWM_VVT_2I 13
#define gppwm_channel_e_GPPWM_Zero 0
#define gppwm_channel_size 108
#define GPPWM_CHANNELS 4
#define gppwm_compare_mode_e_GPPWM_GreaterThan 0
#define gppwm_compare_mode_e_GPPWM_LessThan 1
#define GPPWM_LOAD_COUNT 8
#define GPPWM_NOTE_SIZE 16
#define GPPWM_RPM_COUNT 8
#define HIGH_SPEED_COUNT 32
#define hpfp_cam_e_auto_enum 0="HPFP_CAM_NONE",2="HPFP_CAM_EX1",4="HPFP_CAM_EX2",1="HPFP_CAM_IN1",3="HPFP_CAM_IN2"
#define hpfp_cam_e_enum "NONE", "Intake 1", "Exhaust 1", "Intake 2", "Exhaust 2"
#define hpfp_cam_e_HPFP_CAM_EX1 2
#define hpfp_cam_e_HPFP_CAM_EX2 4
#define hpfp_cam_e_HPFP_CAM_IN1 1
#define hpfp_cam_e_HPFP_CAM_IN2 3
#define hpfp_cam_e_HPFP_CAM_NONE 0
#define HPFP_COMPENSATION_SIZE 10
#define HPFP_DEADTIME_SIZE 8
#define HPFP_LOBE_PROFILE_SIZE 16
#define HPFP_TARGET_SIZE 10
#define HW_MAX_ADC_INDEX 17
#define IAC_PID_MULT_SIZE 8
#define IAT_CURVE_SIZE 16
#define IDLE_ADVANCE_CURVE_SIZE 8
#define idle_hardware_s_size 12
#define idle_mode_e_auto_enum 0="IM_AUTO",1="IM_MANUAL"
#define idle_mode_e_IM_AUTO 0
#define idle_mode_e_IM_MANUAL 1
#define idle_state_e_BLIP 64
#define idle_state_e_INIT 0
#define idle_state_e_PID_UPPER 16
#define idle_state_e_PID_VALUE 4
#define idle_state_e_RPM_DEAD_ZONE 2
#define idle_state_e_TPS_THRESHOLD 1
#define IDLE_VE_SIZE 4
#define IGN_BLEND_COUNT 4
#define ign_cyl_trim_s_size 16
#define IGN_LOAD_COUNT 16
#define IGN_RPM_COUNT 16
#define IGN_TRIM_SIZE 4
#define ignition_mode_e_auto_enum 0="IM_ONE_COIL",1="IM_INDIVIDUAL_COILS",3="IM_TWO_COILS",2="IM_WASTED_SPARK"
#define ignition_mode_e_IM_INDIVIDUAL_COILS 1
#define ignition_mode_e_IM_ONE_COIL 0
#define ignition_mode_e_IM_TWO_COILS 3
#define ignition_mode_e_IM_WASTED_SPARK 2
#define IGNITION_OUTPUT "Ignition Output"
#define imu_type_e_auto_enum 0="IMU_NONE",2="IMU_MM5_10",3="IMU_TYPE_3",4="IMU_TYPE_4",5="IMU_TYPE_MB_A0065422618",1="IMU_VAG"
#define imu_type_e_IMU_MM5_10 2
#define imu_type_e_IMU_NONE 0
#define imu_type_e_IMU_TYPE_3 3
#define imu_type_e_IMU_TYPE_4 4
#define imu_type_e_IMU_TYPE_MB_A0065422618 5
#define imu_type_e_IMU_VAG 1
#define INDICATOR_NAME_AC_SWITCH "AC switch"
#define INDICATOR_NAME_BRAKE_DOWN "Brake switch"
#define INDICATOR_NAME_CLUTCH_DOWN "Clutch: down"
#define INDICATOR_NAME_CLUTCH_UP "Clutch: up"
#define INJ_STAGING_COUNT 6
#define injection_mode_e_auto_enum 0="IM_SIMULTANEOUS",2="IM_BATCH",1="IM_SEQUENTIAL",3="IM_SINGLE_POINT"
#define injection_mode_e_IM_BATCH 2
#define injection_mode_e_IM_SEQUENTIAL 1
#define injection_mode_e_IM_SIMULTANEOUS 0
#define injection_mode_e_IM_SINGLE_POINT 3
#define InjectionTimingMode_auto_enum 0="End",2="Center",1="Start"
#define InjectionTimingMode_Center 2
#define InjectionTimingMode_End 0
#define InjectionTimingMode_Start 1
#define INJECTOR_1_NAME "Injector #1"
#define INJECTOR_2_NAME "Injector #2"
#define injector_compensation_mode_e_auto_enum 0="ICM_None",1="ICM_FixedRailPressure",2="ICM_SensedRailPressure"
#define injector_compensation_mode_e_ICM_FixedRailPressure 1
#define injector_compensation_mode_e_ICM_None 0
#define injector_compensation_mode_e_ICM_SensedRailPressure 2
#define injector_pressure_type_e_auto_enum 0="IPT_Low",1="IPT_High"
#define injector_pressure_type_e_IPT_High 1
#define injector_pressure_type_e_IPT_Low 0
#define injector_s_size 68
#define InjectorNonlinearMode_auto_enum 0="INJ_None",2="INJ_FordModel",1="INJ_PolynomialAdder"
#define InjectorNonlinearMode_INJ_FordModel 2
#define InjectorNonlinearMode_INJ_None 0
#define InjectorNonlinearMode_INJ_PolynomialAdder 1
#define launchActivationMode_e_ALWAYS_ACTIVE_LAUNCH 2
#define launchActivationMode_e_auto_enum 0="SWITCH_INPUT_LAUNCH",2="ALWAYS_ACTIVE_LAUNCH",1="CLUTCH_INPUT_LAUNCH",3="STOP_INPUT_LAUNCH"
#define launchActivationMode_e_CLUTCH_INPUT_LAUNCH 1
#define launchActivationMode_e_enum "Launch Button", "Clutch Down Switch", "Speed Based", "Brake Pedal"
#define launchActivationMode_e_STOP_INPUT_LAUNCH 3
#define launchActivationMode_e_SWITCH_INPUT_LAUNCH 0
#define linear_sensor_s_size 20
#define load_override_e_AFR_AccPedal 3
#define load_override_e_AFR_CylFilling 4
#define load_override_e_AFR_MAP 1
#define load_override_e_AFR_None 0
#define load_override_e_AFR_Tps 2
#define load_override_e_auto_enum 0="AFR_None",3="AFR_AccPedal",4="AFR_CylFilling",1="AFR_MAP",2="AFR_Tps"
#define load_override_e_enum "None", "MAP", "TPS", "Acc Pedal", "Cyl Filling %"
#define LOG_DELIMITER "`"
#define LOGIC_ANALYZER_CHANNEL_COUNT 4
#define LUA_ANALOG_INPUT_COUNT 8
#define LUA_DIGITAL_INPUT_COUNT 8
#define LUA_GAUGE_COUNT 8
#define LUA_PWM_COUNT 8
#define LUA_SCRIPT_SIZE 8000
#define MAF_DECODING_COUNT 32
#define maf_sensor_type_e_auto_enum 0="CUSTOM",2="Bosch0280218004",1="Bosch0280218037",3="DensoTODO"
#define maf_sensor_type_e_Bosch0280218004 2
#define maf_sensor_type_e_Bosch0280218037 1
#define maf_sensor_type_e_CUSTOM 0
#define maf_sensor_type_e_DensoTODO 3
#define maf_sensor_type_e_enum "v0", "v1", "v2", "v3"
#define MAIN_HELP_URL "https://rusefi.com/s/hellen72"
#define MAP_ANGLE_SIZE 8
#define MAP_sensor_config_s_size 140
#define MAP_WINDOW_SIZE 8
#define MAX_CYLINDER_COUNT 12
#define MAX_TPS_PPS_DISCREPANCY 5
#define mc33810maxDwellTimer_e_auto_enum 0="DWELL_2MS",3="DWELL_16MS",4="DWELL_32MS",1="DWELL_4MS",5="DWELL_64MS",2="DWELL_8MS"
#define mc33810maxDwellTimer_e_DWELL_16MS 3
#define mc33810maxDwellTimer_e_DWELL_2MS 0
#define mc33810maxDwellTimer_e_DWELL_32MS 4
#define mc33810maxDwellTimer_e_DWELL_4MS 1
#define mc33810maxDwellTimer_e_DWELL_64MS 5
#define mc33810maxDwellTimer_e_DWELL_8MS 2
#define MLQ_FIELD_HEADER_SIZE 89
#define MLQ_HEADER_SIZE 24
#define MsIoBox_config_s_size 4
#define MsIoBoxId_auto_enum 0="OFF",1="ID200",2="ID220",3="ID240"
#define MsIoBoxId_enum "Off", "ID1 (0x200)", "ID2 (0x220)", "ID3 (0x240)"
#define MsIoBoxId_ID200 1
#define MsIoBoxId_ID220 2
#define MsIoBoxId_ID240 3
#define MsIoBoxId_OFF 0
#define MsIoBoxVss_ALL1234 3
#define MsIoBoxVss_auto_enum 0="OFF",3="ALL1234",2="HALL34",1="VR12"
#define MsIoBoxVss_enum "Off", "VR speed in (1, 2)", "Hall speed in (3, 4)", "All (1, 2, 3, 4)"
#define MsIoBoxVss_HALL34 2
#define MsIoBoxVss_OFF 0
#define MsIoBoxVss_VR12 1
#define operation_mode_e_FOUR_STROKE_CAM_SENSOR 2
#define operation_mode_e_FOUR_STROKE_CRANK_SENSOR 1
#define operation_mode_e_FOUR_STROKE_SIX_TIMES_CRANK_SENSOR 7
#define operation_mode_e_FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR 4
#define operation_mode_e_FOUR_STROKE_THREE_TIMES_CRANK_SENSOR 5
#define operation_mode_e_FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR 6
#define operation_mode_e_OM_NONE 0
#define operation_mode_e_TWO_STROKE 3
#define output_pin_e_enum 0="NONE",105="2A - INJ_1",62="2B - ECF (PWM8)",59="2C - AC Fan / INJ_5",106="2D - INJ_2",61="2G - INJ_3",60="2J - INJ_4",129="2K - AC (O4)",100="2M - FPUMP (O9)",94="2N - VTSC / INJ_6",70="2O - IGN_3",64="2Q - IDLE (PWM5)",102="2R - CE (O11)",95="3C - Purge Solenoid / INJ_7",96="3D - EGR Solenoid / INJ_8",101="3E - CANIST (O10)",138="3F - IGN_1 (1&4)",132="3H - MAIN (O1)",71="3I - IGN_2 (2&3)",128="3J - O2H (O3)",136="3L - IGN_7 / AFR",65="3M - ALTERN (PWM6)",69="3N - IGN_4",63="3O - TACH (PWM7)",11="3P - O2H2 (O7)",127="3U - AWARN (O2)",68="3Z - IGN_5 / GNDA",135="4K - IGN_6 / +5V_MAP",130="4R - VVT (O5)",137="4U - MAP2/Ign8 (A10)",43="5E - SOLENOID OUTPUT",41="ETB +",42="ETB -",40="ETB EN"
#define output_pin_e_fullenum "NONE","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","3P - O2H2 (O7)","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","ETB EN","ETB +","ETB -","5E - SOLENOID OUTPUT","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","2C - AC Fan / INJ_5","2J - INJ_4","2G - INJ_3","2B - ECF (PWM8)","3O - TACH (PWM7)","2Q - IDLE (PWM5)","3M - ALTERN (PWM6)","INVALID","INVALID","3Z - IGN_5 / GNDA","3N - IGN_4","2O - IGN_3","3I - IGN_2 (2&3)","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","2N - VTSC / INJ_6","3C - Purge Solenoid / INJ_7","3D - EGR Solenoid / INJ_8","INVALID","INVALID","INVALID","2M - FPUMP (O9)","3E - CANIST (O10)","2R - CE (O11)","INVALID","INVALID","2A - INJ_1","2D - INJ_2","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","3U - AWARN (O2)","3J - O2H (O3)","2K - AC (O4)","4R - VVT (O5)","INVALID","3H - MAIN (O1)","INVALID","INVALID","4K - IGN_6 / +5V_MAP","3L - IGN_7 / AFR","4U - MAP2/Ign8 (A10)","3F - IGN_1 (1&4)"
#define PACK_ADD_TEMPERATURE 40
#define PACK_MULT_AFR 1000
#define PACK_MULT_AFR_CFG 10
#define PACK_MULT_ANGLE 50
#define PACK_MULT_EGTEMPERATURE 4
#define PACK_MULT_FLEX 2
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
#define PACK_MULT_VSS 100
#define PACK_PERCENT_BYTE_MULT 100.0
#define PEDAL_TO_TPS_SIZE 8
#define PERCENT_TRIM_BYTE_PACKING_DIV 0.02
#define persistent_config_s_size 22656
#define pid_s_size 20
#define pin_input_mode_e_auto_enum 0="PI_DEFAULT",2="PI_PULLDOWN",1="PI_PULLUP"
#define pin_input_mode_e_enum "DEFAULT", "PULLUP", "PULLDOWN"
#define pin_input_mode_e_PI_DEFAULT 0
#define pin_input_mode_e_PI_PULLDOWN 2
#define pin_input_mode_e_PI_PULLUP 1
#define pin_mode_e_enum "default", "INVALID", "INVALID", "INVALID", "opendrain", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PULLUP", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PULLDOWN"
#define pin_output_mode_e_auto_enum 0="OM_DEFAULT",1="OM_INVERTED",2="OM_OPENDRAIN",3="OM_OPENDRAIN_INVERTED"
#define pin_output_mode_e_enum "default", "default inverted", "open collector", "open collector inverted"
#define pin_output_mode_e_OM_DEFAULT 0
#define pin_output_mode_e_OM_INVERTED 1
#define pin_output_mode_e_OM_OPENDRAIN 2
#define pin_output_mode_e_OM_OPENDRAIN_INVERTED 3
#define PRIME_CURVE_COUNT 8
#define PROTOCOL_ACR_NAME "acr"
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
#define PROTOCOL_INJ1_STAGE2_SHORT_NAME "j1"
#define PROTOCOL_KNOCK_SPECTROGRAMM "knock_spectrogram"
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
#define RANGE_INPUT_COUNT 6
#define SCRIPT_CURVE_16 16
#define SCRIPT_CURVE_8 8
#define SCRIPT_CURVE_COUNT 6
#define SCRIPT_SETTING_COUNT 8
#define SCRIPT_TABLE_8 8
#define SCRIPT_TABLE_COUNT 4
#define SelectedGear_Drive 6
#define SelectedGear_Invalid 0
#define SelectedGear_Low 11
#define SelectedGear_Manual 7
#define SelectedGear_Manual1 10
#define SelectedGear_Manual2 9
#define SelectedGear_Manual3 8
#define SelectedGear_ManualMinus 2
#define SelectedGear_ManualPlus 1
#define SelectedGear_Neutral 5
#define SelectedGear_Park 3
#define SelectedGear_Reverse 4
#define sensor_chart_e_auto_enum 0="SC_OFF",5="SC_AUX_FAST1",4="SC_DETAILED_RPM",3="SC_RPM_ACCEL",1="SC_TRIGGER"
#define sensor_chart_e_enum "none", "trigger", "INVALID", "RPM ACCEL", "DETAILED RPM", "Fast Aux1"
#define sensor_chart_e_SC_AUX_FAST1 5
#define sensor_chart_e_SC_DETAILED_RPM 4
#define sensor_chart_e_SC_OFF 0
#define sensor_chart_e_SC_RPM_ACCEL 3
#define sensor_chart_e_SC_TRIGGER 1
#define SENT_INPUT_COUNT 1
#define sent_input_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15"
#define SentEtbType_auto_enum 0="NONE",3="CUSTOM",2="FORD_TYPE_1",1="GM_TYPE_1"
#define SentEtbType_CUSTOM 3
#define SentEtbType_enum "None", "GM type 1", "Ford type 1", "Custom"
#define SentEtbType_FORD_TYPE_1 2
#define SentEtbType_GM_TYPE_1 1
#define SentEtbType_NONE 0
#define show_Frankenso_presets false
#define show_HellenNB2_presets true
#define show_microRusEFI_presets false
#define show_Proteus_presets false
#define show_test_presets false
#define SIGNATURE_HASH 3710616047
#define SIMULATOR_TUNE_BIN_FILE_NAME "generated/simulator_tune_image.bin"
#define SIMULATOR_TUNE_BIN_FILE_NAME_PREFIX "generated/simulator_tune_image"
#define SIMULATOR_TUNE_BIN_FILE_NAME_SUFFIX ".bin"
#define spi_device_e_auto_enum 0="SPI_NONE",1="SPI_DEVICE_1",2="SPI_DEVICE_2",3="SPI_DEVICE_3",4="SPI_DEVICE_4",5="SPI_DEVICE_5",6="SPI_DEVICE_6"
#define spi_device_e_SPI_DEVICE_1 1
#define spi_device_e_SPI_DEVICE_2 2
#define spi_device_e_SPI_DEVICE_3 3
#define spi_device_e_SPI_DEVICE_4 4
#define spi_device_e_SPI_DEVICE_5 5
#define spi_device_e_SPI_DEVICE_6 6
#define spi_device_e_SPI_NONE 0
#define spi_speed_e__150KHz 3
#define spi_speed_e__1_25MHz 2
#define spi_speed_e__2_5MHz 1
#define spi_speed_e__5MHz 0
#define stepper_num_micro_steps_e_auto_enum 0="STEPPER_FULL",8="STEPPER_EIGHTH",4="STEPPER_FOURTH",2="STEPPER_HALF"
#define stepper_num_micro_steps_e_enum "Full-Step (Default)", "INVALID", "Half-Step", "INVALID", "1/4 Micro-Step", "INVALID", "INVALID", "INVALID", "1/8 Micro-Step"
#define stepper_num_micro_steps_e_STEPPER_EIGHTH 8
#define stepper_num_micro_steps_e_STEPPER_FOURTH 4
#define stepper_num_micro_steps_e_STEPPER_FULL 0
#define stepper_num_micro_steps_e_STEPPER_HALF 2
#define STFT_BANK_COUNT 2
#define stft_cell_cfg_s_size 4
#define STFT_CELL_COUNT 4
#define stft_s_size 24
#define switch_input_pin_e_enum 0="NONE",91="4B - Brake/RES1 (A7)",18="4F - AC_PRES/AUX1 (A23)",90="4I - Clutch rev D",79="4I - Clutch rev E/F",9="4J - VTCS/AUX4 (A20)",34="5A - Pressure Input",87="5D - SENSOR INPUT",38="5M - PPS2 OR TEMPERATURE SENSOR",2="5N - TPS2",5="5P - PPS1"
#define switch_input_pin_e_fullenum "NONE","INVALID","5N - TPS2","INVALID","INVALID","5P - PPS1","INVALID","INVALID","INVALID","4J - VTCS/AUX4 (A20)","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","4F - AC_PRES/AUX1 (A23)","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","5A - Pressure Input","INVALID","INVALID","INVALID","5M - PPS2 OR TEMPERATURE SENSOR","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","4I - Clutch rev E/F","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","INVALID","5D - SENSOR INPUT","INVALID","INVALID","4I - Clutch rev D","4B - Brake/RES1 (A7)"
#define TABLE_2_LOAD_SIZE 8
#define TABLE_2_RPM_SIZE 8
#define TABLE_3_LOAD_SIZE 8
#define TABLE_3_RPM_SIZE 8
#define TABLE_4_RPM 10
#define tChargeMode_e_auto_enum 0="TCHARGE_MODE_RPM_TPS",1="TCHARGE_MODE_AIR_INTERP",2="TCHARGE_MODE_AIR_INTERP_TABLE"
#define tChargeMode_e_enum "RPM+TPS (Default)", "Air Mass Interpolation", "Table"
#define tChargeMode_e_TCHARGE_MODE_AIR_INTERP 1
#define tChargeMode_e_TCHARGE_MODE_AIR_INTERP_TABLE 2
#define tChargeMode_e_TCHARGE_MODE_RPM_TPS 0
#define TCU_GEAR_COUNT 10
#define TCU_RANGE_COUNT 11
#define TCU_SOLENOID_COUNT 6
#define TCU_TABLE_WIDTH 8
#define thermistor_conf_s_size 28
#define ThermistorConf_size 32
#define timing_mode_e_auto_enum 0="TM_DYNAMIC",1="TM_FIXED"
#define timing_mode_e_TM_DYNAMIC 0
#define timing_mode_e_TM_FIXED 1
#define tle8888_mode_e_auto_enum 0="TL_AUTO",3="TL_HALL",2="TL_MANUAL",1="TL_SEMI_AUTO"
#define tle8888_mode_e_TL_AUTO 0
#define tle8888_mode_e_TL_HALL 3
#define tle8888_mode_e_TL_MANUAL 2
#define tle8888_mode_e_TL_SEMI_AUTO 1
#define TOOTH_DATA_LENGTH 2000
#define TOOTH_PACKET_COUNT 1000
#define TOOTH_PACKET_SIZE 2
#define TOP_DEAD_CENTER_MESSAGE "r"
#define TORQUE_CURVE_SIZE 6
#define TOTAL_CONFIG_SIZE 22656
#define TPS_2_BYTE_PACKING_MULT 100
#define TPS_PPS_TOO_HIGH_THRESHOLD 110
#define TPS_PPS_TOO_LOW_THRESHOLD -10
#define TPS_TPS_ACCEL_CLT_CORR_TABLE 4
#define TPS_TPS_ACCEL_TABLE 8
#define TRACTION_CONTROL_ETB_DROP_SIZE 6
#define transmission_controller_e_enum "None", "Simple Transmission", "Generic 4-Speed", "GM 4L6X"
#define TransmissionControllerMode_auto_enum 0="None",2="Generic4",3="Gm4l6x",1="SimpleTransmissionController"
#define TransmissionControllerMode_Generic4 2
#define TransmissionControllerMode_Gm4l6x 3
#define TransmissionControllerMode_None 0
#define TransmissionControllerMode_SimpleTransmissionController 1
#define TRIGGER_COMMENT "#"
#define trigger_config_s_size 12
#define TRIGGER_CRANK_BASED trigger_type == 2 || trigger_type == 5 || trigger_type == 10 || trigger_type == 12 || trigger_type == 20 || trigger_type == 21 || trigger_type == 26 || trigger_type == 27 || trigger_type == 33 || trigger_type == 38 || trigger_type == 39 || trigger_type == 44 || trigger_type == 45 || trigger_type == 46 || trigger_type == 53 || trigger_type == 55 || trigger_type == 57 || trigger_type == 58 || trigger_type == 61 || trigger_type == 62 || trigger_type == 68 || trigger_type == 70 || trigger_type == 71 || trigger_type == 72 || trigger_type == 74 || trigger_type == 75 || trigger_type == 76 || trigger_type == 77
#define TRIGGER_CYCLE_DURATION "cycleDuration"
#define TRIGGER_GAP_FROM "gapFrom"
#define TRIGGER_GAP_TO "gapTo"
#define TRIGGER_GAPS_COUNT "gapsCount"
#define TRIGGER_HAS_SECOND_CHANNEL "hasSecondChannel"
#define TRIGGER_INPUT_PIN_COUNT 2
#define TRIGGER_IS_CRANK_KEY "crankBased"
#define TRIGGER_IS_SECOND_WHEEL_CAM "isSecondWheelCam"
#define TRIGGER_KNOWN_OPERATION_MODE "knownOperationMode"
#define TRIGGER_SIMULATOR_PIN_COUNT 2
#define TRIGGER_SYNC_EDGE "syncEdge"
#define trigger_type_e_auto_enum 0="TT_TOOTHED_WHEEL",38="TT_12_TOOTH_CRANK",25="TT_2JZ_3_34_SIMULATION_ONLY",70="TT_36_2_1",71="TT_36_2_1_1",23="TT_36_2_2_2",31="TT_3_1_CAM",72="TT_3_TOOTH_CRANK",77="TT_60DEG_TOOTH",75="TT_60_2_2_F3R",20="TT_60_2_WRONG_POLARITY",80="TT_6_TOOTH_CRANK",21="TT_BENELLI_TRE",30="TT_CHRYSLER_NGC_36_2_2",6="TT_DAIHATSU_3_CYL",81="TT_DAIHATSU_4_CYL",2="TT_DODGE_NEON_1995",39="TT_DODGE_NEON_1995_ONLY_CRANK",14="TT_DODGE_NEON_2003_CAM",32="TT_DODGE_NEON_2003_CRANK",19="TT_DODGE_RAM",22="TT_DODGE_STRATUS",41="TT_FIAT_IAW_P8",1="TT_FORD_ASPIRE",42="TT_FORD_ST170",65="TT_FORD_TFI_PIP",74="TT_GM_24x_3",27="TT_GM_24x_5",54="TT_GM_60_2_2_2",5="TT_GM_7X",18="TT_HALF_MOON",28="TT_HONDA_CBR_600",67="TT_HONDA_K_CAM_4_1",46="TT_HONDA_K_CRANK_12_1",37="TT_JEEP_18_2_2_2",40="TT_JEEP_4_CYL",57="TT_KAWA_KX450F",15="TT_MAZDA_DOHC_1_4",3="TT_MAZDA_MIATA_NA",35="TT_MAZDA_MIATA_VVT_TEST",7="TT_MAZDA_SOHC_4",10="TT_MERCEDES_2_SEGMENT",33="TT_MIATA_VVT",34="TT_MITSU_4G63_CAM",76="TT_MITSU_4G63_CRANK",29="TT_MITSU_4G9x_CAM",11="TT_NARROW_SINGLE_TOOTH",52="TT_NISSAN_MR18_CAM_VVT",68="TT_NISSAN_MR18_CRANK",61="TT_NISSAN_QR25",24="TT_NISSAN_SR20VE",60="TT_NISSAN_VQ30",58="TT_NISSAN_VQ35",16="TT_ONE_PLUS_ONE",44="TT_RENIX_44_2_2",45="TT_RENIX_66_2_2_2",26="TT_ROVER_K",55="TT_SKODA_FAVORIT",36="TT_SUBARU_7_6",51="TT_SUBARU_7_WITHOUT_6",12="TT_SUBARU_EZ30",49="TT_SUBARU_SVX",64="TT_SUBARU_SVX_CAM_VVT",63="TT_SUBARU_SVX_CRANK_1",66="TT_SUZUKI_G13B",69="TT_TOOTHED_WHEEL_32_2",9="TT_TOOTHED_WHEEL_36_1",48="TT_TOOTHED_WHEEL_36_2",8="TT_TOOTHED_WHEEL_60_2",53="TT_TRI_TACH",82="TT_UNUSED",50="TT_UNUSED_50",56="TT_VVT_BARRA_3_PLUS_1",47="TT_VVT_BOSCH_QUICK_START",4="TT_VVT_FORD_COYOTE",13="TT_VVT_MAZDA_SKYACTIV",43="TT_VVT_MIATA_NB",62="TT_VVT_MITSUBISHI_3A92",78="TT_VVT_MITSUBISHI_4G69",79="TT_VVT_MITSU_6G72",59="TT_VVT_NISSAN_VQ35",17="TT_VVT_TOYOTA_3_TOOTH",73="TT_VVT_TOYOTA_4_1"
#define trigger_type_e_enum "custom toothed wheel", "Ford Aspire", "Dodge Neon 1995", "Miata NA", "INVALID", "GM_7X", "Daihatsu 3 cylinder", "Mazda SOHC 4", "60-2", "36-1", "Mercedes Two Segment", "Single Tooth", "EZ30", "INVALID", "Dodge Neon 2003", "Mazda DOHC 1+4", "1+1", "INVALID", "Half Moon", "Dodge Ram 1+16", "60-2 Wrong Polarity", "Benelli Tre", "Dodge Stratus", "36_2_2_2", "Nissan Primera", "dev 2JZ 3/34 simulator", "Rover K", "GM 24x 5 degree", "Honda CBR 600 Cam", "Mitsubishi 4G92/93/94 Cam 29", "Honda CBR 600 custom", "3-1 skipped", "Dodge Neon 2003 crank", "Miata NB", "Mitsubishi 4G63 Cam 34", "INVALID", "Subaru 7+6", "Jeep 18-2-2-2", "12crank/24cam", "Dodge Neon 1995 crank only", "Jeep XJ 4 cyl", "FiatIAQ_P8", "Mazda Z5", "INVALID", "Renix 44-2-2", "Renix 66-2-2-2", "Honda K 12+1", "INVALID", "36-2", "Subaru SVX", "1+16", "Subaru 7 without 6", "INVALID", "INVALID", "GM 60-2-2-2", "Skoda Favorit", "Barra 3+1 Cam", "Kawa KX450F", "Nissan VQ35", "INVALID", "Nissan VQ30", "Nissan QR25", "INVALID", "Subaru SVX Crank 1", "Subaru SVX Cam VVT", "Ford PIP", "Suzuki G13B", "Honda K 4+1", "Nissan MR18 Crank", "32-2", "36-2-1", "36-2-1-1", "3-0", "INVALID", "GM 24x 3 degree", "60-2-2 F3R", "Mitsu 4G63 Crank", "x2 30 Deg camshaft BTDC", "INVALID", "Mitsu 6G72 Cam", "6-0", "Daihatsu 4 cylinder", "trg82", "trg83"
#define trigger_type_e_TT_12_TOOTH_CRANK 38
#define trigger_type_e_TT_2JZ_3_34_SIMULATION_ONLY 25
#define trigger_type_e_TT_36_2_1 70
#define trigger_type_e_TT_36_2_1_1 71
#define trigger_type_e_TT_36_2_2_2 23
#define trigger_type_e_TT_3_1_CAM 31
#define trigger_type_e_TT_3_TOOTH_CRANK 72
#define trigger_type_e_TT_60_2_2_F3R 75
#define trigger_type_e_TT_60_2_WRONG_POLARITY 20
#define trigger_type_e_TT_60DEG_TOOTH 77
#define trigger_type_e_TT_6_TOOTH_CRANK 80
#define trigger_type_e_TT_BENELLI_TRE 21
#define trigger_type_e_TT_CHRYSLER_NGC_36_2_2 30
#define trigger_type_e_TT_DAIHATSU_3_CYL 6
#define trigger_type_e_TT_DAIHATSU_4_CYL 81
#define trigger_type_e_TT_DODGE_NEON_1995 2
#define trigger_type_e_TT_DODGE_NEON_1995_ONLY_CRANK 39
#define trigger_type_e_TT_DODGE_NEON_2003_CAM 14
#define trigger_type_e_TT_DODGE_NEON_2003_CRANK 32
#define trigger_type_e_TT_DODGE_RAM 19
#define trigger_type_e_TT_DODGE_STRATUS 22
#define trigger_type_e_TT_FIAT_IAW_P8 41
#define trigger_type_e_TT_FORD_ASPIRE 1
#define trigger_type_e_TT_FORD_ST170 42
#define trigger_type_e_TT_FORD_TFI_PIP 65
#define trigger_type_e_TT_GM_24x_3 74
#define trigger_type_e_TT_GM_24x_5 27
#define trigger_type_e_TT_GM_60_2_2_2 54
#define trigger_type_e_TT_GM_7X 5
#define trigger_type_e_TT_HALF_MOON 18
#define trigger_type_e_TT_HONDA_CBR_600 28
#define trigger_type_e_TT_HONDA_K_CAM_4_1 67
#define trigger_type_e_TT_HONDA_K_CRANK_12_1 46
#define trigger_type_e_TT_JEEP_18_2_2_2 37
#define trigger_type_e_TT_JEEP_4_CYL 40
#define trigger_type_e_TT_KAWA_KX450F 57
#define trigger_type_e_TT_MAZDA_DOHC_1_4 15
#define trigger_type_e_TT_MAZDA_MIATA_NA 3
#define trigger_type_e_TT_MAZDA_MIATA_VVT_TEST 35
#define trigger_type_e_TT_MAZDA_SOHC_4 7
#define trigger_type_e_TT_MERCEDES_2_SEGMENT 10
#define trigger_type_e_TT_MIATA_VVT 33
#define trigger_type_e_TT_MITSU_4G63_CAM 34
#define trigger_type_e_TT_MITSU_4G63_CRANK 76
#define trigger_type_e_TT_MITSU_4G9x_CAM 29
#define trigger_type_e_TT_NARROW_SINGLE_TOOTH 11
#define trigger_type_e_TT_NISSAN_MR18_CAM_VVT 52
#define trigger_type_e_TT_NISSAN_MR18_CRANK 68
#define trigger_type_e_TT_NISSAN_QR25 61
#define trigger_type_e_TT_NISSAN_SR20VE 24
#define trigger_type_e_TT_NISSAN_VQ30 60
#define trigger_type_e_TT_NISSAN_VQ35 58
#define trigger_type_e_TT_ONE_PLUS_ONE 16
#define trigger_type_e_TT_RENIX_44_2_2 44
#define trigger_type_e_TT_RENIX_66_2_2_2 45
#define trigger_type_e_TT_ROVER_K 26
#define trigger_type_e_TT_SKODA_FAVORIT 55
#define trigger_type_e_TT_SUBARU_7_6 36
#define trigger_type_e_TT_SUBARU_7_WITHOUT_6 51
#define trigger_type_e_TT_SUBARU_EZ30 12
#define trigger_type_e_TT_SUBARU_SVX 49
#define trigger_type_e_TT_SUBARU_SVX_CAM_VVT 64
#define trigger_type_e_TT_SUBARU_SVX_CRANK_1 63
#define trigger_type_e_TT_SUZUKI_G13B 66
#define trigger_type_e_TT_TOOTHED_WHEEL 0
#define trigger_type_e_TT_TOOTHED_WHEEL_32_2 69
#define trigger_type_e_TT_TOOTHED_WHEEL_36_1 9
#define trigger_type_e_TT_TOOTHED_WHEEL_36_2 48
#define trigger_type_e_TT_TOOTHED_WHEEL_60_2 8
#define trigger_type_e_TT_TRI_TACH 53
#define trigger_type_e_TT_UNUSED 82
#define trigger_type_e_TT_UNUSED_50 50
#define trigger_type_e_TT_VVT_BARRA_3_PLUS_1 56
#define trigger_type_e_TT_VVT_BOSCH_QUICK_START 47
#define trigger_type_e_TT_VVT_FORD_COYOTE 4
#define trigger_type_e_TT_VVT_MAZDA_SKYACTIV 13
#define trigger_type_e_TT_VVT_MIATA_NB 43
#define trigger_type_e_TT_VVT_MITSU_6G72 79
#define trigger_type_e_TT_VVT_MITSUBISHI_3A92 62
#define trigger_type_e_TT_VVT_MITSUBISHI_4G69 78
#define trigger_type_e_TT_VVT_NISSAN_VQ35 59
#define trigger_type_e_TT_VVT_TOYOTA_3_TOOTH 17
#define trigger_type_e_TT_VVT_TOYOTA_4_1 73
#define TRIGGER_TYPE_WITH_SECOND_WHEEL trigger_type == 1 || trigger_type == 3 || trigger_type == 15 || trigger_type == 16 || trigger_type == 19 || trigger_type == 25 || trigger_type == 31 || trigger_type == 35 || trigger_type == 36 || trigger_type == 37 || trigger_type == 40 || trigger_type == 49 || trigger_type == 53 || trigger_type == 54 || trigger_type == 63 || trigger_type == 64
#define TRIGGER_TYPE_WITHOUT_KNOWN_LOCATION trigger_type == 8 || trigger_type == 9 || trigger_type == 11 || trigger_type == 17 || trigger_type == 18 || trigger_type == 23 || trigger_type == 48 || trigger_type == 69 || trigger_type == 73
#define TRIGGER_WITH_SYNC "isSynchronizationNeeded"
#define TRIGGERS_FILE_NAME "triggers.txt"
#define TriggerWheel_T_PRIMARY 0
#define TriggerWheel_T_SECONDARY 1
#define ts_14_command_TS_14_0 0
#define ts_14_command_TS_14_1 1
#define ts_14_command_TS_14_4 4
#define ts_14_command_TS_14_5 5
#define ts_14_command_TS_14_9 9
#define ts_14_command_TS_BURN_WITHOUT_FLASH 0x15
#define ts_14_command_TS_ETB_AUTOCAL_0 0x0E
#define ts_14_command_TS_ETB_AUTOCAL_1 0x11
#define ts_14_command_TS_ETB_RESET 0x0B
#define ts_14_command_TS_ETB_START_AUTOTUNE 0x0C
#define ts_14_command_TS_ETB_STOP_AUTOTUNE 0x10
#define ts_14_command_TS_EXTERNAL_TRIGGER_STIMULATOR_ENABLE 0x13
#define ts_14_command_TS_GRAB_PEDAL_UP 6
#define ts_14_command_TS_GRAB_PEDAL_WOT 7
#define ts_14_command_TS_GRAB_TPS_CLOSED 2
#define ts_14_command_TS_GRAB_TPS_WOT 3
#define ts_14_command_TS_RESET_MC33810 0x14
#define ts_14_command_TS_RESET_TLE8888 8
#define ts_14_command_TS_TRIGGER_STIMULATOR_DISABLE 0x0F
#define ts_14_command_TS_TRIGGER_STIMULATOR_ENABLE 0x0D
#define ts_14_command_TS_WIDEBAND_UPDATE 0x12
#define ts_14_command_TS_WRITE_FLASH 0x0A
#define TS_BLOCK_READ_TIMEOUT 3000
#define TS_BURN_COMMAND 'B'
#define TS_BURN_COMMAND_char B
#define TS_CHUNK_WRITE_COMMAND 'C'
#define TS_CHUNK_WRITE_COMMAND_char C
#define ts_command_e_TS_BENCH_CATEGORY 22
#define ts_command_e_TS_CLEAR_WARNINGS 17
#define ts_command_e_TS_COMMAND_1 1
#define ts_command_e_TS_COMMAND_10 10
#define ts_command_e_TS_COMMAND_11 11
#define ts_command_e_TS_COMMAND_12 12
#define ts_command_e_TS_COMMAND_13 13
#define ts_command_e_TS_COMMAND_14 14
#define ts_command_e_TS_COMMAND_15 15
#define ts_command_e_TS_COMMAND_16 16
#define ts_command_e_TS_COMMAND_2 2
#define ts_command_e_TS_COMMAND_3 3
#define ts_command_e_TS_COMMAND_4 4
#define ts_command_e_TS_COMMAND_5 5
#define ts_command_e_TS_COMMAND_6 6
#define ts_command_e_TS_COMMAND_7 7
#define ts_command_e_TS_COMMAND_8 8
#define ts_command_e_TS_COMMAND_9 9
#define ts_command_e_TS_DEBUG_MODE 0
#define ts_command_e_TS_IGNITION_CATEGORY 18
#define ts_command_e_TS_INJECTOR_CATEGORY 19
#define ts_command_e_TS_LUA_OUTPUT_CATEGORY 32
#define ts_command_e_TS_SET_DEFAULT_ENGINE 31
#define ts_command_e_TS_SET_ENGINE_TYPE 30
#define ts_command_e_TS_SOLENOID_CATEGORY 25
#define ts_command_e_TS_UNUSED_23 23
#define ts_command_e_TS_UNUSED_24 24
#define ts_command_e_TS_UNUSED_26 26
#define ts_command_e_TS_UNUSED_27 27
#define ts_command_e_TS_UNUSED_28 28
#define ts_command_e_TS_UNUSED_29 29
#define ts_command_e_TS_WIDEBAND 21
#define ts_command_e_TS_X14 20
#define TS_COMMAND_F 'F'
#define TS_COMMAND_F_char F
#define TS_COMPOSITE_DISABLE 2
#define TS_COMPOSITE_ENABLE 1
#define TS_COMPOSITE_READ 3
#define TS_CRC_CHECK_COMMAND 'k'
#define TS_CRC_CHECK_COMMAND_char k
#define TS_EXECUTE 'E'
#define TS_EXECUTE_char E
#define TS_FILE_VERSION 20240404
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
#define TS_KNOCK_SPECTROGRAM_DISABLE 'n'
#define TS_KNOCK_SPECTROGRAM_DISABLE_char n
#define TS_KNOCK_SPECTROGRAM_ENABLE 'm'
#define TS_KNOCK_SPECTROGRAM_ENABLE_char m
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
#define TS_QUERY_BOOTLOADER 'L'
#define TS_QUERY_BOOTLOADER_char L
#define TS_QUERY_BOOTLOADER_NONE 0
#define TS_QUERY_BOOTLOADER_OPENBLT 1
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
#define ts_show_air_conditioning true
#define ts_show_analog_divider false
#define ts_show_auxserial_pins true
#define ts_show_bank2_cam1 true
#define ts_show_bank2_cam2 true
#define ts_show_cam2 true
#define ts_show_can2 false
#define ts_show_can_pins false
#define ts_show_check_engine true
#define ts_show_clt_iat_pullup true
#define ts_show_communityCommsLedPid false
#define ts_show_compression_ratio false
#define ts_show_cylinder_10 true
#define ts_show_cylinder_11 true
#define ts_show_cylinder_12 true
#define ts_show_cylinder_2 true
#define ts_show_cylinder_3 true
#define ts_show_cylinder_4 true
#define ts_show_cylinder_5 true
#define ts_show_cylinder_6 true
#define ts_show_cylinder_7 true
#define ts_show_cylinder_8 true
#define ts_show_cylinder_9 true
#define ts_show_cylinder_bank true
#define ts_show_debug_mode false
#define ts_show_egt false
#define ts_show_etb true
#define ts_show_etb_pins false
#define ts_show_experimental true
#define ts_show_fan2_settings true
#define ts_show_fancy_hardware false
#define ts_show_fancy_options true
#define ts_show_firing_order true
#define ts_show_forced_induction true
#define ts_show_fuel_pump true
#define ts_show_fuel_relay_pin true
#define ts_show_fuel_strategy true
#define ts_show_full_pinout true
#define ts_show_gdi false
#define ts_show_hardware_simulator false
#define ts_show_hd true
#define ts_show_hip9011 false
#define ts_show_idle_hardware true
#define ts_show_ign_key_analog_input false
#define ts_show_ign_key_switch false
#define ts_show_ignitionPinMode true
#define ts_show_inj_diag false
#define ts_show_injection_mode true
#define ts_show_injection_phase true
#define ts_show_injectionPinMode false
#define ts_show_k_line false
#define ts_show_l9779 false
#define ts_show_maf true
#define ts_show_main_relay true
#define ts_show_main_relay_microRusEFI_message false
#define ts_show_mc33810 false
#define ts_show_number_of_cylinders true
#define ts_show_odd_fire true
#define ts_show_onboard_accelerometer false
#define ts_show_output_diag false
#define ts_show_popular_vehicles true
#define ts_show_reset_calibrations false
#define ts_show_rotary true
#define ts_show_sd_card true
#define ts_show_sd_pins false
#define ts_show_software_knock true
#define ts_show_speedo_settings true
#define ts_show_spi true
#define ts_show_spi2_enable true
#define ts_show_spi2_pins true
#define ts_show_spi3_pins true
#define ts_show_starter_disable true
#define ts_show_status_leds true
#define ts_show_sync_cam true
#define ts_show_tcu false
#define ts_show_tle8888 false
#define ts_show_top_level_can_menu true
#define ts_show_tps2 true
#define ts_show_tps_sent false
#define ts_show_trigger_comparator false
#define ts_show_tunerstudio_port true
#define ts_show_vbatt true
#define ts_show_vehicle_weight false
#define ts_show_vr_threshold_2 false
#define ts_show_vr_threshold_all false
#define ts_show_vr_threshold_pins true
#define ts_show_vvt_output true
#define TS_SIGNATURE "rusEFI master.2024.05.03.hellen72.3710616047"
#define TS_SIMULATE_CAN '>'
#define TS_SIMULATE_CAN_char >
#define TS_SINGLE_WRITE_COMMAND 'W'
#define TS_SINGLE_WRITE_COMMAND_char W
#define TS_TEST_COMMAND 't'
#define TS_TEST_COMMAND_char t
#define TS_TOTAL_OUTPUT_SIZE 1744
#define TS_TRIGGER_SCOPE_CHANNEL_1_NAME "Channel 1"
#define TS_TRIGGER_SCOPE_CHANNEL_2_NAME "Channel 2"
#define TS_TRIGGER_SCOPE_DISABLE 5
#define TS_TRIGGER_SCOPE_ENABLE 4
#define TS_TRIGGER_SCOPE_READ 6
#define TsCalMode_EtbKd 5
#define TsCalMode_EtbKi 4
#define TsCalMode_EtbKp 3
#define TsCalMode_None 0
#define TsCalMode_PedalMax 13
#define TsCalMode_PedalMin 12
#define TsCalMode_Tps1Max 1
#define TsCalMode_Tps1Min 2
#define TsCalMode_Tps1SecondaryMax 6
#define TsCalMode_Tps1SecondaryMin 7
#define TsCalMode_Tps2Max 8
#define TsCalMode_Tps2Min 9
#define TsCalMode_Tps2SecondaryMax 10
#define TsCalMode_Tps2SecondaryMin 11
#define uart_device_e_auto_enum 0="UART_NONE",1="UART_DEVICE_1",2="UART_DEVICE_2",3="UART_DEVICE_3",4="UART_DEVICE_4"
#define uart_device_e_UART_DEVICE_1 1
#define uart_device_e_UART_DEVICE_2 2
#define uart_device_e_UART_DEVICE_3 3
#define uart_device_e_UART_DEVICE_4 4
#define uart_device_e_UART_NONE 0
#define UiMode_auto_enum 0="FULL",1="INSTALLATION",2="TUNING"
#define UiMode_enum "Full", "Installation", "Tuning"
#define UiMode_FULL 0
#define UiMode_INSTALLATION 1
#define UiMode_TUNING 2
#define VBAT_INJECTOR_CURVE_SIZE 8
#define VE_BLEND_COUNT 4
#define ve_override_e_auto_enum 0="VE_None",1="VE_MAP",2="VE_TPS"
#define ve_override_e_enum "None", "MAP", "TPS"
#define ve_override_e_VE_MAP 1
#define ve_override_e_VE_None 0
#define ve_override_e_VE_TPS 2
#define VEHICLE_INFO_SIZE 32
#define VIN_NUMBER_SIZE 17
#define VOLTAGE_1_BYTE_PACKING_DIV 0.02
#define VR_THRESHOLD_COUNT 2
#define vr_threshold_s_size 16
#define VSS_FILTER_MAX 200
#define VSS_FILTER_MIN 3
#define vvt_mode_e_auto_enum 0="VVT_INACTIVE",8="VVT_BARRA_3_PLUS_1",5="VVT_BOSCH_QUICK_START",19="VVT_FORD_COYOTE",7="VVT_FORD_ST170",21="VVT_HONDA_CBR_600",16="VVT_HONDA_K_EXHAUST",10="VVT_HONDA_K_INTAKE",13="VVT_MAP_V_TWIN",15="VVT_MAZDA_SKYACTIV",3="VVT_MIATA_NB",12="VVT_MITSUBISHI_3A92",18="VVT_MITSUBISHI_4G63",4="VVT_MITSUBISHI_4G69",17="VVT_MITSUBISHI_4G9x",20="VVT_MITSUBISHI_6G72",14="VVT_MITSUBISHI_6G75",11="VVT_NISSAN_MR",9="VVT_NISSAN_VQ",1="VVT_SINGLE_TOOTH",2="VVT_TOYOTA_3_TOOTH",6="VVT_TOYOTA_4_1"
#define vvt_mode_e_enum "Inactive", "Single Tooth", "Toyota 3 Tooth", "Miata NB2", "Mitsu 4G69", "Bosch Quick Start", "4/1", "ST 170", "Ford Barra 3+1", "Nissan VQ", "Honda K Intake", "Nissan MR18", "Mitsu 3A92", "VTwin by MAP", "Mitsu 6G75", "Mazda Skyactiv", "Honda K Exhaust", "Mitsubishi 4G92/93/94", "Mitsubishi 4G63", "Ford Coyote", "Mitsu 6G72", "Honda 600", "vvt22", "vvt23"
#define vvt_mode_e_VVT_BARRA_3_PLUS_1 8
#define vvt_mode_e_VVT_BOSCH_QUICK_START 5
#define vvt_mode_e_VVT_FORD_COYOTE 19
#define vvt_mode_e_VVT_FORD_ST170 7
#define vvt_mode_e_VVT_HONDA_CBR_600 21
#define vvt_mode_e_VVT_HONDA_K_EXHAUST 16
#define vvt_mode_e_VVT_HONDA_K_INTAKE 10
#define vvt_mode_e_VVT_INACTIVE 0
#define vvt_mode_e_VVT_MAP_V_TWIN 13
#define vvt_mode_e_VVT_MAZDA_SKYACTIV 15
#define vvt_mode_e_VVT_MIATA_NB 3
#define vvt_mode_e_VVT_MITSUBISHI_3A92 12
#define vvt_mode_e_VVT_MITSUBISHI_4G63 18
#define vvt_mode_e_VVT_MITSUBISHI_4G69 4
#define vvt_mode_e_VVT_MITSUBISHI_4G9x 17
#define vvt_mode_e_VVT_MITSUBISHI_6G72 20
#define vvt_mode_e_VVT_MITSUBISHI_6G75 14
#define vvt_mode_e_VVT_NISSAN_MR 11
#define vvt_mode_e_VVT_NISSAN_VQ 9
#define vvt_mode_e_VVT_SINGLE_TOOTH 1
#define vvt_mode_e_VVT_TOYOTA_3_TOOTH 2
#define vvt_mode_e_VVT_TOYOTA_4_1 6
#define VVT_TRACKING_LENGTH 4
#define WARNING_BUFFER_SIZE 120
#define WWAE_TABLE_SIZE 8
