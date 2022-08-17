package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Wed Aug 17 04:29:36 UTC 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TsOutputs {
	public static final int ADC_CHANNEL_NONE = 0;
	public static final int AFTERSTART_DECAY_CURVE_SIZE = 8;
	public static final int AFTERSTART_ENRICH_CURVE_SIZE = 8;
	public static final int AFTERSTART_HOLD_CURVE_SIZE = 8;
	public static final int AUX_ANALOG_INPUT_COUNT = 8;
	public static final int AUX_DIGITAL_VALVE_COUNT = 2;
	public static final int BANKS_COUNT = 2;
	public static final int BARO_CORR_SIZE = 4;
	public static final int BLOCKING_FACTOR = 500;
	public static final int BOOST_LOAD_COUNT = 8;
	public static final int BOOST_RPM_COUNT = 8;
	public static final int CAM_INPUTS_COUNT = 4;
	public static final int CAMS_PER_BANK = 2;
	public static final int CAN_DEFAULT_BASE = 0x200;
	public static final int CAN_ECU_SERIAL_RX_ID = 0x100;
	public static final int CAN_ECU_SERIAL_TX_ID = 0x102;
	public static final String CAN_RX_PREFIX = "CAN_rx";
	public static final int CLT_CRANKING_CURVE_SIZE = 8;
	public static final int CLT_CURVE_SIZE = 16;
	public static final int CLT_LIMITER_CURVE_SIZE = 4;
	public static final int CLT_TIMING_CURVE_SIZE = 8;
	public static final String CMD_AC_RELAY_BENCH = "acrelaybench";
	public static final String CMD_ALTERNATOR_PIN = "set_alternator_pin";
	public static final String CMD_BOOST_PIN = "set_boost_pin";
	public static final String CMD_BURNCONFIG = "burnconfig";
	public static final String CMD_DATE = "date";
	public static final String CMD_DISABLE = "disable";
	public static final String CMD_ECU_UNLOCK = "unlock";
	public static final String CMD_ENABLE = "enable";
	public static final String CMD_ENGINE_TYPE = "engine_type";
	public static final String CMD_ENGINESNIFFERRPMTHRESHOLD = "engineSnifferRpmThreshold";
	public static final String CMD_ETB_DUTY = "set_etb_duty";
	public static final String CMD_EXTERNAL_STIMULATION = "ext_stimulation";
	public static final String CMD_FAN2_BENCH = "fan2bench";
	public static final String CMD_FAN_BENCH = "fanbench";
	public static final String CMD_FUEL_BENCH = "fuelbench";
	public static final String CMD_FUEL_PUMP_BENCH = "fuelpumpbench";
	public static final String CMD_FUNCTIONAL_TEST_MODE = "test_mode";
	public static final String CMD_GET = "get";
	public static final String CMD_HPFP_BENCH = "hpfpbench";
	public static final String CMD_IDLE_BENCH = "idlebench";
	public static final String CMD_IDLE_PIN = "set_idle_pin";
	public static final String CMD_IGNITION = "ignition";
	public static final String CMD_IGNITION_PIN = "set_ignition_pin";
	public static final String CMD_INJECTION = "injection";
	public static final String CMD_INJECTION_PIN = "set_injection_pin";
	public static final int CMD_KEYPAD = 0x17;
	public static final String CMD_LOGIC_PIN = "set_logic_input_pin";
	public static final String CMD_MIL_BENCH = "milbench";
	public static final String CMD_PINS = "pins";
	public static final String CMD_PWM = "pwm";
	public static final String CMD_REBOOT = "reboot";
	public static final String CMD_REBOOT_DFU = "reboot_dfu";
	public static final String CMD_REBOOT_OPENBLT = "reboot_openblt";
	public static final String CMD_RESET_ENGINE_SNIFFER = "reset_engine_chart";
	public static final String CMD_RPM = "rpm";
	public static final String CMD_SELF_STIMULATION = "self_stimulation";
	public static final String CMD_SET = "set";
	public static final String CMD_SPARK_BENCH = "sparkbench";
	public static final String CMD_STARTER_BENCH = "starterbench";
	public static final String CMD_TRIGGER_HW_INPUT = "trigger_hw_input";
	public static final String CMD_TRIGGER_PIN = "set_trigger_input_pin";
	public static final String CMD_TRIGGER_SIMULATOR_PIN = "set_trigger_simulator_pin";
	public static final String CMD_TRIGGERINFO = "triggerinfo";
	public static final int CMD_TS_BENCH_CATEGORY = 22;
	public static final int CMD_TS_LUA_OUTPUT_CATEGORY = 33;
	public static final int CMD_TS_SOLENOID_CATEGORY = 41;
	public static final int CMD_TS_WBO_UPDATE = 0x15;
	public static final int CMD_TS_X14 = 20;
	public static final int CMD_TS_X31 = 49;
	public static final String CMD_VSS_PIN = "vss_pin";
	public static final String CMD_WRITECONFIG = "writeconfig";
	public static final int COMPOSITE_PACKET_COUNT = 1000;
	public static final int COMPOSITE_PACKET_SIZE = 5;
	public static final String CONSOLE_DATA_PROTOCOL_TAG = " @";
	public static final int CRANKING_ADVANCE_CURVE_SIZE = 4;
	public static final int CRANKING_CLT_IDLE_CURVE_SIZE = 8;
	public static final int CRANKING_CURVE_SIZE = 8;
	public static final String CRITICAL_PREFIX = "CRITICAL";
	public static final int DC_PER_STEPPER = 2;
	public static final int DIGIPOT_COUNT = 4;
	public static final int DWELL_CURVE_SIZE = 8;
	public static final int EGT_CHANNEL_COUNT = 8;
	public static final String ENGINE_MAKE_BMW = "BMW";
	public static final String ENGINE_MAKE_GM = "GM";
	public static final String ENGINE_MAKE_HONDA = "Honda";
	public static final String ENGINE_MAKE_Hyundai = "Hyundai";
	public static final String ENGINE_MAKE_LADA = "Lada";
	public static final String ENGINE_MAKE_MAZDA = "Mazda";
	public static final String ENGINE_MAKE_MERCEDES = "Mercedes";
	public static final String ENGINE_MAKE_NISSAN = "Nissan";
	public static final String ENGINE_MAKE_SUBARU = "Subaru";
	public static final String ENGINE_MAKE_TOYOTA = "Toyota";
	public static final String ENGINE_MAKE_VAG = "VAG";
	public static final int ENGINE_NOISE_CURVE_SIZE = 16;
	public static final int ENGINE_SNIFFER_UNIT_US = 10;
	public static final int ERROR_BUFFER_SIZE = 120;
	public static final int ETB_BIAS_CURVE_LENGTH = 8;
	public static final int ETB_COUNT = 2;
	public static final int ETB_HW_MAX_FREQUENCY = 3000;
	public static final int FLASH_DATA_VERSION = 10013;
	public static final int FUEL_LEVEL_TABLE_COUNT = 8;
	public static final int FUEL_LOAD_COUNT = 16;
	public static final int FUEL_RPM_COUNT = 16;
	public static final int GAP_TRACKING_LENGTH = 18;
	public static final String GAUGE_COIL_DWELL_TIME = "dwell: coil charge time";
	public static final String GAUGE_NAME_ACCEL_ROLL = "Acceleration: Roll";
	public static final String GAUGE_NAME_ACCEL_X = "Acceleration: X";
	public static final String GAUGE_NAME_ACCEL_Y = "Acceleration: Y";
	public static final String GAUGE_NAME_ACCEL_YAW = "Acceleration: Yaw";
	public static final String GAUGE_NAME_ACCEL_Z = "Acceleration: Z";
	public static final String GAUGE_NAME_AFR = "Air/Fuel Ratio";
	public static final String GAUGE_NAME_AFR2 = "Air/Fuel Ratio 2";
	public static final String GAUGE_NAME_AIR_FLOW_ESTIMATE = "MAF estimate";
	public static final String GAUGE_NAME_AIR_FLOW_MEASURED = "MAF sensor";
	public static final String GAUGE_NAME_AIR_MASS = "air mass";
	public static final String GAUGE_NAME_AUX_LINEAR_1 = "Aux Linear 1";
	public static final String GAUGE_NAME_AUX_LINEAR_2 = "Aux Linear 2";
	public static final String GAUGE_NAME_AUX_TEMP1 = "Aux temp 1";
	public static final String GAUGE_NAME_AUX_TEMP2 = "Aux temp 2";
	public static final String GAUGE_NAME_BARO_PRESSURE = "Barometric pressure";
	public static final String GAUGE_NAME_BOOST_CLOSED_LOOP = "Boost Closed Loop";
	public static final String GAUGE_NAME_BOOST_OPEN_LOOP = "Boost Open Loop";
	public static final String GAUGE_NAME_BOOST_OUTPUT = "Boost Output";
	public static final String GAUGE_NAME_BOOST_TARGET = "Boost Target";
	public static final String GAUGE_NAME_CLT = "CLT";
	public static final String GAUGE_NAME_CPU_TEMP = "CPU Temperature";
	public static final String GAUGE_NAME_CURRENT_GEAR = "Current Gear";
	public static final String GAUGE_NAME_DEBUG_F1 = "debug f1";
	public static final String GAUGE_NAME_DEBUG_F2 = "debug f2: iTerm";
	public static final String GAUGE_NAME_DEBUG_F3 = "debug f3: prevError";
	public static final String GAUGE_NAME_DEBUG_F4 = "debug f4: iParam";
	public static final String GAUGE_NAME_DEBUG_F5 = "debug f5: dParam";
	public static final String GAUGE_NAME_DEBUG_F6 = "debug f6: dTerm";
	public static final String GAUGE_NAME_DEBUG_F7 = "debug f7";
	public static final String GAUGE_NAME_DEBUG_I1 = "debug i1: pParam";
	public static final String GAUGE_NAME_DEBUG_I2 = "debug i2: offset";
	public static final String GAUGE_NAME_DEBUG_I3 = "debug i3";
	public static final String GAUGE_NAME_DEBUG_I4 = "debug i4";
	public static final String GAUGE_NAME_DEBUG_I5 = "debug i5";
	public static final String GAUGE_NAME_DESIRED_GEAR = "Desired Gear";
	public static final String GAUGE_NAME_DETECTED_GEAR = "Detected Gear";
	public static final String GAUGE_NAME_DWELL_DUTY = "dwell: coil duty cycle";
	public static final String GAUGE_NAME_ECU_TEMPERATURE = "ECU temperature";
	public static final String GAUGE_NAME_ENGINE_CRC16 = "Engine CRC16";
	public static final String GAUGE_NAME_ETB_DUTY = "ETB Duty";
	public static final String GAUGE_NAME_ETB_ERROR = "ETB position error";
	public static final String GAUGE_NAME_ETB_TARGET = "ETB position target";
	public static final String GAUGE_NAME_FLEX = "Flex Ethanol %";
	public static final String GAUGE_NAME_FUEL_BARO_CORR = "fuel: Barometric pressure correction";
	public static final String GAUGE_NAME_FUEL_BASE = "fuel: base mass";
	public static final String GAUGE_NAME_FUEL_CHARGE_TEMP = "fuel: Estimated charge temperature";
	public static final String GAUGE_NAME_FUEL_CLT_CORR = "fuel: CLT correction";
	public static final String GAUGE_NAME_FUEL_CONSUMPTION = "fuel: Total consumed";
	public static final String GAUGE_NAME_FUEL_CRANKING = "fuel: cranking";
	public static final String GAUGE_NAME_FUEL_EL_EXTRA = "fuel: engine load acceleration extra fuel";
	public static final String GAUGE_NAME_FUEL_FLOW = "fuel: Flow rate";
	public static final String GAUGE_NAME_FUEL_IAT_CORR = "fuel: IAT correction";
	public static final String GAUGE_NAME_FUEL_INJ_DUTY = "fuel: injector duty cycle";
	public static final String GAUGE_NAME_FUEL_LAST_INJECTION = "fuel: Last injection";
	public static final String GAUGE_NAME_FUEL_LEVEL = "fuel level";
	public static final String GAUGE_NAME_FUEL_LOAD = "fuel: load";
	public static final String GAUGE_NAME_FUEL_PRESSURE_HIGH = "Fuel pressure (high)";
	public static final String GAUGE_NAME_FUEL_PRESSURE_HIGH_UNITS = "bar";
	public static final String GAUGE_NAME_FUEL_PRESSURE_LOW = "Fuel pressure (low)";
	public static final String GAUGE_NAME_FUEL_PRESSURE_LOW_UNITS = "kPa";
	public static final String GAUGE_NAME_FUEL_RUNNING = "fuel: running";
	public static final String GAUGE_NAME_FUEL_TPS_EXTRA = "fuel: TPS acceleration extra fuel ms";
	public static final String GAUGE_NAME_FUEL_TPS_ROC = "fuel: TPS change";
	public static final String GAUGE_NAME_FUEL_TRIM = "fuel: fuel trim";
	public static final String GAUGE_NAME_FUEL_TRIM_2 = "fuel: fuel trim 2";
	public static final String GAUGE_NAME_FUEL_VE = "fuel: VE";
	public static final String GAUGE_NAME_FUEL_WALL_AMOUNT = "fuel: wall amount";
	public static final String GAUGE_NAME_FUEL_WALL_CORRECTION = "fuel: wall correction";
	public static final String GAUGE_NAME_FW_VERSION = "ECU Software Version";
	public static final String GAUGE_NAME_GEAR_RATIO = "Gearbox Ratio";
	public static final String GAUGE_NAME_IAC = "Idle Air Valve";
	public static final String GAUGE_NAME_IAT = "IAT";
	public static final String GAUGE_NAME_IDLE_POSITION = "Idle position sensor";
	public static final String GAUGE_NAME_IGNITION_LOAD = "ign: load";
	public static final String GAUGE_NAME_INJECTOR_LAG = "fuel: injector lag";
	public static final String GAUGE_NAME_ISS = "Input Shaft Speed";
	public static final String GAUGE_NAME_KNOCK_1 = "knock 1";
	public static final String GAUGE_NAME_KNOCK_10 = "knock 10";
	public static final String GAUGE_NAME_KNOCK_11 = "knock 11";
	public static final String GAUGE_NAME_KNOCK_12 = "knock 12";
	public static final String GAUGE_NAME_KNOCK_2 = "knock 2";
	public static final String GAUGE_NAME_KNOCK_3 = "knock 3";
	public static final String GAUGE_NAME_KNOCK_4 = "knock 4";
	public static final String GAUGE_NAME_KNOCK_5 = "knock 5";
	public static final String GAUGE_NAME_KNOCK_6 = "knock 6";
	public static final String GAUGE_NAME_KNOCK_7 = "knock 7";
	public static final String GAUGE_NAME_KNOCK_8 = "knock 8";
	public static final String GAUGE_NAME_KNOCK_9 = "knock 9";
	public static final String GAUGE_NAME_KNOCK_COUNTER = "knock: count";
	public static final String GAUGE_NAME_KNOCK_LEVEL = "knock: current level";
	public static final String GAUGE_NAME_KNOCK_RETARD = "knock: retard";
	public static final String GAUGE_NAME_LAMBDA = "Lambda";
	public static final String GAUGE_NAME_LAMBDA2 = "Lambda 2";
	public static final String GAUGE_NAME_LAST_ERROR = "Last error";
	public static final String GAUGE_NAME_MAF = "MAF";
	public static final String GAUGE_NAME_MAP = "MAP";
	public static final String GAUGE_NAME_OIL_PRESSURE = "Oil Pressure";
	public static final String GAUGE_NAME_OIL_PRESSURE_UNITS = "kPa";
	public static final String GAUGE_NAME_RAW_FUEL_PRESSURE_HIGH = "Fuel pressure raw (high)";
	public static final String GAUGE_NAME_RAW_FUEL_PRESSURE_LOW = "Fuel pressure raw (low)";
	public static final String GAUGE_NAME_RPM = "RPM";
	public static final String GAUGE_NAME_TARGET_AFR = "fuel: target AFR";
	public static final String GAUGE_NAME_TARGET_LAMBDA = "fuel: target lambda";
	public static final String GAUGE_NAME_TC_RATIO = "Torque Converter Ratio";
	public static final String GAUGE_NAME_TCHARGE = "fuel: SD tCharge";
	public static final String GAUGE_NAME_THROTTLE_PEDAL = "Throttle pedal position";
	public static final String GAUGE_NAME_TIME = "Time";
	public static final String GAUGE_NAME_TIMING_ADVANCE = "timing";
	public static final String GAUGE_NAME_TPS = "TPS";
	public static final String GAUGE_NAME_TPS2 = "TPS2";
	public static final String GAUGE_NAME_TRG_ERR = "Trigger Error Counter";
	public static final String GAUGE_NAME_TRG_GAP = "Trigger Sync Latest Ratio";
	public static final String GAUGE_NAME_TUNE_CRC16 = "Tune CRC16";
	public static final String GAUGE_NAME_TURBO_SPEED = "Turbocharger Speed";
	public static final String GAUGE_NAME_UPTIME = "Uptime";
	public static final String GAUGE_NAME_VBAT = "VBatt";
	public static final String GAUGE_NAME_VERSION = "firmware";
	public static final String GAUGE_NAME_VVS = "Vehicle Speed";
	public static final String GAUGE_NAME_VVT_B1E = "VVT: bank 1 exhaust";
	public static final String GAUGE_NAME_VVT_B1I = "VVT: bank 1 intake";
	public static final String GAUGE_NAME_VVT_B2E = "VVT: bank 2 exhaust";
	public static final String GAUGE_NAME_VVT_B2I = "VVT: bank 2 intake";
	public static final String GAUGE_NAME_VVT_TARGET_B1E = "VVT: bank 1 exhaust target";
	public static final String GAUGE_NAME_VVT_TARGET_B1I = "VVT: bank 1 intake target";
	public static final String GAUGE_NAME_VVT_TARGET_B2E = "VVT: bank 2 exhaust target";
	public static final String GAUGE_NAME_VVT_TARGET_B2I = "VVT: bank 2 intake target";
	public static final String GAUGE_NAME_WARNING_COUNT = "Warning count";
	public static final String GAUGE_NAME_WARNING_COUNTER = "warning: counter";
	public static final String GAUGE_NAME_WARNING_LAST = "warning: last";
	public static final String GAUGE_NAME_WG_POSITION = "Wastegate position sensor";
	public static final int GEARS_COUNT = 8;
	public static final int GPPWM_CHANNELS = 4;
	public static final int GPPWM_LOAD_COUNT = 8;
	public static final int GPPWM_NOTE_SIZE = 16;
	public static final int GPPWM_RPM_COUNT = 8;
	public static final int HPFP_COMPENSATION_SIZE = 10;
	public static final int HPFP_DEADTIME_SIZE = 8;
	public static final int HPFP_LOBE_PROFILE_SIZE = 16;
	public static final int HPFP_TARGET_SIZE = 10;
	public static final int HW_MAX_ADC_INDEX = 17;
	public static final int IAC_PID_MULT_SIZE = 8;
	public static final int IAT_CURVE_SIZE = 16;
	public static final int IDLE_ADVANCE_CURVE_SIZE = 8;
	public static final int IDLE_VE_SIZE = 4;
	public static final int IGN_LOAD_COUNT = 16;
	public static final int IGN_RPM_COUNT = 16;
	public static final String INDICATOR_NAME_AC_SWITCH = "AC switch";
	public static final String INDICATOR_NAME_BRAKE_DOWN = "brake: down";
	public static final String INDICATOR_NAME_CLUTCH_DOWN = "clutch: down";
	public static final String INDICATOR_NAME_CLUTCH_UP = "clutch: up";
	public static final int LE_COMMAND_LENGTH = 200;
	public static final String LOG_DELIMITER = "`";
	public static final int LOGIC_ANALYZER_CHANNEL_COUNT = 4;
	public static final int LUA_ANALOG_INPUT_COUNT = 8;
	public static final int LUA_PWM_COUNT = 8;
	public static final int LUA_SCRIPT_SIZE = 8000;
	public static final int MAF_DECODING_COUNT = 256;
	public static final String MAIN_HELP_URL = "http://www.rusefi.com/";
	public static final int MAP_ANGLE_SIZE = 8;
	public static final int MAP_WINDOW_SIZE = 8;
	public static final int MAX_CYLINDER_COUNT = 12;
	public static final int MLQ_FIELD_HEADER_SIZE = 55;
	public static final int MLQ_HEADER_SIZE = 22;
	public static final String MOCK_PPS_POSITION_COMMAND = "mock_pps_position";
	public static final String MOCK_PPS_VOLTAGE_COMMAND = "mock_pps_voltage";
	public static final int NARROW_BAND_WIDE_BAND_CONVERSION_SIZE = 8;
	public static final int PACK_ADD_TEMPERATURE = 40;
	public static final int PACK_MULT_AFR = 1000;
	public static final int PACK_MULT_AFR_CFG = 10;
	public static final int PACK_MULT_ANGLE = 50;
	public static final int PACK_MULT_FUEL_FLOW = 200;
	public static final int PACK_MULT_FUEL_MASS = 100;
	public static final int PACK_MULT_HIGH_PRESSURE = 10;
	public static final int PACK_MULT_LAMBDA = 10000;
	public static final int PACK_MULT_LAMBDA_CFG = 147;
	public static final int PACK_MULT_MASS_FLOW = 10;
	public static final int PACK_MULT_MS = 300;
	public static final int PACK_MULT_PERCENT = 100;
	public static final int PACK_MULT_PRESSURE = 30;
	public static final int PACK_MULT_TEMPERATURE = 100;
	public static final int PACK_MULT_VOLTAGE = 1000;
	public static final int PEDAL_TO_TPS_SIZE = 8;
	public static final String PROTOCOL_ANALOG_CHART = "analog_chart";
	public static final String PROTOCOL_COIL1_SHORT_NAME = "c1";
	public static final String PROTOCOL_CRANK1 = "t1";
	public static final String PROTOCOL_CRANK2 = "t2";
	public static final String PROTOCOL_CRANK3 = "t3";
	public static final String PROTOCOL_ENGINE_SNIFFER = "wave_chart";
	public static final String PROTOCOL_ES_DOWN = "d";
	public static final String PROTOCOL_ES_UP = "u";
	public static final String PROTOCOL_HELLO_PREFIX = "***";
	public static final String PROTOCOL_HIP_NAME = "HIP";
	public static final String PROTOCOL_HPFP_NAME = "hpfp";
	public static final String PROTOCOL_INJ1_SHORT_NAME = "i1";
	public static final String PROTOCOL_MSG = "msg";
	public static final String PROTOCOL_OUTPIN = "outpin";
	public static final String PROTOCOL_SIGNATURE_PREFIX = "rusEFI ";
	public static final String PROTOCOL_TACH_NAME = "tach";
	public static final String PROTOCOL_TEST_RESPONSE_TAG = "ts_p_alive";
	public static final String PROTOCOL_VERSION_TAG = "rusEfiVersion";
	public static final String PROTOCOL_VVT1_NAME = "VVT1";
	public static final String PROTOCOL_VVT2_NAME = "VVT2";
	public static final String PROTOCOL_VVT3_NAME = "VVT3";
	public static final String PROTOCOL_VVT4_NAME = "VVT4";
	public static final String PROTOCOL_WA_CHANNEL_1 = "input1";
	public static final String PROTOCOL_WA_CHANNEL_2 = "input2";
	public static final String PROTOCOL_WA_CHANNEL_3 = "input3";
	public static final String PROTOCOL_WA_CHANNEL_4 = "input4";
	public static final int SCRIPT_CURVE_16 = 16;
	public static final int SCRIPT_CURVE_8 = 8;
	public static final int SCRIPT_CURVE_COUNT = 6;
	public static final int SCRIPT_SETTING_COUNT = 8;
	public static final int SCRIPT_TABLE_8 = 8;
	public static final int SCRIPT_TABLE_COUNT = 4;
	public static final int SERVO_COUNT = 8;
	public static final int STFT_BANK_COUNT = 2;
	public static final int STFT_CELL_COUNT = 4;
	public static final int TCU_GEAR_COUNT = 10;
	public static final int TCU_MAGIC_SIZE = 8;
	public static final int TCU_SOLENOID_COUNT = 6;
	public static final int TOOTH_DATA_LENGTH = 2000;
	public static final int TOOTH_PACKET_COUNT = 1000;
	public static final int TOOTH_PACKET_SIZE = 2;
	public static final String TOP_DEAD_CENTER_MESSAGE = "r";
	public static final int TORQUE_CURVE_SIZE = 6;
	public static final int TPS_2_BYTE_PACKING_MULT = 100;
	public static final int TPS_TPS_ACCEL_TABLE = 8;
	public static final String TRIGGER_COMMENT = "#";
	public static final String TRIGGER_GAP_FROM = "gapFrom";
	public static final String TRIGGER_GAP_TO = "gapTo";
	public static final String TRIGGER_GAPS_COUNT = "gapsCount";
	public static final String TRIGGER_HARDCODED_OPERATION_MODE = "knownOperationMode";
	public static final String TRIGGER_HAS_SECOND_CHANNEL = "hasSecondChannel";
	public static final int TRIGGER_INPUT_PIN_COUNT = 2;
	public static final String TRIGGER_IS_CRANK_KEY = "crankBased";
	public static final String TRIGGER_IS_SECOND_WHEEL_CAM = "isSecondWheelCam";
	public static final int TRIGGER_SIMULATOR_PIN_COUNT = 3;
	public static final String TRIGGERS_FILE_NAME = "triggers.txt";
	public static final int TRIM_SIZE = 4;
	public static final char TS_BURN_COMMAND = 'B';
	public static final char TS_CHUNK_WRITE_COMMAND = 'C';
	public static final char TS_COMMAND_F = 'F';
	public static final int TS_COMPOSITE_DISABLE = 2;
	public static final int TS_COMPOSITE_ENABLE = 1;
	public static final char TS_CRC_CHECK_COMMAND = 'k';
	public static final char TS_EXECUTE = 'E';
	public static final int TS_FILE_VERSION = 20210312;
	public static final int TS_FILE_VERSION_OFFSET = 124;
	public static final char TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY = '8';
	public static final char TS_GET_CONFIG_ERROR = 'e';
	public static final char TS_GET_FIRMWARE_VERSION = 'V';
	public static final char TS_GET_LOGGER_GET_BUFFER = 'L';
	public static final char TS_GET_OUTPUTS_SIZE = '4';
	public static final char TS_GET_PROTOCOL_VERSION_COMMAND_F = 'F';
	public static final char TS_GET_TEXT = 'G';
	public static final char TS_HELLO_COMMAND = 'S';
	public static final char TS_IO_TEST_COMMAND = 'Z';
	public static final char TS_ONLINE_PROTOCOL = 'z';
	public static final char TS_OUTPUT_COMMAND = 'O';
	public static final char TS_PAGE_COMMAND = 'P';
	public static final char TS_PERF_TRACE_BEGIN = '_';
	public static final char TS_PERF_TRACE_GET_BUFFER = 'b';
	public static final String TS_PROTOCOL = "001";
	public static final char TS_QUERY_COMMAND = 'Q';
	public static final char TS_READ_COMMAND = 'R';
	public static final int TS_RESPONSE_BURN_OK = 4;
	public static final int TS_RESPONSE_COMMAND_OK = 7;
	public static final int TS_RESPONSE_CRC_FAILURE = 0x82;
	public static final int TS_RESPONSE_FRAMING_ERROR = 0x8D;
	public static final int TS_RESPONSE_OK = 0;
	public static final int TS_RESPONSE_OUT_OF_RANGE = 0x84;
	public static final int TS_RESPONSE_UNDERRUN = 0x80;
	public static final int TS_RESPONSE_UNRECOGNIZED_COMMAND = 0x83;
	public static final char TS_SET_LOGGER_SWITCH = 'l';
	public static final char TS_SINGLE_WRITE_COMMAND = 'W';
	public static final char TS_TEST_COMMAND = 't';
	public static final int VBAT_INJECTOR_CURVE_SIZE = 8;
	public static final int VEHICLE_INFO_SIZE = 32;
	public static final int VM_VVT_INACTIVE = 0;
	public static final int VR_THRESHOLD_COUNT = 2;
	public static final int WWAE_TABLE_SIZE = 8;
	public static final Field SD_PRESENT = Field.create("SD_PRESENT", 0, FieldType.BIT, 0);
	public static final Field ISIGNITIONENABLEDINDICATOR = Field.create("ISIGNITIONENABLEDINDICATOR", 0, FieldType.BIT, 1);
	public static final Field ISINJECTIONENABLEDINDICATOR = Field.create("ISINJECTIONENABLEDINDICATOR", 0, FieldType.BIT, 2);
	public static final Field SD_LOGGING_INTERNAL = Field.create("SD_LOGGING_INTERNAL", 0, FieldType.BIT, 3);
	public static final Field UNUSEDB4 = Field.create("UNUSEDB4", 0, FieldType.BIT, 4);
	public static final Field ISFUELPUMPON = Field.create("ISFUELPUMPON", 0, FieldType.BIT, 5);
	public static final Field ISFANON = Field.create("ISFANON", 0, FieldType.BIT, 6);
	public static final Field ISO2HEATERON = Field.create("ISO2HEATERON", 0, FieldType.BIT, 7);
	public static final Field CHECKENGINE = Field.create("CHECKENGINE", 0, FieldType.BIT, 8);
	public static final Field NEEDBURN = Field.create("NEEDBURN", 0, FieldType.BIT, 9);
	public static final Field SD_MSD = Field.create("SD_MSD", 0, FieldType.BIT, 10);
	public static final Field ISFAN2ON = Field.create("ISFAN2ON", 0, FieldType.BIT, 11);
	public static final Field ALTERNATORONOFF = Field.create("ALTERNATORONOFF", 0, FieldType.BIT, 12);
	public static final Field TOOTHLOGREADY = Field.create("TOOTHLOGREADY", 0, FieldType.BIT, 13);
	public static final Field UNUSED3123 = Field.create("UNUSED3123", 0, FieldType.BIT, 14);
	public static final Field ISTPSERROR = Field.create("ISTPSERROR", 0, FieldType.BIT, 15);
	public static final Field ISCLTERROR = Field.create("ISCLTERROR", 0, FieldType.BIT, 16);
	public static final Field ISMAPERROR = Field.create("ISMAPERROR", 0, FieldType.BIT, 17);
	public static final Field ISIATERROR = Field.create("ISIATERROR", 0, FieldType.BIT, 18);
	public static final Field UNUSED131313 = Field.create("UNUSED131313", 0, FieldType.BIT, 19);
	public static final Field ISTRIGGERERROR = Field.create("ISTRIGGERERROR", 0, FieldType.BIT, 20);
	public static final Field HASCRITICALERROR = Field.create("HASCRITICALERROR", 0, FieldType.BIT, 21);
	public static final Field ISWARNNOW = Field.create("ISWARNNOW", 0, FieldType.BIT, 22);
	public static final Field ISPEDALERROR = Field.create("ISPEDALERROR", 0, FieldType.BIT, 23);
	public static final Field ISKNOCKCHIPOK = Field.create("ISKNOCKCHIPOK", 0, FieldType.BIT, 24);
	public static final Field LAUNCHTRIGGERED = Field.create("LAUNCHTRIGGERED", 0, FieldType.BIT, 25);
	public static final Field ISTPS2ERROR = Field.create("ISTPS2ERROR", 0, FieldType.BIT, 26);
	public static final Field ISIDLECLOSEDLOOP = Field.create("ISIDLECLOSEDLOOP", 0, FieldType.BIT, 27);
	public static final Field ISIDLECOASTING = Field.create("ISIDLECOASTING", 0, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_29_29 = Field.create("UNUSEDBIT_29_29", 0, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_29_30 = Field.create("UNUSEDBIT_29_30", 0, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_29_31 = Field.create("UNUSEDBIT_29_31", 0, FieldType.BIT, 31);
	public static final Field RPMVALUE = Field.create("RPMVALUE", 4, FieldType.INT16).setScale(1.0);
	public static final Field RPMACCELERATION = Field.create("RPMACCELERATION", 6, FieldType.INT16).setScale(1.0);
	public static final Field SPEEDTORPMRATIO = Field.create("SPEEDTORPMRATIO", 8, FieldType.INT16).setScale(0.01);
	public static final Field VEHICLESPEEDKPH = Field.create("VEHICLESPEEDKPH", 10, FieldType.INT8).setScale(1.0);
	public static final Field INTERNALMCUTEMPERATURE = Field.create("INTERNALMCUTEMPERATURE", 11, FieldType.INT8).setScale(1.0);
	public static final Field COOLANT = Field.create("COOLANT", 12, FieldType.INT16).setScale(0.01);
	public static final Field INTAKE = Field.create("INTAKE", 14, FieldType.INT16).setScale(0.01);
	public static final Field AUXTEMP1 = Field.create("AUXTEMP1", 16, FieldType.INT16).setScale(0.01);
	public static final Field AUXTEMP2 = Field.create("AUXTEMP2", 18, FieldType.INT16).setScale(0.01);
	public static final Field TPSVALUE = Field.create("TPSVALUE", 20, FieldType.INT16).setScale(0.01);
	public static final Field THROTTLEPEDALPOSITION = Field.create("THROTTLEPEDALPOSITION", 22, FieldType.INT16).setScale(0.01);
	public static final Field TPSADC = Field.create("TPSADC", 24, FieldType.INT16).setScale(1.0);
	public static final Field RAWMAF = Field.create("RAWMAF", 26, FieldType.INT16).setScale(0.001);
	public static final Field MAFMEASURED = Field.create("MAFMEASURED", 28, FieldType.INT16).setScale(0.1);
	public static final Field MAPVALUE = Field.create("MAPVALUE", 30, FieldType.INT16).setScale(0.03333333333333333);
	public static final Field BAROPRESSURE = Field.create("BAROPRESSURE", 32, FieldType.INT16).setScale(0.03333333333333333);
	public static final Field LAMBDAVALUE = Field.create("LAMBDAVALUE", 34, FieldType.INT16).setScale(1.0E-4);
	public static final Field KNOCKRETARD = Field.create("KNOCKRETARD", 36, FieldType.INT8).setScale(0.1);
	public static final Field IDLECURRENTPOSITION = Field.create("IDLECURRENTPOSITION", 37, FieldType.INT8).setScale(0.5);
	public static final Field VBATT = Field.create("VBATT", 38, FieldType.INT16).setScale(0.001);
	public static final Field OILPRESSURE = Field.create("OILPRESSURE", 40, FieldType.INT16).setScale(0.03333333333333333);
	public static final Field VVTPOSITIONB1I = Field.create("VVTPOSITIONB1I", 42, FieldType.INT16).setScale(0.02);
	public static final Field CHARGEAIRMASS = Field.create("CHARGEAIRMASS", 44, FieldType.INT16).setScale(0.001);
	public static final Field CRANKINGFUELMS = Field.create("CRANKINGFUELMS", 46, FieldType.INT16).setScale(0.01);
	public static final Field CURRENTTARGETAFR = Field.create("CURRENTTARGETAFR", 48, FieldType.INT16).setScale(0.001);
	public static final Field BASEFUEL = Field.create("BASEFUEL", 50, FieldType.INT16).setScale(0.01);
	public static final Field FUELRUNNING = Field.create("FUELRUNNING", 52, FieldType.INT16).setScale(0.01);
	public static final Field ACTUALLASTINJECTION = Field.create("ACTUALLASTINJECTION", 54, FieldType.INT16).setScale(0.0033333333333333335);
	public static final Field INJECTORDUTYCYCLE = Field.create("INJECTORDUTYCYCLE", 56, FieldType.INT8).setScale(0.5);
	public static final Field VEVALUE = Field.create("VEVALUE", 57, FieldType.INT8).setScale(0.5);
	public static final Field INJECTIONOFFSET = Field.create("INJECTIONOFFSET", 58, FieldType.INT16).setScale(1.0);
	public static final Field TCHARGE = Field.create("TCHARGE", 60, FieldType.INT16).setScale(0.01);
	public static final Field INJECTORLAGMS = Field.create("INJECTORLAGMS", 62, FieldType.INT16).setScale(0.0033333333333333335);
	public static final Field IATCORRECTION = Field.create("IATCORRECTION", 64, FieldType.INT16).setScale(0.01);
	public static final Field CLTCORRECTION = Field.create("CLTCORRECTION", 66, FieldType.INT16).setScale(0.01);
	public static final Field BAROCORRECTION = Field.create("BAROCORRECTION", 68, FieldType.INT16).setScale(0.01);
	public static final Field CURRENTENGINEPHASE = Field.create("CURRENTENGINEPHASE", 70, FieldType.INT16).setScale(1.0);
	public static final Field WALLFUELAMOUNT = Field.create("WALLFUELAMOUNT", 72, FieldType.INT16).setScale(0.01);
	public static final Field WALLFUELCORRECTION = Field.create("WALLFUELCORRECTION", 74, FieldType.INT16).setScale(0.01);
	public static final Field REVOLUTIONCOUNTERSINCESTART = Field.create("REVOLUTIONCOUNTERSINCESTART", 76, FieldType.INT16).setScale(1.0);
	public static final Field DELTATPS = Field.create("DELTATPS", 78, FieldType.INT16).setScale(0.01);
	public static final Field CANREADCOUNTER = Field.create("CANREADCOUNTER", 80, FieldType.INT16).setScale(1.0);
	public static final Field TPSACCELFUEL = Field.create("TPSACCELFUEL", 82, FieldType.INT16).setScale(0.0033333333333333335);
	public static final Field IGNITIONADVANCE = Field.create("IGNITIONADVANCE", 84, FieldType.INT16).setScale(0.02);
	public static final Field SPARKDWELLVALUE = Field.create("SPARKDWELLVALUE", 86, FieldType.INT16).setScale(0.0033333333333333335);
	public static final Field COILDUTYCYCLE = Field.create("COILDUTYCYCLE", 88, FieldType.INT16).setScale(0.01);
	public static final Field IDLEAIRVALVEPOSITION = Field.create("IDLEAIRVALVEPOSITION", 90, FieldType.INT16).setScale(0.01);
	public static final Field ETBTARGET = Field.create("ETBTARGET", 92, FieldType.INT16).setScale(0.01);
	public static final Field ETB1DUTYCYCLE = Field.create("ETB1DUTYCYCLE", 94, FieldType.INT16).setScale(0.01);
	public static final Field ETB1ERROR = Field.create("ETB1ERROR", 96, FieldType.INT16).setScale(0.01);
	public static final Field FUELTANKLEVEL = Field.create("FUELTANKLEVEL", 98, FieldType.INT16).setScale(0.01);
	public static final Field TOTALFUELCONSUMPTION = Field.create("TOTALFUELCONSUMPTION", 100, FieldType.INT16).setScale(1.0);
	public static final Field FUELFLOWRATE = Field.create("FUELFLOWRATE", 102, FieldType.INT16).setScale(0.005);
	public static final Field VETABLEYAXIS = Field.create("VETABLEYAXIS", 104, FieldType.INT16).setScale(0.01);
	public static final Field AFRTABLEYAXIS = Field.create("AFRTABLEYAXIS", 106, FieldType.INT16).setScale(0.01);
	public static final Field KNOCKLEVEL = Field.create("KNOCKLEVEL", 108, FieldType.FLOAT);
	public static final Field SECONDS = Field.create("SECONDS", 112, FieldType.INT).setScale(1.0);
	public static final Field ENGINEMODE = Field.create("ENGINEMODE", 116, FieldType.INT).setScale(1.0);
	public static final Field FIRMWAREVERSION = Field.create("FIRMWAREVERSION", 120, FieldType.INT).setScale(1.0);
	public static final Field TSCONFIGVERSION = Field.create("TSCONFIGVERSION", 124, FieldType.INT).setScale(1.0);
	public static final Field CALIBRATIONVALUE = Field.create("CALIBRATIONVALUE", 128, FieldType.FLOAT);
	public static final Field CALIBRATIONMODE = Field.create("CALIBRATIONMODE", 132, FieldType.INT8).setScale(1.0);
	public static final Field IDLETARGETPOSITION = Field.create("IDLETARGETPOSITION", 133, FieldType.INT8).setScale(1.0);
	public static final Field FUELINGLOAD = Field.create("FUELINGLOAD", 134, FieldType.INT16).setScale(0.01);
	public static final Field IGNITIONLOAD = Field.create("IGNITIONLOAD", 136, FieldType.INT16).setScale(0.01);
	public static final Field ENGINEMAKECODENAMECRC16 = Field.create("ENGINEMAKECODENAMECRC16", 138, FieldType.INT16).setScale(1.0);
	public static final Field TOTALTRIGGERERRORCOUNTER = Field.create("TOTALTRIGGERERRORCOUNTER", 140, FieldType.INT).setScale(1.0);
	public static final Field ORDERINGERRORCOUNTER = Field.create("ORDERINGERRORCOUNTER", 144, FieldType.INT).setScale(1.0);
	public static final Field WARNINGCOUNTER = Field.create("WARNINGCOUNTER", 148, FieldType.INT16).setScale(1.0);
	public static final Field LASTERRORCODE = Field.create("LASTERRORCODE", 150, FieldType.INT16).setScale(1.0);
	public static final Field RECENTERRORCODE1 = Field.create("RECENTERRORCODE1", 152, FieldType.INT16).setScale(1.0);
	public static final Field RECENTERRORCODE2 = Field.create("RECENTERRORCODE2", 154, FieldType.INT16).setScale(1.0);
	public static final Field RECENTERRORCODE3 = Field.create("RECENTERRORCODE3", 156, FieldType.INT16).setScale(1.0);
	public static final Field RECENTERRORCODE4 = Field.create("RECENTERRORCODE4", 158, FieldType.INT16).setScale(1.0);
	public static final Field RECENTERRORCODE5 = Field.create("RECENTERRORCODE5", 160, FieldType.INT16).setScale(1.0);
	public static final Field RECENTERRORCODE6 = Field.create("RECENTERRORCODE6", 162, FieldType.INT16).setScale(1.0);
	public static final Field RECENTERRORCODE7 = Field.create("RECENTERRORCODE7", 164, FieldType.INT16).setScale(1.0);
	public static final Field RECENTERRORCODE8 = Field.create("RECENTERRORCODE8", 166, FieldType.INT16).setScale(1.0);
	public static final Field DEBUGFLOATFIELD1 = Field.create("DEBUGFLOATFIELD1", 168, FieldType.FLOAT);
	public static final Field DEBUGFLOATFIELD2 = Field.create("DEBUGFLOATFIELD2", 172, FieldType.FLOAT);
	public static final Field DEBUGFLOATFIELD3 = Field.create("DEBUGFLOATFIELD3", 176, FieldType.FLOAT);
	public static final Field DEBUGFLOATFIELD4 = Field.create("DEBUGFLOATFIELD4", 180, FieldType.FLOAT);
	public static final Field DEBUGFLOATFIELD5 = Field.create("DEBUGFLOATFIELD5", 184, FieldType.FLOAT);
	public static final Field DEBUGFLOATFIELD6 = Field.create("DEBUGFLOATFIELD6", 188, FieldType.FLOAT);
	public static final Field DEBUGFLOATFIELD7 = Field.create("DEBUGFLOATFIELD7", 192, FieldType.FLOAT);
	public static final Field DEBUGINTFIELD1 = Field.create("DEBUGINTFIELD1", 196, FieldType.INT).setScale(1.0);
	public static final Field DEBUGINTFIELD2 = Field.create("DEBUGINTFIELD2", 200, FieldType.INT).setScale(1.0);
	public static final Field DEBUGINTFIELD3 = Field.create("DEBUGINTFIELD3", 204, FieldType.INT).setScale(1.0);
	public static final Field DEBUGINTFIELD4 = Field.create("DEBUGINTFIELD4", 208, FieldType.INT16).setScale(1.0);
	public static final Field DEBUGINTFIELD5 = Field.create("DEBUGINTFIELD5", 210, FieldType.INT16).setScale(1.0);
	public static final Field ACCELERATIONX = Field.create("ACCELERATIONX", 212, FieldType.INT16).setScale(0.01);
	public static final Field ACCELERATIONY = Field.create("ACCELERATIONY", 214, FieldType.INT16).setScale(0.01);
	public static final Field EGT1 = Field.create("EGT1", 216, FieldType.INT16).setScale(1.0);
	public static final Field EGT2 = Field.create("EGT2", 218, FieldType.INT16).setScale(1.0);
	public static final Field EGT3 = Field.create("EGT3", 220, FieldType.INT16).setScale(1.0);
	public static final Field EGT4 = Field.create("EGT4", 222, FieldType.INT16).setScale(1.0);
	public static final Field EGT5 = Field.create("EGT5", 224, FieldType.INT16).setScale(1.0);
	public static final Field EGT6 = Field.create("EGT6", 226, FieldType.INT16).setScale(1.0);
	public static final Field EGT7 = Field.create("EGT7", 228, FieldType.INT16).setScale(1.0);
	public static final Field EGT8 = Field.create("EGT8", 230, FieldType.INT16).setScale(1.0);
	public static final Field TPS2VALUE = Field.create("TPS2VALUE", 232, FieldType.INT16).setScale(0.01);
	public static final Field RAWTPS1PRIMARY = Field.create("RAWTPS1PRIMARY", 234, FieldType.INT16).setScale(0.001);
	public static final Field RAWPPSPRIMARY = Field.create("RAWPPSPRIMARY", 236, FieldType.INT16).setScale(0.001);
	public static final Field RAWCLT = Field.create("RAWCLT", 238, FieldType.INT16).setScale(0.001);
	public static final Field RAWIAT = Field.create("RAWIAT", 240, FieldType.INT16).setScale(0.001);
	public static final Field RAWOILPRESSURE = Field.create("RAWOILPRESSURE", 242, FieldType.INT16).setScale(0.001);
	public static final Field TUNECRC16 = Field.create("TUNECRC16", 244, FieldType.INT16).setScale(1.0);
	public static final Field FUELCLOSEDLOOPBINIDX = Field.create("FUELCLOSEDLOOPBINIDX", 246, FieldType.INT8).setScale(1.0);
	public static final Field TCUCURRENTGEAR = Field.create("TCUCURRENTGEAR", 247, FieldType.INT8).setScale(1.0);
	public static final Field RAWPPSSECONDARY = Field.create("RAWPPSSECONDARY", 248, FieldType.INT16).setScale(0.001);
	public static final Field KNOCK1 = Field.create("KNOCK1", 250, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK2 = Field.create("KNOCK2", 251, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK3 = Field.create("KNOCK3", 252, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK4 = Field.create("KNOCK4", 253, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK5 = Field.create("KNOCK5", 254, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK6 = Field.create("KNOCK6", 255, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK7 = Field.create("KNOCK7", 256, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK8 = Field.create("KNOCK8", 257, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK9 = Field.create("KNOCK9", 258, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK10 = Field.create("KNOCK10", 259, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK11 = Field.create("KNOCK11", 260, FieldType.INT8).setScale(1.0);
	public static final Field KNOCK12 = Field.create("KNOCK12", 261, FieldType.INT8).setScale(1.0);
	public static final Field TCUDESIREDGEAR = Field.create("TCUDESIREDGEAR", 262, FieldType.INT8).setScale(1.0);
	public static final Field FLEXPERCENT = Field.create("FLEXPERCENT", 263, FieldType.INT8).setScale(0.5);
	public static final Field RAWIDLEPOSITIONSENSOR = Field.create("RAWIDLEPOSITIONSENSOR", 264, FieldType.INT16).setScale(0.001);
	public static final Field RAWWASTEGATEPOSITION = Field.create("RAWWASTEGATEPOSITION", 266, FieldType.INT16).setScale(0.001);
	public static final Field WASTEGATEPOSITIONSENSOR = Field.create("WASTEGATEPOSITIONSENSOR", 268, FieldType.INT16).setScale(0.01);
	public static final Field IDLEPOSITIONSENSOR = Field.create("IDLEPOSITIONSENSOR", 270, FieldType.INT16).setScale(0.01);
	public static final Field RAWLOWFUELPRESSURE = Field.create("RAWLOWFUELPRESSURE", 272, FieldType.INT16).setScale(0.001);
	public static final Field RAWHIGHFUELPRESSURE = Field.create("RAWHIGHFUELPRESSURE", 274, FieldType.INT16).setScale(0.001);
	public static final Field LOWFUELPRESSURE = Field.create("LOWFUELPRESSURE", 276, FieldType.INT16).setScale(0.03333333333333333);
	public static final Field HIGHFUELPRESSURE = Field.create("HIGHFUELPRESSURE", 278, FieldType.INT16).setScale(0.1);
	public static final Field TARGETLAMBDA = Field.create("TARGETLAMBDA", 280, FieldType.INT16).setScale(1.0E-4);
	public static final Field AFRVALUE = Field.create("AFRVALUE", 282, FieldType.INT16).setScale(0.001);
	public static final Field VSSACCELERATION = Field.create("VSSACCELERATION", 284, FieldType.INT16).setScale(0.0033333333333333335);
	public static final Field LAMBDAVALUE2 = Field.create("LAMBDAVALUE2", 286, FieldType.INT16).setScale(1.0E-4);
	public static final Field AFRVALUE2 = Field.create("AFRVALUE2", 288, FieldType.INT16).setScale(0.001);
	public static final Field VVTPOSITIONB1E = Field.create("VVTPOSITIONB1E", 290, FieldType.INT16).setScale(0.02);
	public static final Field VVTPOSITIONB2I = Field.create("VVTPOSITIONB2I", 292, FieldType.INT16).setScale(0.02);
	public static final Field VVTPOSITIONB2E = Field.create("VVTPOSITIONB2E", 294, FieldType.INT16).setScale(0.02);
	public static final Field FUELPIDCORRECTION1 = Field.create("FUELPIDCORRECTION1", 296, FieldType.INT16).setScale(0.01);
	public static final Field FUELPIDCORRECTION2 = Field.create("FUELPIDCORRECTION2", 298, FieldType.INT16).setScale(0.01);
	public static final Field RAWTPS1SECONDARY = Field.create("RAWTPS1SECONDARY", 300, FieldType.INT16).setScale(0.001);
	public static final Field RAWTPS2PRIMARY = Field.create("RAWTPS2PRIMARY", 302, FieldType.INT16).setScale(0.001);
	public static final Field RAWTPS2SECONDARY = Field.create("RAWTPS2SECONDARY", 304, FieldType.INT16).setScale(0.001);
	public static final Field KNOCKCOUNT = Field.create("KNOCKCOUNT", 306, FieldType.INT16).setScale(1.0);
	public static final Field ACCELERATIONZ = Field.create("ACCELERATIONZ", 308, FieldType.INT16).setScale(0.01);
	public static final Field ACCELERATIONROLL = Field.create("ACCELERATIONROLL", 310, FieldType.INT16).setScale(0.01);
	public static final Field ACCELERATIONYAW = Field.create("ACCELERATIONYAW", 312, FieldType.INT16).setScale(0.01);
	public static final Field VVTTARGETS1 = Field.create("VVTTARGETS1", 314, FieldType.INT8).setScale(1.0);
	public static final Field VVTTARGETS2 = Field.create("VVTTARGETS2", 315, FieldType.INT8).setScale(1.0);
	public static final Field VVTTARGETS3 = Field.create("VVTTARGETS3", 316, FieldType.INT8).setScale(1.0);
	public static final Field VVTTARGETS4 = Field.create("VVTTARGETS4", 317, FieldType.INT8).setScale(1.0);
	public static final Field TURBOSPEED = Field.create("TURBOSPEED", 318, FieldType.INT16).setScale(1.0);
	public static final Field TPS1SPLIT = Field.create("TPS1SPLIT", 320, FieldType.INT16).setScale(0.01);
	public static final Field TPS2SPLIT = Field.create("TPS2SPLIT", 322, FieldType.INT16).setScale(0.01);
	public static final Field TPS12SPLIT = Field.create("TPS12SPLIT", 324, FieldType.INT16).setScale(0.01);
	public static final Field ACCPEDALSPLIT = Field.create("ACCPEDALSPLIT", 326, FieldType.INT16).setScale(0.01);
	public static final Field SPARKCUTREASON = Field.create("SPARKCUTREASON", 328, FieldType.INT8).setScale(1.0);
	public static final Field FUELCUTREASON = Field.create("FUELCUTREASON", 329, FieldType.INT8).setScale(1.0);
	public static final Field MAFESTIMATE = Field.create("MAFESTIMATE", 330, FieldType.INT16).setScale(0.1);
	public static final Field INSTANTRPM = Field.create("INSTANTRPM", 332, FieldType.INT16).setScale(1.0);
	public static final Field SYSTEMEVENTREUSE = Field.create("SYSTEMEVENTREUSE", 334, FieldType.INT16).setScale(1.0);
	public static final Field RAWMAP = Field.create("RAWMAP", 336, FieldType.INT16).setScale(0.001);
	public static final Field RAWAFR = Field.create("RAWAFR", 338, FieldType.INT16).setScale(0.001);
	public static final Field TPSACCELFROM = Field.create("TPSACCELFROM", 340, FieldType.INT8).setScale(1.0);
	public static final Field TPSACCELTO = Field.create("TPSACCELTO", 341, FieldType.INT8).setScale(1.0);
	public static final Field ALIGNMENTFILL_AT_342 = Field.create("ALIGNMENTFILL_AT_342", 342, FieldType.INT8).setScale(1.0);
	public static final Field CALIBRATIONVALUE2 = Field.create("CALIBRATIONVALUE2", 344, FieldType.FLOAT);
	public static final Field ISMAINRELAYON = Field.create("ISMAINRELAYON", 348, FieldType.BIT, 0);
	public static final Field ISUSBCONNECTED = Field.create("ISUSBCONNECTED", 348, FieldType.BIT, 1);
	public static final Field UNUSEDBIT_174_2 = Field.create("UNUSEDBIT_174_2", 348, FieldType.BIT, 2);
	public static final Field UNUSEDBIT_174_3 = Field.create("UNUSEDBIT_174_3", 348, FieldType.BIT, 3);
	public static final Field UNUSEDBIT_174_4 = Field.create("UNUSEDBIT_174_4", 348, FieldType.BIT, 4);
	public static final Field UNUSEDBIT_174_5 = Field.create("UNUSEDBIT_174_5", 348, FieldType.BIT, 5);
	public static final Field UNUSEDBIT_174_6 = Field.create("UNUSEDBIT_174_6", 348, FieldType.BIT, 6);
	public static final Field UNUSEDBIT_174_7 = Field.create("UNUSEDBIT_174_7", 348, FieldType.BIT, 7);
	public static final Field UNUSEDBIT_174_8 = Field.create("UNUSEDBIT_174_8", 348, FieldType.BIT, 8);
	public static final Field UNUSEDBIT_174_9 = Field.create("UNUSEDBIT_174_9", 348, FieldType.BIT, 9);
	public static final Field UNUSEDBIT_174_10 = Field.create("UNUSEDBIT_174_10", 348, FieldType.BIT, 10);
	public static final Field UNUSEDBIT_174_11 = Field.create("UNUSEDBIT_174_11", 348, FieldType.BIT, 11);
	public static final Field UNUSEDBIT_174_12 = Field.create("UNUSEDBIT_174_12", 348, FieldType.BIT, 12);
	public static final Field UNUSEDBIT_174_13 = Field.create("UNUSEDBIT_174_13", 348, FieldType.BIT, 13);
	public static final Field UNUSEDBIT_174_14 = Field.create("UNUSEDBIT_174_14", 348, FieldType.BIT, 14);
	public static final Field UNUSEDBIT_174_15 = Field.create("UNUSEDBIT_174_15", 348, FieldType.BIT, 15);
	public static final Field UNUSEDBIT_174_16 = Field.create("UNUSEDBIT_174_16", 348, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_174_17 = Field.create("UNUSEDBIT_174_17", 348, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_174_18 = Field.create("UNUSEDBIT_174_18", 348, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_174_19 = Field.create("UNUSEDBIT_174_19", 348, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_174_20 = Field.create("UNUSEDBIT_174_20", 348, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_174_21 = Field.create("UNUSEDBIT_174_21", 348, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_174_22 = Field.create("UNUSEDBIT_174_22", 348, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_174_23 = Field.create("UNUSEDBIT_174_23", 348, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_174_24 = Field.create("UNUSEDBIT_174_24", 348, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_174_25 = Field.create("UNUSEDBIT_174_25", 348, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_174_26 = Field.create("UNUSEDBIT_174_26", 348, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_174_27 = Field.create("UNUSEDBIT_174_27", 348, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_174_28 = Field.create("UNUSEDBIT_174_28", 348, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_174_29 = Field.create("UNUSEDBIT_174_29", 348, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_174_30 = Field.create("UNUSEDBIT_174_30", 348, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_174_31 = Field.create("UNUSEDBIT_174_31", 348, FieldType.BIT, 31);
	public static final Field LUAINVOCATIONCOUNTER = Field.create("LUAINVOCATIONCOUNTER", 352, FieldType.INT).setScale(1.0);
	public static final Field LUALASTCYCLEDURATION = Field.create("LUALASTCYCLEDURATION", 356, FieldType.INT).setScale(1.0);
	public static final Field TESTBENCHITER = Field.create("TESTBENCHITER", 360, FieldType.INT8).setScale(1.0);
	public static final Field TCU_CURRENTRANGE = Field.create("TCU_CURRENTRANGE", 361, FieldType.INT8).setScale(1.0);
	public static final Field TCRATIO = Field.create("TCRATIO", 362, FieldType.INT16).setScale(0.01);
	public static final Field LASTSHIFTTIME = Field.create("LASTSHIFTTIME", 364, FieldType.FLOAT);
	public static final Field VSSEDGECOUNTER = Field.create("VSSEDGECOUNTER", 368, FieldType.INT).setScale(1.0);
	public static final Field ISSEDGECOUNTER = Field.create("ISSEDGECOUNTER", 372, FieldType.INT).setScale(1.0);
	public static final Field AUXLINEAR1 = Field.create("AUXLINEAR1", 376, FieldType.INT16).setScale(0.01);
	public static final Field AUXLINEAR2 = Field.create("AUXLINEAR2", 378, FieldType.INT16).setScale(0.01);
	public static final Field DFCOACTIVE = Field.create("DFCOACTIVE", 380, FieldType.BIT, 0);
	public static final Field TPSACCELACTIVE = Field.create("TPSACCELACTIVE", 380, FieldType.BIT, 1);
	public static final Field UNUSEDBIT_216_2 = Field.create("UNUSEDBIT_216_2", 380, FieldType.BIT, 2);
	public static final Field UNUSEDBIT_216_3 = Field.create("UNUSEDBIT_216_3", 380, FieldType.BIT, 3);
	public static final Field UNUSEDBIT_216_4 = Field.create("UNUSEDBIT_216_4", 380, FieldType.BIT, 4);
	public static final Field UNUSEDBIT_216_5 = Field.create("UNUSEDBIT_216_5", 380, FieldType.BIT, 5);
	public static final Field UNUSEDBIT_216_6 = Field.create("UNUSEDBIT_216_6", 380, FieldType.BIT, 6);
	public static final Field UNUSEDBIT_216_7 = Field.create("UNUSEDBIT_216_7", 380, FieldType.BIT, 7);
	public static final Field UNUSEDBIT_216_8 = Field.create("UNUSEDBIT_216_8", 380, FieldType.BIT, 8);
	public static final Field UNUSEDBIT_216_9 = Field.create("UNUSEDBIT_216_9", 380, FieldType.BIT, 9);
	public static final Field UNUSEDBIT_216_10 = Field.create("UNUSEDBIT_216_10", 380, FieldType.BIT, 10);
	public static final Field UNUSEDBIT_216_11 = Field.create("UNUSEDBIT_216_11", 380, FieldType.BIT, 11);
	public static final Field UNUSEDBIT_216_12 = Field.create("UNUSEDBIT_216_12", 380, FieldType.BIT, 12);
	public static final Field UNUSEDBIT_216_13 = Field.create("UNUSEDBIT_216_13", 380, FieldType.BIT, 13);
	public static final Field UNUSEDBIT_216_14 = Field.create("UNUSEDBIT_216_14", 380, FieldType.BIT, 14);
	public static final Field UNUSEDBIT_216_15 = Field.create("UNUSEDBIT_216_15", 380, FieldType.BIT, 15);
	public static final Field UNUSEDBIT_216_16 = Field.create("UNUSEDBIT_216_16", 380, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_216_17 = Field.create("UNUSEDBIT_216_17", 380, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_216_18 = Field.create("UNUSEDBIT_216_18", 380, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_216_19 = Field.create("UNUSEDBIT_216_19", 380, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_216_20 = Field.create("UNUSEDBIT_216_20", 380, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_216_21 = Field.create("UNUSEDBIT_216_21", 380, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_216_22 = Field.create("UNUSEDBIT_216_22", 380, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_216_23 = Field.create("UNUSEDBIT_216_23", 380, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_216_24 = Field.create("UNUSEDBIT_216_24", 380, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_216_25 = Field.create("UNUSEDBIT_216_25", 380, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_216_26 = Field.create("UNUSEDBIT_216_26", 380, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_216_27 = Field.create("UNUSEDBIT_216_27", 380, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_216_28 = Field.create("UNUSEDBIT_216_28", 380, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_216_29 = Field.create("UNUSEDBIT_216_29", 380, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_216_30 = Field.create("UNUSEDBIT_216_30", 380, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_216_31 = Field.create("UNUSEDBIT_216_31", 380, FieldType.BIT, 31);
	public static final Field BOOSTCONTROLLEROUTPUT = Field.create("BOOSTCONTROLLEROUTPUT", 384, FieldType.INT8).setScale(0.5);
	public static final Field BOOSTCONTROLLEROPENLOOPPART = Field.create("BOOSTCONTROLLEROPENLOOPPART", 385, FieldType.INT8).setScale(0.5);
	public static final Field FALLBACKMAP = Field.create("FALLBACKMAP", 386, FieldType.INT16).setScale(0.1);
	public static final Field DETECTEDGEAR = Field.create("DETECTEDGEAR", 388, FieldType.INT8).setScale(1.0);
	public static final Field ALIGNMENTFILL_AT_389 = Field.create("ALIGNMENTFILL_AT_389", 389, FieldType.INT8).setScale(1.0);
	public static final Field INSTANTMAPVALUE = Field.create("INSTANTMAPVALUE", 390, FieldType.INT16).setScale(0.03333333333333333);
	public static final Field MOSTRECENTTIMEBETWEENSPARKEVENTS = Field.create("MOSTRECENTTIMEBETWEENSPARKEVENTS", 392, FieldType.INT16).setScale(1.0);
	public static final Field MOSTRECENTTIMEBETWEENIGNITIONEVENTS = Field.create("MOSTRECENTTIMEBETWEENIGNITIONEVENTS", 394, FieldType.INT16).setScale(1.0);
	public static final Field MAXLOCKEDDURATION = Field.create("MAXLOCKEDDURATION", 396, FieldType.INT16).setScale(1.0);
	public static final Field MAXTRIGGERREENTRANT = Field.create("MAXTRIGGERREENTRANT", 398, FieldType.INT16).setScale(1.0);
	public static final Field CANWRITEOK = Field.create("CANWRITEOK", 400, FieldType.INT16).setScale(1.0);
	public static final Field CANWRITENOTOK = Field.create("CANWRITENOTOK", 402, FieldType.INT16).setScale(1.0);
	public static final Field TRIGGERPRIMARYFALL = Field.create("TRIGGERPRIMARYFALL", 404, FieldType.INT).setScale(1.0);
	public static final Field TRIGGERPRIMARYRISE = Field.create("TRIGGERPRIMARYRISE", 408, FieldType.INT).setScale(1.0);
	public static final Field TRIGGERSECONDARYFALL = Field.create("TRIGGERSECONDARYFALL", 412, FieldType.INT).setScale(1.0);
	public static final Field TRIGGERSECONDARYRISE = Field.create("TRIGGERSECONDARYRISE", 416, FieldType.INT).setScale(1.0);
	public static final Field TRIGGERVVTFALL = Field.create("TRIGGERVVTFALL", 420, FieldType.INT).setScale(1.0);
	public static final Field TRIGGERVVTRISE = Field.create("TRIGGERVVTRISE", 424, FieldType.INT).setScale(1.0);
	public static final Field STARTSTOPSTATETOGGLECOUNTER = Field.create("STARTSTOPSTATETOGGLECOUNTER", 428, FieldType.INT8).setScale(1.0);
	public static final Field STARTERSTATE = Field.create("STARTERSTATE", 429, FieldType.INT8).setScale(1.0);
	public static final Field STARTERRELAYDISABLE = Field.create("STARTERRELAYDISABLE", 430, FieldType.INT8).setScale(1.0);
	public static final Field MULTISPARKCOUNTER = Field.create("MULTISPARKCOUNTER", 431, FieldType.INT8).setScale(1.0);
	public static final Field INJECTORFLOWPRESSURERATIO = Field.create("INJECTORFLOWPRESSURERATIO", 432, FieldType.FLOAT);
	public static final Field INJECTORFLOWPRESSUREDELTA = Field.create("INJECTORFLOWPRESSUREDELTA", 436, FieldType.FLOAT);
	public static final Field ALTERNATORSTATUS_PTERM = Field.create("ALTERNATORSTATUS_PTERM", 440, FieldType.FLOAT);
	public static final Field ALTERNATORSTATUS_ITERM = Field.create("ALTERNATORSTATUS_ITERM", 444, FieldType.INT16).setScale(0.01);
	public static final Field ALTERNATORSTATUS_DTERM = Field.create("ALTERNATORSTATUS_DTERM", 446, FieldType.INT16).setScale(0.01);
	public static final Field ALTERNATORSTATUS_OUTPUT = Field.create("ALTERNATORSTATUS_OUTPUT", 448, FieldType.INT16).setScale(0.01);
	public static final Field ALTERNATORSTATUS_ERROR = Field.create("ALTERNATORSTATUS_ERROR", 450, FieldType.INT16).setScale(0.01);
	public static final Field ALTERNATORSTATUS_RESETCOUNTER = Field.create("ALTERNATORSTATUS_RESETCOUNTER", 452, FieldType.INT).setScale(1.0);
	public static final Field IDLESTATUS_PTERM = Field.create("IDLESTATUS_PTERM", 456, FieldType.FLOAT);
	public static final Field IDLESTATUS_ITERM = Field.create("IDLESTATUS_ITERM", 460, FieldType.INT16).setScale(0.01);
	public static final Field IDLESTATUS_DTERM = Field.create("IDLESTATUS_DTERM", 462, FieldType.INT16).setScale(0.01);
	public static final Field IDLESTATUS_OUTPUT = Field.create("IDLESTATUS_OUTPUT", 464, FieldType.INT16).setScale(0.01);
	public static final Field IDLESTATUS_ERROR = Field.create("IDLESTATUS_ERROR", 466, FieldType.INT16).setScale(0.01);
	public static final Field IDLESTATUS_RESETCOUNTER = Field.create("IDLESTATUS_RESETCOUNTER", 468, FieldType.INT).setScale(1.0);
	public static final Field ETBSTATUS_PTERM = Field.create("ETBSTATUS_PTERM", 472, FieldType.FLOAT);
	public static final Field ETBSTATUS_ITERM = Field.create("ETBSTATUS_ITERM", 476, FieldType.INT16).setScale(0.01);
	public static final Field ETBSTATUS_DTERM = Field.create("ETBSTATUS_DTERM", 478, FieldType.INT16).setScale(0.01);
	public static final Field ETBSTATUS_OUTPUT = Field.create("ETBSTATUS_OUTPUT", 480, FieldType.INT16).setScale(0.01);
	public static final Field ETBSTATUS_ERROR = Field.create("ETBSTATUS_ERROR", 482, FieldType.INT16).setScale(0.01);
	public static final Field ETBSTATUS_RESETCOUNTER = Field.create("ETBSTATUS_RESETCOUNTER", 484, FieldType.INT).setScale(1.0);
	public static final Field BOOSTSTATUS_PTERM = Field.create("BOOSTSTATUS_PTERM", 488, FieldType.FLOAT);
	public static final Field BOOSTSTATUS_ITERM = Field.create("BOOSTSTATUS_ITERM", 492, FieldType.INT16).setScale(0.01);
	public static final Field BOOSTSTATUS_DTERM = Field.create("BOOSTSTATUS_DTERM", 494, FieldType.INT16).setScale(0.01);
	public static final Field BOOSTSTATUS_OUTPUT = Field.create("BOOSTSTATUS_OUTPUT", 496, FieldType.INT16).setScale(0.01);
	public static final Field BOOSTSTATUS_ERROR = Field.create("BOOSTSTATUS_ERROR", 498, FieldType.INT16).setScale(0.01);
	public static final Field BOOSTSTATUS_RESETCOUNTER = Field.create("BOOSTSTATUS_RESETCOUNTER", 500, FieldType.INT).setScale(1.0);
	public static final Field VVTSTATUS1_PTERM = Field.create("VVTSTATUS1_PTERM", 504, FieldType.FLOAT);
	public static final Field VVTSTATUS1_ITERM = Field.create("VVTSTATUS1_ITERM", 508, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS1_DTERM = Field.create("VVTSTATUS1_DTERM", 510, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS1_OUTPUT = Field.create("VVTSTATUS1_OUTPUT", 512, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS1_ERROR = Field.create("VVTSTATUS1_ERROR", 514, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS1_RESETCOUNTER = Field.create("VVTSTATUS1_RESETCOUNTER", 516, FieldType.INT).setScale(1.0);
	public static final Field VVTSTATUS2_PTERM = Field.create("VVTSTATUS2_PTERM", 520, FieldType.FLOAT);
	public static final Field VVTSTATUS2_ITERM = Field.create("VVTSTATUS2_ITERM", 524, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS2_DTERM = Field.create("VVTSTATUS2_DTERM", 526, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS2_OUTPUT = Field.create("VVTSTATUS2_OUTPUT", 528, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS2_ERROR = Field.create("VVTSTATUS2_ERROR", 530, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS2_RESETCOUNTER = Field.create("VVTSTATUS2_RESETCOUNTER", 532, FieldType.INT).setScale(1.0);
	public static final Field VVTSTATUS3_PTERM = Field.create("VVTSTATUS3_PTERM", 536, FieldType.FLOAT);
	public static final Field VVTSTATUS3_ITERM = Field.create("VVTSTATUS3_ITERM", 540, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS3_DTERM = Field.create("VVTSTATUS3_DTERM", 542, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS3_OUTPUT = Field.create("VVTSTATUS3_OUTPUT", 544, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS3_ERROR = Field.create("VVTSTATUS3_ERROR", 546, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS3_RESETCOUNTER = Field.create("VVTSTATUS3_RESETCOUNTER", 548, FieldType.INT).setScale(1.0);
	public static final Field VVTSTATUS4_PTERM = Field.create("VVTSTATUS4_PTERM", 552, FieldType.FLOAT);
	public static final Field VVTSTATUS4_ITERM = Field.create("VVTSTATUS4_ITERM", 556, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS4_DTERM = Field.create("VVTSTATUS4_DTERM", 558, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS4_OUTPUT = Field.create("VVTSTATUS4_OUTPUT", 560, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS4_ERROR = Field.create("VVTSTATUS4_ERROR", 562, FieldType.INT16).setScale(0.01);
	public static final Field VVTSTATUS4_RESETCOUNTER = Field.create("VVTSTATUS4_RESETCOUNTER", 564, FieldType.INT).setScale(1.0);
	public static final Field AUXSPEED1 = Field.create("AUXSPEED1", 568, FieldType.INT16).setScale(1.0);
	public static final Field AUXSPEED2 = Field.create("AUXSPEED2", 570, FieldType.INT16).setScale(1.0);
	public static final Field ISSVALUE = Field.create("ISSVALUE", 572, FieldType.INT16).setScale(1.0);
	public static final Field RAWANALOGINPUT1 = Field.create("RAWANALOGINPUT1", 574, FieldType.INT16).setScale(0.001);
	public static final Field RAWANALOGINPUT2 = Field.create("RAWANALOGINPUT2", 576, FieldType.INT16).setScale(0.001);
	public static final Field RAWANALOGINPUT3 = Field.create("RAWANALOGINPUT3", 578, FieldType.INT16).setScale(0.001);
	public static final Field RAWANALOGINPUT4 = Field.create("RAWANALOGINPUT4", 580, FieldType.INT16).setScale(0.001);
	public static final Field RAWANALOGINPUT5 = Field.create("RAWANALOGINPUT5", 582, FieldType.INT16).setScale(0.001);
	public static final Field RAWANALOGINPUT6 = Field.create("RAWANALOGINPUT6", 584, FieldType.INT16).setScale(0.001);
	public static final Field RAWANALOGINPUT7 = Field.create("RAWANALOGINPUT7", 586, FieldType.INT16).setScale(0.001);
	public static final Field RAWANALOGINPUT8 = Field.create("RAWANALOGINPUT8", 588, FieldType.INT16).setScale(0.001);
	public static final Field GPPWMOUTPUT1 = Field.create("GPPWMOUTPUT1", 590, FieldType.INT8).setScale(0.5);
	public static final Field GPPWMOUTPUT2 = Field.create("GPPWMOUTPUT2", 591, FieldType.INT8).setScale(0.5);
	public static final Field GPPWMOUTPUT3 = Field.create("GPPWMOUTPUT3", 592, FieldType.INT8).setScale(0.5);
	public static final Field GPPWMOUTPUT4 = Field.create("GPPWMOUTPUT4", 593, FieldType.INT8).setScale(0.5);
	public static final Field RAWBATTERY = Field.create("RAWBATTERY", 594, FieldType.INT16).setScale(0.001);
	public static final Field UNUSEDATTHEEND1 = Field.create("UNUSEDATTHEEND1", 596, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND2 = Field.create("UNUSEDATTHEEND2", 597, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND3 = Field.create("UNUSEDATTHEEND3", 598, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND4 = Field.create("UNUSEDATTHEEND4", 599, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND5 = Field.create("UNUSEDATTHEEND5", 600, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND6 = Field.create("UNUSEDATTHEEND6", 601, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND7 = Field.create("UNUSEDATTHEEND7", 602, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND8 = Field.create("UNUSEDATTHEEND8", 603, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND9 = Field.create("UNUSEDATTHEEND9", 604, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND10 = Field.create("UNUSEDATTHEEND10", 605, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND11 = Field.create("UNUSEDATTHEEND11", 606, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND12 = Field.create("UNUSEDATTHEEND12", 607, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND13 = Field.create("UNUSEDATTHEEND13", 608, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND14 = Field.create("UNUSEDATTHEEND14", 609, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND15 = Field.create("UNUSEDATTHEEND15", 610, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND16 = Field.create("UNUSEDATTHEEND16", 611, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND17 = Field.create("UNUSEDATTHEEND17", 612, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND18 = Field.create("UNUSEDATTHEEND18", 613, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND19 = Field.create("UNUSEDATTHEEND19", 614, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND20 = Field.create("UNUSEDATTHEEND20", 615, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND21 = Field.create("UNUSEDATTHEEND21", 616, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND22 = Field.create("UNUSEDATTHEEND22", 617, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND23 = Field.create("UNUSEDATTHEEND23", 618, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND24 = Field.create("UNUSEDATTHEEND24", 619, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND25 = Field.create("UNUSEDATTHEEND25", 620, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND26 = Field.create("UNUSEDATTHEEND26", 621, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND27 = Field.create("UNUSEDATTHEEND27", 622, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND28 = Field.create("UNUSEDATTHEEND28", 623, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND29 = Field.create("UNUSEDATTHEEND29", 624, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND30 = Field.create("UNUSEDATTHEEND30", 625, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND31 = Field.create("UNUSEDATTHEEND31", 626, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND32 = Field.create("UNUSEDATTHEEND32", 627, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND33 = Field.create("UNUSEDATTHEEND33", 628, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND34 = Field.create("UNUSEDATTHEEND34", 629, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND35 = Field.create("UNUSEDATTHEEND35", 630, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND36 = Field.create("UNUSEDATTHEEND36", 631, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND37 = Field.create("UNUSEDATTHEEND37", 632, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND38 = Field.create("UNUSEDATTHEEND38", 633, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND39 = Field.create("UNUSEDATTHEEND39", 634, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND40 = Field.create("UNUSEDATTHEEND40", 635, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND41 = Field.create("UNUSEDATTHEEND41", 636, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND42 = Field.create("UNUSEDATTHEEND42", 637, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND43 = Field.create("UNUSEDATTHEEND43", 638, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND44 = Field.create("UNUSEDATTHEEND44", 639, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND45 = Field.create("UNUSEDATTHEEND45", 640, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND46 = Field.create("UNUSEDATTHEEND46", 641, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND47 = Field.create("UNUSEDATTHEEND47", 642, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND48 = Field.create("UNUSEDATTHEEND48", 643, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND49 = Field.create("UNUSEDATTHEEND49", 644, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND50 = Field.create("UNUSEDATTHEEND50", 645, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND51 = Field.create("UNUSEDATTHEEND51", 646, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND52 = Field.create("UNUSEDATTHEEND52", 647, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND53 = Field.create("UNUSEDATTHEEND53", 648, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND54 = Field.create("UNUSEDATTHEEND54", 649, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND55 = Field.create("UNUSEDATTHEEND55", 650, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND56 = Field.create("UNUSEDATTHEEND56", 651, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND57 = Field.create("UNUSEDATTHEEND57", 652, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND58 = Field.create("UNUSEDATTHEEND58", 653, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND59 = Field.create("UNUSEDATTHEEND59", 654, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND60 = Field.create("UNUSEDATTHEEND60", 655, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND61 = Field.create("UNUSEDATTHEEND61", 656, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND62 = Field.create("UNUSEDATTHEEND62", 657, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND63 = Field.create("UNUSEDATTHEEND63", 658, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND64 = Field.create("UNUSEDATTHEEND64", 659, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND65 = Field.create("UNUSEDATTHEEND65", 660, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND66 = Field.create("UNUSEDATTHEEND66", 661, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND67 = Field.create("UNUSEDATTHEEND67", 662, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND68 = Field.create("UNUSEDATTHEEND68", 663, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND69 = Field.create("UNUSEDATTHEEND69", 664, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND70 = Field.create("UNUSEDATTHEEND70", 665, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND71 = Field.create("UNUSEDATTHEEND71", 666, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND72 = Field.create("UNUSEDATTHEEND72", 667, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND73 = Field.create("UNUSEDATTHEEND73", 668, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND74 = Field.create("UNUSEDATTHEEND74", 669, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND75 = Field.create("UNUSEDATTHEEND75", 670, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND76 = Field.create("UNUSEDATTHEEND76", 671, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND77 = Field.create("UNUSEDATTHEEND77", 672, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND78 = Field.create("UNUSEDATTHEEND78", 673, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND79 = Field.create("UNUSEDATTHEEND79", 674, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND80 = Field.create("UNUSEDATTHEEND80", 675, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND81 = Field.create("UNUSEDATTHEEND81", 676, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND82 = Field.create("UNUSEDATTHEEND82", 677, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND83 = Field.create("UNUSEDATTHEEND83", 678, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND84 = Field.create("UNUSEDATTHEEND84", 679, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND85 = Field.create("UNUSEDATTHEEND85", 680, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND86 = Field.create("UNUSEDATTHEEND86", 681, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND87 = Field.create("UNUSEDATTHEEND87", 682, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND88 = Field.create("UNUSEDATTHEEND88", 683, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND89 = Field.create("UNUSEDATTHEEND89", 684, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND90 = Field.create("UNUSEDATTHEEND90", 685, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND91 = Field.create("UNUSEDATTHEEND91", 686, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND92 = Field.create("UNUSEDATTHEEND92", 687, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND93 = Field.create("UNUSEDATTHEEND93", 688, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND94 = Field.create("UNUSEDATTHEEND94", 689, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND95 = Field.create("UNUSEDATTHEEND95", 690, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND96 = Field.create("UNUSEDATTHEEND96", 691, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND97 = Field.create("UNUSEDATTHEEND97", 692, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND98 = Field.create("UNUSEDATTHEEND98", 693, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND99 = Field.create("UNUSEDATTHEEND99", 694, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND100 = Field.create("UNUSEDATTHEEND100", 695, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND101 = Field.create("UNUSEDATTHEEND101", 696, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND102 = Field.create("UNUSEDATTHEEND102", 697, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND103 = Field.create("UNUSEDATTHEEND103", 698, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND104 = Field.create("UNUSEDATTHEEND104", 699, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND105 = Field.create("UNUSEDATTHEEND105", 700, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND106 = Field.create("UNUSEDATTHEEND106", 701, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND107 = Field.create("UNUSEDATTHEEND107", 702, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND108 = Field.create("UNUSEDATTHEEND108", 703, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND109 = Field.create("UNUSEDATTHEEND109", 704, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND110 = Field.create("UNUSEDATTHEEND110", 705, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND111 = Field.create("UNUSEDATTHEEND111", 706, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND112 = Field.create("UNUSEDATTHEEND112", 707, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND113 = Field.create("UNUSEDATTHEEND113", 708, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND114 = Field.create("UNUSEDATTHEEND114", 709, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND115 = Field.create("UNUSEDATTHEEND115", 710, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND116 = Field.create("UNUSEDATTHEEND116", 711, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND117 = Field.create("UNUSEDATTHEEND117", 712, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND118 = Field.create("UNUSEDATTHEEND118", 713, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND119 = Field.create("UNUSEDATTHEEND119", 714, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND120 = Field.create("UNUSEDATTHEEND120", 715, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND121 = Field.create("UNUSEDATTHEEND121", 716, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND122 = Field.create("UNUSEDATTHEEND122", 717, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND123 = Field.create("UNUSEDATTHEEND123", 718, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND124 = Field.create("UNUSEDATTHEEND124", 719, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND125 = Field.create("UNUSEDATTHEEND125", 720, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND126 = Field.create("UNUSEDATTHEEND126", 721, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND127 = Field.create("UNUSEDATTHEEND127", 722, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND128 = Field.create("UNUSEDATTHEEND128", 723, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND129 = Field.create("UNUSEDATTHEEND129", 724, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND130 = Field.create("UNUSEDATTHEEND130", 725, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND131 = Field.create("UNUSEDATTHEEND131", 726, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND132 = Field.create("UNUSEDATTHEEND132", 727, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND133 = Field.create("UNUSEDATTHEEND133", 728, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND134 = Field.create("UNUSEDATTHEEND134", 729, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND135 = Field.create("UNUSEDATTHEEND135", 730, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND136 = Field.create("UNUSEDATTHEEND136", 731, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND137 = Field.create("UNUSEDATTHEEND137", 732, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND138 = Field.create("UNUSEDATTHEEND138", 733, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND139 = Field.create("UNUSEDATTHEEND139", 734, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND140 = Field.create("UNUSEDATTHEEND140", 735, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND141 = Field.create("UNUSEDATTHEEND141", 736, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND142 = Field.create("UNUSEDATTHEEND142", 737, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND143 = Field.create("UNUSEDATTHEEND143", 738, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND144 = Field.create("UNUSEDATTHEEND144", 739, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND145 = Field.create("UNUSEDATTHEEND145", 740, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND146 = Field.create("UNUSEDATTHEEND146", 741, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND147 = Field.create("UNUSEDATTHEEND147", 742, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND148 = Field.create("UNUSEDATTHEEND148", 743, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND149 = Field.create("UNUSEDATTHEEND149", 744, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND150 = Field.create("UNUSEDATTHEEND150", 745, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND151 = Field.create("UNUSEDATTHEEND151", 746, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND152 = Field.create("UNUSEDATTHEEND152", 747, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND153 = Field.create("UNUSEDATTHEEND153", 748, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND154 = Field.create("UNUSEDATTHEEND154", 749, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND155 = Field.create("UNUSEDATTHEEND155", 750, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND156 = Field.create("UNUSEDATTHEEND156", 751, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND157 = Field.create("UNUSEDATTHEEND157", 752, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND158 = Field.create("UNUSEDATTHEEND158", 753, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND159 = Field.create("UNUSEDATTHEEND159", 754, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND160 = Field.create("UNUSEDATTHEEND160", 755, FieldType.INT8).setScale(1.0);
	public static final Field[] VALUES = {
	SD_PRESENT,
	ISIGNITIONENABLEDINDICATOR,
	ISINJECTIONENABLEDINDICATOR,
	SD_LOGGING_INTERNAL,
	UNUSEDB4,
	ISFUELPUMPON,
	ISFANON,
	ISO2HEATERON,
	CHECKENGINE,
	NEEDBURN,
	SD_MSD,
	ISFAN2ON,
	ALTERNATORONOFF,
	TOOTHLOGREADY,
	UNUSED3123,
	ISTPSERROR,
	ISCLTERROR,
	ISMAPERROR,
	ISIATERROR,
	UNUSED131313,
	ISTRIGGERERROR,
	HASCRITICALERROR,
	ISWARNNOW,
	ISPEDALERROR,
	ISKNOCKCHIPOK,
	LAUNCHTRIGGERED,
	ISTPS2ERROR,
	ISIDLECLOSEDLOOP,
	ISIDLECOASTING,
	UNUSEDBIT_29_29,
	UNUSEDBIT_29_30,
	UNUSEDBIT_29_31,
	RPMVALUE,
	RPMACCELERATION,
	SPEEDTORPMRATIO,
	VEHICLESPEEDKPH,
	INTERNALMCUTEMPERATURE,
	COOLANT,
	INTAKE,
	AUXTEMP1,
	AUXTEMP2,
	TPSVALUE,
	THROTTLEPEDALPOSITION,
	TPSADC,
	RAWMAF,
	MAFMEASURED,
	MAPVALUE,
	BAROPRESSURE,
	LAMBDAVALUE,
	KNOCKRETARD,
	IDLECURRENTPOSITION,
	VBATT,
	OILPRESSURE,
	VVTPOSITIONB1I,
	CHARGEAIRMASS,
	CRANKINGFUELMS,
	CURRENTTARGETAFR,
	BASEFUEL,
	FUELRUNNING,
	ACTUALLASTINJECTION,
	INJECTORDUTYCYCLE,
	VEVALUE,
	INJECTIONOFFSET,
	TCHARGE,
	INJECTORLAGMS,
	IATCORRECTION,
	CLTCORRECTION,
	BAROCORRECTION,
	CURRENTENGINEPHASE,
	WALLFUELAMOUNT,
	WALLFUELCORRECTION,
	REVOLUTIONCOUNTERSINCESTART,
	DELTATPS,
	CANREADCOUNTER,
	TPSACCELFUEL,
	IGNITIONADVANCE,
	SPARKDWELLVALUE,
	COILDUTYCYCLE,
	IDLEAIRVALVEPOSITION,
	ETBTARGET,
	ETB1DUTYCYCLE,
	ETB1ERROR,
	FUELTANKLEVEL,
	TOTALFUELCONSUMPTION,
	FUELFLOWRATE,
	VETABLEYAXIS,
	AFRTABLEYAXIS,
	KNOCKLEVEL,
	SECONDS,
	ENGINEMODE,
	FIRMWAREVERSION,
	TSCONFIGVERSION,
	CALIBRATIONVALUE,
	CALIBRATIONMODE,
	IDLETARGETPOSITION,
	FUELINGLOAD,
	IGNITIONLOAD,
	ENGINEMAKECODENAMECRC16,
	TOTALTRIGGERERRORCOUNTER,
	ORDERINGERRORCOUNTER,
	WARNINGCOUNTER,
	LASTERRORCODE,
	RECENTERRORCODE1,
	RECENTERRORCODE2,
	RECENTERRORCODE3,
	RECENTERRORCODE4,
	RECENTERRORCODE5,
	RECENTERRORCODE6,
	RECENTERRORCODE7,
	RECENTERRORCODE8,
	DEBUGFLOATFIELD1,
	DEBUGFLOATFIELD2,
	DEBUGFLOATFIELD3,
	DEBUGFLOATFIELD4,
	DEBUGFLOATFIELD5,
	DEBUGFLOATFIELD6,
	DEBUGFLOATFIELD7,
	DEBUGINTFIELD1,
	DEBUGINTFIELD2,
	DEBUGINTFIELD3,
	DEBUGINTFIELD4,
	DEBUGINTFIELD5,
	ACCELERATIONX,
	ACCELERATIONY,
	EGT1,
	EGT2,
	EGT3,
	EGT4,
	EGT5,
	EGT6,
	EGT7,
	EGT8,
	TPS2VALUE,
	RAWTPS1PRIMARY,
	RAWPPSPRIMARY,
	RAWCLT,
	RAWIAT,
	RAWOILPRESSURE,
	TUNECRC16,
	FUELCLOSEDLOOPBINIDX,
	TCUCURRENTGEAR,
	RAWPPSSECONDARY,
	KNOCK1,
	KNOCK2,
	KNOCK3,
	KNOCK4,
	KNOCK5,
	KNOCK6,
	KNOCK7,
	KNOCK8,
	KNOCK9,
	KNOCK10,
	KNOCK11,
	KNOCK12,
	TCUDESIREDGEAR,
	FLEXPERCENT,
	RAWIDLEPOSITIONSENSOR,
	RAWWASTEGATEPOSITION,
	WASTEGATEPOSITIONSENSOR,
	IDLEPOSITIONSENSOR,
	RAWLOWFUELPRESSURE,
	RAWHIGHFUELPRESSURE,
	LOWFUELPRESSURE,
	HIGHFUELPRESSURE,
	TARGETLAMBDA,
	AFRVALUE,
	VSSACCELERATION,
	LAMBDAVALUE2,
	AFRVALUE2,
	VVTPOSITIONB1E,
	VVTPOSITIONB2I,
	VVTPOSITIONB2E,
	FUELPIDCORRECTION1,
	FUELPIDCORRECTION2,
	RAWTPS1SECONDARY,
	RAWTPS2PRIMARY,
	RAWTPS2SECONDARY,
	KNOCKCOUNT,
	ACCELERATIONZ,
	ACCELERATIONROLL,
	ACCELERATIONYAW,
	VVTTARGETS1,
	VVTTARGETS2,
	VVTTARGETS3,
	VVTTARGETS4,
	TURBOSPEED,
	TPS1SPLIT,
	TPS2SPLIT,
	TPS12SPLIT,
	ACCPEDALSPLIT,
	SPARKCUTREASON,
	FUELCUTREASON,
	MAFESTIMATE,
	INSTANTRPM,
	SYSTEMEVENTREUSE,
	RAWMAP,
	RAWAFR,
	TPSACCELFROM,
	TPSACCELTO,
	ALIGNMENTFILL_AT_342,
	CALIBRATIONVALUE2,
	ISMAINRELAYON,
	ISUSBCONNECTED,
	UNUSEDBIT_174_2,
	UNUSEDBIT_174_3,
	UNUSEDBIT_174_4,
	UNUSEDBIT_174_5,
	UNUSEDBIT_174_6,
	UNUSEDBIT_174_7,
	UNUSEDBIT_174_8,
	UNUSEDBIT_174_9,
	UNUSEDBIT_174_10,
	UNUSEDBIT_174_11,
	UNUSEDBIT_174_12,
	UNUSEDBIT_174_13,
	UNUSEDBIT_174_14,
	UNUSEDBIT_174_15,
	UNUSEDBIT_174_16,
	UNUSEDBIT_174_17,
	UNUSEDBIT_174_18,
	UNUSEDBIT_174_19,
	UNUSEDBIT_174_20,
	UNUSEDBIT_174_21,
	UNUSEDBIT_174_22,
	UNUSEDBIT_174_23,
	UNUSEDBIT_174_24,
	UNUSEDBIT_174_25,
	UNUSEDBIT_174_26,
	UNUSEDBIT_174_27,
	UNUSEDBIT_174_28,
	UNUSEDBIT_174_29,
	UNUSEDBIT_174_30,
	UNUSEDBIT_174_31,
	LUAINVOCATIONCOUNTER,
	LUALASTCYCLEDURATION,
	TESTBENCHITER,
	TCU_CURRENTRANGE,
	TCRATIO,
	LASTSHIFTTIME,
	VSSEDGECOUNTER,
	ISSEDGECOUNTER,
	AUXLINEAR1,
	AUXLINEAR2,
	DFCOACTIVE,
	TPSACCELACTIVE,
	UNUSEDBIT_216_2,
	UNUSEDBIT_216_3,
	UNUSEDBIT_216_4,
	UNUSEDBIT_216_5,
	UNUSEDBIT_216_6,
	UNUSEDBIT_216_7,
	UNUSEDBIT_216_8,
	UNUSEDBIT_216_9,
	UNUSEDBIT_216_10,
	UNUSEDBIT_216_11,
	UNUSEDBIT_216_12,
	UNUSEDBIT_216_13,
	UNUSEDBIT_216_14,
	UNUSEDBIT_216_15,
	UNUSEDBIT_216_16,
	UNUSEDBIT_216_17,
	UNUSEDBIT_216_18,
	UNUSEDBIT_216_19,
	UNUSEDBIT_216_20,
	UNUSEDBIT_216_21,
	UNUSEDBIT_216_22,
	UNUSEDBIT_216_23,
	UNUSEDBIT_216_24,
	UNUSEDBIT_216_25,
	UNUSEDBIT_216_26,
	UNUSEDBIT_216_27,
	UNUSEDBIT_216_28,
	UNUSEDBIT_216_29,
	UNUSEDBIT_216_30,
	UNUSEDBIT_216_31,
	BOOSTCONTROLLEROUTPUT,
	BOOSTCONTROLLEROPENLOOPPART,
	FALLBACKMAP,
	DETECTEDGEAR,
	ALIGNMENTFILL_AT_389,
	INSTANTMAPVALUE,
	MOSTRECENTTIMEBETWEENSPARKEVENTS,
	MOSTRECENTTIMEBETWEENIGNITIONEVENTS,
	MAXLOCKEDDURATION,
	MAXTRIGGERREENTRANT,
	CANWRITEOK,
	CANWRITENOTOK,
	TRIGGERPRIMARYFALL,
	TRIGGERPRIMARYRISE,
	TRIGGERSECONDARYFALL,
	TRIGGERSECONDARYRISE,
	TRIGGERVVTFALL,
	TRIGGERVVTRISE,
	STARTSTOPSTATETOGGLECOUNTER,
	STARTERSTATE,
	STARTERRELAYDISABLE,
	MULTISPARKCOUNTER,
	INJECTORFLOWPRESSURERATIO,
	INJECTORFLOWPRESSUREDELTA,
	ALTERNATORSTATUS_PTERM,
	ALTERNATORSTATUS_ITERM,
	ALTERNATORSTATUS_DTERM,
	ALTERNATORSTATUS_OUTPUT,
	ALTERNATORSTATUS_ERROR,
	ALTERNATORSTATUS_RESETCOUNTER,
	IDLESTATUS_PTERM,
	IDLESTATUS_ITERM,
	IDLESTATUS_DTERM,
	IDLESTATUS_OUTPUT,
	IDLESTATUS_ERROR,
	IDLESTATUS_RESETCOUNTER,
	ETBSTATUS_PTERM,
	ETBSTATUS_ITERM,
	ETBSTATUS_DTERM,
	ETBSTATUS_OUTPUT,
	ETBSTATUS_ERROR,
	ETBSTATUS_RESETCOUNTER,
	BOOSTSTATUS_PTERM,
	BOOSTSTATUS_ITERM,
	BOOSTSTATUS_DTERM,
	BOOSTSTATUS_OUTPUT,
	BOOSTSTATUS_ERROR,
	BOOSTSTATUS_RESETCOUNTER,
	VVTSTATUS1_PTERM,
	VVTSTATUS1_ITERM,
	VVTSTATUS1_DTERM,
	VVTSTATUS1_OUTPUT,
	VVTSTATUS1_ERROR,
	VVTSTATUS1_RESETCOUNTER,
	VVTSTATUS2_PTERM,
	VVTSTATUS2_ITERM,
	VVTSTATUS2_DTERM,
	VVTSTATUS2_OUTPUT,
	VVTSTATUS2_ERROR,
	VVTSTATUS2_RESETCOUNTER,
	VVTSTATUS3_PTERM,
	VVTSTATUS3_ITERM,
	VVTSTATUS3_DTERM,
	VVTSTATUS3_OUTPUT,
	VVTSTATUS3_ERROR,
	VVTSTATUS3_RESETCOUNTER,
	VVTSTATUS4_PTERM,
	VVTSTATUS4_ITERM,
	VVTSTATUS4_DTERM,
	VVTSTATUS4_OUTPUT,
	VVTSTATUS4_ERROR,
	VVTSTATUS4_RESETCOUNTER,
	AUXSPEED1,
	AUXSPEED2,
	ISSVALUE,
	RAWANALOGINPUT1,
	RAWANALOGINPUT2,
	RAWANALOGINPUT3,
	RAWANALOGINPUT4,
	RAWANALOGINPUT5,
	RAWANALOGINPUT6,
	RAWANALOGINPUT7,
	RAWANALOGINPUT8,
	GPPWMOUTPUT1,
	GPPWMOUTPUT2,
	GPPWMOUTPUT3,
	GPPWMOUTPUT4,
	RAWBATTERY,
	UNUSEDATTHEEND1,
	UNUSEDATTHEEND2,
	UNUSEDATTHEEND3,
	UNUSEDATTHEEND4,
	UNUSEDATTHEEND5,
	UNUSEDATTHEEND6,
	UNUSEDATTHEEND7,
	UNUSEDATTHEEND8,
	UNUSEDATTHEEND9,
	UNUSEDATTHEEND10,
	UNUSEDATTHEEND11,
	UNUSEDATTHEEND12,
	UNUSEDATTHEEND13,
	UNUSEDATTHEEND14,
	UNUSEDATTHEEND15,
	UNUSEDATTHEEND16,
	UNUSEDATTHEEND17,
	UNUSEDATTHEEND18,
	UNUSEDATTHEEND19,
	UNUSEDATTHEEND20,
	UNUSEDATTHEEND21,
	UNUSEDATTHEEND22,
	UNUSEDATTHEEND23,
	UNUSEDATTHEEND24,
	UNUSEDATTHEEND25,
	UNUSEDATTHEEND26,
	UNUSEDATTHEEND27,
	UNUSEDATTHEEND28,
	UNUSEDATTHEEND29,
	UNUSEDATTHEEND30,
	UNUSEDATTHEEND31,
	UNUSEDATTHEEND32,
	UNUSEDATTHEEND33,
	UNUSEDATTHEEND34,
	UNUSEDATTHEEND35,
	UNUSEDATTHEEND36,
	UNUSEDATTHEEND37,
	UNUSEDATTHEEND38,
	UNUSEDATTHEEND39,
	UNUSEDATTHEEND40,
	UNUSEDATTHEEND41,
	UNUSEDATTHEEND42,
	UNUSEDATTHEEND43,
	UNUSEDATTHEEND44,
	UNUSEDATTHEEND45,
	UNUSEDATTHEEND46,
	UNUSEDATTHEEND47,
	UNUSEDATTHEEND48,
	UNUSEDATTHEEND49,
	UNUSEDATTHEEND50,
	UNUSEDATTHEEND51,
	UNUSEDATTHEEND52,
	UNUSEDATTHEEND53,
	UNUSEDATTHEEND54,
	UNUSEDATTHEEND55,
	UNUSEDATTHEEND56,
	UNUSEDATTHEEND57,
	UNUSEDATTHEEND58,
	UNUSEDATTHEEND59,
	UNUSEDATTHEEND60,
	UNUSEDATTHEEND61,
	UNUSEDATTHEEND62,
	UNUSEDATTHEEND63,
	UNUSEDATTHEEND64,
	UNUSEDATTHEEND65,
	UNUSEDATTHEEND66,
	UNUSEDATTHEEND67,
	UNUSEDATTHEEND68,
	UNUSEDATTHEEND69,
	UNUSEDATTHEEND70,
	UNUSEDATTHEEND71,
	UNUSEDATTHEEND72,
	UNUSEDATTHEEND73,
	UNUSEDATTHEEND74,
	UNUSEDATTHEEND75,
	UNUSEDATTHEEND76,
	UNUSEDATTHEEND77,
	UNUSEDATTHEEND78,
	UNUSEDATTHEEND79,
	UNUSEDATTHEEND80,
	UNUSEDATTHEEND81,
	UNUSEDATTHEEND82,
	UNUSEDATTHEEND83,
	UNUSEDATTHEEND84,
	UNUSEDATTHEEND85,
	UNUSEDATTHEEND86,
	UNUSEDATTHEEND87,
	UNUSEDATTHEEND88,
	UNUSEDATTHEEND89,
	UNUSEDATTHEEND90,
	UNUSEDATTHEEND91,
	UNUSEDATTHEEND92,
	UNUSEDATTHEEND93,
	UNUSEDATTHEEND94,
	UNUSEDATTHEEND95,
	UNUSEDATTHEEND96,
	UNUSEDATTHEEND97,
	UNUSEDATTHEEND98,
	UNUSEDATTHEEND99,
	UNUSEDATTHEEND100,
	UNUSEDATTHEEND101,
	UNUSEDATTHEEND102,
	UNUSEDATTHEEND103,
	UNUSEDATTHEEND104,
	UNUSEDATTHEEND105,
	UNUSEDATTHEEND106,
	UNUSEDATTHEEND107,
	UNUSEDATTHEEND108,
	UNUSEDATTHEEND109,
	UNUSEDATTHEEND110,
	UNUSEDATTHEEND111,
	UNUSEDATTHEEND112,
	UNUSEDATTHEEND113,
	UNUSEDATTHEEND114,
	UNUSEDATTHEEND115,
	UNUSEDATTHEEND116,
	UNUSEDATTHEEND117,
	UNUSEDATTHEEND118,
	UNUSEDATTHEEND119,
	UNUSEDATTHEEND120,
	UNUSEDATTHEEND121,
	UNUSEDATTHEEND122,
	UNUSEDATTHEEND123,
	UNUSEDATTHEEND124,
	UNUSEDATTHEEND125,
	UNUSEDATTHEEND126,
	UNUSEDATTHEEND127,
	UNUSEDATTHEEND128,
	UNUSEDATTHEEND129,
	UNUSEDATTHEEND130,
	UNUSEDATTHEEND131,
	UNUSEDATTHEEND132,
	UNUSEDATTHEEND133,
	UNUSEDATTHEEND134,
	UNUSEDATTHEEND135,
	UNUSEDATTHEEND136,
	UNUSEDATTHEEND137,
	UNUSEDATTHEEND138,
	UNUSEDATTHEEND139,
	UNUSEDATTHEEND140,
	UNUSEDATTHEEND141,
	UNUSEDATTHEEND142,
	UNUSEDATTHEEND143,
	UNUSEDATTHEEND144,
	UNUSEDATTHEEND145,
	UNUSEDATTHEEND146,
	UNUSEDATTHEEND147,
	UNUSEDATTHEEND148,
	UNUSEDATTHEEND149,
	UNUSEDATTHEEND150,
	UNUSEDATTHEEND151,
	UNUSEDATTHEEND152,
	UNUSEDATTHEEND153,
	UNUSEDATTHEEND154,
	UNUSEDATTHEEND155,
	UNUSEDATTHEEND156,
	UNUSEDATTHEEND157,
	UNUSEDATTHEEND158,
	UNUSEDATTHEEND159,
	UNUSEDATTHEEND160,
	};
}
