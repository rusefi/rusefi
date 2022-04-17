package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary\output_channels.txt Sun Apr 17 17:01:39 EDT 2022

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TsOutputs {
	public static final int accelerationRoll_offset = 310;
	public static final int accelerationX_offset = 212;
	public static final int accelerationY_offset = 214;
	public static final int accelerationYaw_offset = 312;
	public static final int accelerationZ_offset = 308;
	public static final int accPedalSplit_offset = 332;
	public static final int acState_offset = 0;
	public static final int acSwitchState_offset = 0;
	public static final int actualLastInjection_offset = 54;
	public static final int ADC_CHANNEL_NONE = 0;
	public static final int afrTableYAxis_offset = 106;
	public static final int AFRValue2_offset = 288;
	public static final int AFRValue_offset = 282;
	public static final int AFTERSTART_DECAY_CURVE_SIZE = 8;
	public static final int AFTERSTART_ENRICH_CURVE_SIZE = 8;
	public static final int AFTERSTART_HOLD_CURVE_SIZE = 8;
	public static final int alternatorOnOff_offset = 0;
	public static final int alternatorStatus_dTerm_offset = 518;
	public static final int alternatorStatus_error_offset = 522;
	public static final int alternatorStatus_iTerm_offset = 516;
	public static final int alternatorStatus_offset = 516;
	public static final int alternatorStatus_output_offset = 520;
	public static final int alternatorStatus_resetCounter_offset = 524;
	public static final int AUX_ANALOG_INPUT_COUNT = 8;
	public static final int AUX_DIGITAL_VALVE_COUNT = 2;
	public static final int auxLinear1_offset = 404;
	public static final int auxLinear2_offset = 406;
	public static final int auxSpeed1_offset = 564;
	public static final int auxSpeed2_offset = 566;
	public static final int auxTemp1_offset = 16;
	public static final int auxTemp2_offset = 18;
	public static final int BANKS_COUNT = 2;
	public static final int BARO_CORR_SIZE = 4;
	public static final int baroCorrection_offset = 68;
	public static final int baroPressure_offset = 32;
	public static final int baseFuel_offset = 50;
	public static final int BLOCKING_FACTOR = 256;
	public static final int BOOST_LOAD_COUNT = 8;
	public static final int BOOST_RPM_COUNT = 8;
	public static final int boostControllerClosedLoopPart_offset = 442;
	public static final int boostControllerOpenLoopPart_offset = 415;
	public static final int boostControllerOutput_offset = 414;
	public static final int boostControlTarget_offset = 336;
	public static final int boostStatus_dTerm_offset = 554;
	public static final int boostStatus_error_offset = 558;
	public static final int boostStatus_iTerm_offset = 552;
	public static final int boostStatus_offset = 552;
	public static final int boostStatus_output_offset = 556;
	public static final int boostStatus_resetCounter_offset = 560;
	public static final int brakePedalState_offset = 0;
	public static final int calibrationMode_offset = 132;
	public static final int calibrationValue2_offset = 372;
	public static final int calibrationValue_offset = 128;
	public static final int CAM_INPUTS_COUNT = 4;
	public static final int CAM_INPUTS_COUNT_padding = 0;
	public static final int CAMS_PER_BANK = 2;
	public static final int CAMS_PER_BANK_padding = 0;
	public static final int CAN_DEFAULT_BASE = 0x200;
	public static final int CAN_ECU_SERIAL_RX_ID = 0x100;
	public static final int CAN_ECU_SERIAL_TX_ID = 0x102;
	public static final String CAN_RX_PREFIX = "CAN_rx";
	public static final int canReadCounter_offset = 80;
	public static final int canWriteNotOk_offset = 462;
	public static final int canWriteOk_offset = 460;
	public static final int chargeAirMass_offset = 44;
	public static final int checkEngine_offset = 0;
	public static final int CLT_CRANKING_CURVE_SIZE = 8;
	public static final int CLT_CURVE_SIZE = 16;
	public static final int CLT_LIMITER_CURVE_SIZE = 4;
	public static final int CLT_TIMING_CURVE_SIZE = 8;
	public static final int cltCorrection_offset = 66;
	public static final int clutchDownState_offset = 0;
	public static final int clutchUpState_offset = 0;
	public static final String CMD_AC_RELAY_BENCH = "acrelaybench";
	public static final String CMD_ALTERNATOR_PIN = "set_alternator_pin";
	public static final String CMD_BOOST_PIN = "set_boost_pin";
	public static final String CMD_BURNCONFIG = "burnconfig";
	public static final String CMD_DATE = "date";
	public static final String CMD_DISABLE = "disable";
	public static final String CMD_ENABLE = "enable";
	public static final String CMD_ENGINE_TYPE = "engine_type";
	public static final String CMD_ENGINESNIFFERRPMTHRESHOLD = "engineSnifferRpmThreshold";
	public static final String CMD_ETB_DUTY = "set_etb_duty";
	public static final String CMD_EXTERNAL_STIMULATION = "ext_stimulation";
	public static final String CMD_FAN2_BENCH = "fan2bench";
	public static final String CMD_FAN_BENCH = "fanbench";
	public static final String CMD_FUEL_BENCH = "fuelbench";
	public static final String CMD_FUNCTIONAL_TEST_MODE = "test_mode";
	public static final String CMD_GET = "get";
	public static final String CMD_HPFP_BENCH = "hpfpbench";
	public static final String CMD_IDLE_PIN = "set_idle_pin";
	public static final String CMD_IGNITION = "ignition";
	public static final String CMD_IGNITION_PIN = "set_ignition_pin";
	public static final String CMD_INJECTION = "injection";
	public static final String CMD_INJECTION_PIN = "set_injection_pin";
	public static final String CMD_LOGIC_PIN = "set_logic_input_pin";
	public static final String CMD_MIL_BENCH = "milbench";
	public static final String CMD_PINS = "pins";
	public static final String CMD_PWM = "pwm";
	public static final String CMD_REBOOT = "reboot";
	public static final String CMD_REBOOT_DFU = "reboot_dfu";
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
	public static final int CMD_TS_FSIO_CATEGORY = 33;
	public static final int CMD_TS_SOLENOID_CATEGORY = 41;
	public static final int CMD_TS_X14 = 20;
	public static final int CMD_TS_X31 = 49;
	public static final String CMD_VSS_PIN = "vss_pin";
	public static final String CMD_WRITECONFIG = "writeconfig";
	public static final int coilDutyCycle_offset = 88;
	public static final int COMPOSITE_DATA_LENGTH = 2500;
	public static final int COMPOSITE_DATA_LENGTH_HALF = 1250;
	public static final int COMPOSITE_PACKET_COUNT = 500;
	public static final int COMPOSITE_PACKET_SIZE = 5;
	public static final String CONSOLE_DATA_PROTOCOL_TAG = " @";
	public static final int coolant_offset = 12;
	public static final int CRANKING_ADVANCE_CURVE_SIZE = 4;
	public static final int CRANKING_CLT_IDLE_CURVE_SIZE = 8;
	public static final int CRANKING_CURVE_SIZE = 8;
	public static final int crankingFuelMs_offset = 46;
	public static final String CRITICAL_PREFIX = "CRITICAL";
	public static final int currentEnginePhase_offset = 70;
	public static final int currentTargetAfr_offset = 48;
	public static final int DC_PER_STEPPER = 2;
	public static final int debugFloatField1_offset = 168;
	public static final int debugFloatField2_offset = 172;
	public static final int debugFloatField3_offset = 176;
	public static final int debugFloatField4_offset = 180;
	public static final int debugFloatField5_offset = 184;
	public static final int debugFloatField6_offset = 188;
	public static final int debugFloatField7_offset = 192;
	public static final int debugIntField1_offset = 196;
	public static final int debugIntField2_offset = 200;
	public static final int debugIntField3_offset = 204;
	public static final int debugIntField4_offset = 208;
	public static final int debugIntField5_offset = 210;
	public static final int deltaTps_offset = 78;
	public static final int detectedGear_offset = 443;
	public static final int dfcoActive_offset = 408;
	public static final int di_nextStart_offset = 360;
	public static final int DIGIPOT_COUNT = 4;
	public static final int DWELL_CURVE_SIZE = 8;
	public static final int egt1_offset = 216;
	public static final int egt2_offset = 218;
	public static final int egt3_offset = 220;
	public static final int egt4_offset = 222;
	public static final int egt5_offset = 224;
	public static final int egt6_offset = 226;
	public static final int egt7_offset = 228;
	public static final int egt8_offset = 230;
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
	public static final int ENGINE_NOISE_CURVE_SIZE = 8;
	public static final int ENGINE_SNIFFER_UNIT_US = 10;
	public static final int engineMakeCodeNameCrc16_offset = 138;
	public static final int engineMode_offset = 116;
	public static final int ENUM_16_BITS = 60000;
	public static final int ENUM_32_BITS = 2000000000;
	public static final int ERROR_BUFFER_SIZE = 120;
	public static final int etb1DutyCycle_offset = 94;
	public static final int etb1Error_offset = 96;
	public static final int ETB_BIAS_CURVE_LENGTH = 8;
	public static final int ETB_COUNT = 2;
	public static final int ETB_HW_MAX_FREQUENCY = 3000;
	public static final int etbCurrentTarget_offset = 512;
	public static final int etbFeedForward_offset = 504;
	public static final int etbIntegralError_offset = 508;
	public static final int etbStatus_dTerm_offset = 542;
	public static final int etbStatus_error_offset = 546;
	public static final int etbStatus_iTerm_offset = 540;
	public static final int etbStatus_offset = 540;
	public static final int etbStatus_output_offset = 544;
	public static final int etbStatus_resetCounter_offset = 548;
	public static final int etbTarget_offset = 92;
	public static final int fallbackMap_offset = 440;
	public static final int firmwareVersion_offset = 120;
	public static final int FLASH_DATA_VERSION = 10009;
	public static final int flexPercent_offset = 263;
	public static final int FUEL_LEVEL_TABLE_COUNT = 8;
	public static final int FUEL_LOAD_COUNT = 16;
	public static final int fuel_requested_percent_offset = 340;
	public static final int fuel_requested_percent_pi_offset = 344;
	public static final int FUEL_RPM_COUNT = 16;
	public static final int fuelClosedLoopBinIdx_offset = 246;
	public static final int fuelCutReason_offset = 339;
	public static final int fuelFlowRate_offset = 102;
	public static final int fuelingLoad_offset = 134;
	public static final int fuelPidCorrection1_offset = 296;
	public static final int fuelPidCorrection2_offset = 298;
	public static final int fuelRunning_offset = 52;
	public static final int fuelTankLevel_offset = 98;
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
	public static final String GAUGE_NAME_KNOCK_COUNTER = "knock: counter";
	public static final String GAUGE_NAME_KNOCK_LEVEL = "knock: current level";
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
	public static final int hasCriticalError_offset = 0;
	public static final int highFuelPressure_offset = 278;
	public static final int HPFP_COMPENSATION_SIZE = 10;
	public static final int HPFP_DEADTIME_SIZE = 8;
	public static final int HPFP_LOBE_PROFILE_SIZE = 16;
	public static final int HPFP_TARGET_SIZE = 10;
	public static final int HW_MAX_ADC_INDEX = 17;
	public static final int IAC_PID_MULT_SIZE = 8;
	public static final int IAT_CURVE_SIZE = 16;
	public static final int iatCorrection_offset = 64;
	public static final int IDLE_ADVANCE_CURVE_SIZE = 8;
	public static final int IDLE_VE_SIZE = 4;
	public static final int idleAirValvePosition_offset = 90;
	public static final int idleCurrentPosition_offset = 37;
	public static final int idlePositionSensor_offset = 270;
	public static final int idleState_offset = 492;
	public static final int idleStatus_dTerm_offset = 530;
	public static final int idleStatus_error_offset = 534;
	public static final int idleStatus_iTerm_offset = 528;
	public static final int idleStatus_offset = 528;
	public static final int idleStatus_output_offset = 532;
	public static final int idleStatus_resetCounter_offset = 536;
	public static final int idleTargetPosition_offset = 133;
	public static final int IGN_LOAD_COUNT = 16;
	public static final int IGN_RPM_COUNT = 16;
	public static final int ignitionAdvance_offset = 84;
	public static final int ignitionLoad_offset = 136;
	public static final String INDICATOR_NAME_AC_SWITCH = "AC switch";
	public static final String INDICATOR_NAME_BRAKE_DOWN = "brake: down";
	public static final String INDICATOR_NAME_CLUTCH_DOWN = "clutch: down";
	public static final String INDICATOR_NAME_CLUTCH_UP = "clutch: up";
	public static final int injectionOffset_offset = 58;
	public static final int injectorDutyCycle_offset = 56;
	public static final int injectorFlowPressureDelta_offset = 500;
	public static final int injectorFlowPressureRatio_offset = 496;
	public static final int injectorLagMs_offset = 62;
	public static final int instantMAPValue_offset = 450;
	public static final int instantRpm_offset = 358;
	public static final int intake_offset = 14;
	public static final int internalMcuTemperature_offset = 11;
	public static final int isCltError_offset = 0;
	public static final int isFan2On_offset = 0;
	public static final int isFanOn_offset = 0;
	public static final int isFuelPumpOn_offset = 0;
	public static final int isIatError_offset = 0;
	public static final int isIdleClosedLoop_offset = 0;
	public static final int isIdleCoasting_offset = 0;
	public static final int isIgnitionEnabledIndicator_offset = 0;
	public static final int isInjectionEnabledIndicator_offset = 0;
	public static final int isKnockChipOk_offset = 0;
	public static final int isMainRelayOn_offset = 376;
	public static final int isMapError_offset = 0;
	public static final int isO2HeaterOn_offset = 0;
	public static final int isPedalError_offset = 0;
	public static final int isTps2Error_offset = 0;
	public static final int isTpsError_offset = 0;
	public static final int isTriggerError_offset = 0;
	public static final int isUsbConnected_offset = 376;
	public static final int isWarnNow_offset = 0;
	public static final int knock10_offset = 259;
	public static final int knock11_offset = 260;
	public static final int knock12_offset = 261;
	public static final int knock1_offset = 250;
	public static final int knock2_offset = 251;
	public static final int knock3_offset = 252;
	public static final int knock4_offset = 253;
	public static final int knock5_offset = 254;
	public static final int knock6_offset = 255;
	public static final int knock7_offset = 256;
	public static final int knock8_offset = 257;
	public static final int knock9_offset = 258;
	public static final int knockCount_offset = 306;
	public static final int knockLevel_offset = 108;
	public static final int knockRetard_offset = 36;
	public static final int lambdaValue2_offset = 286;
	public static final int lambdaValue_offset = 34;
	public static final int lastErrorCode_offset = 150;
	public static final int launchActivatePinState_offset = 408;
	public static final int launchActivateSwitchCondition_offset = 408;
	public static final int launchCombinedConditions_offset = 408;
	public static final int launchIsLaunchCondition_offset = 408;
	public static final int launchRpmCondition_offset = 408;
	public static final int launchSpeedCondition_offset = 408;
	public static final int launchTpsCondition_offset = 408;
	public static final int launchTriggered_offset = 0;
	public static final int LE_COMMAND_LENGTH = 200;
	public static final String LOG_DELIMITER = "`";
	public static final int LOGIC_ANALYZER_CHANNEL_COUNT = 4;
	public static final int lowFuelPressure_offset = 276;
	public static final int LUA_ANALOG_INPUT_COUNT = 8;
	public static final int LUA_PWM_COUNT = 8;
	public static final int LUA_SCRIPT_SIZE = 8000;
	public static final int luaInvocationCounter_offset = 380;
	public static final int luaLastCycleDuration_offset = 384;
	public static final int m_I_sum_percent_offset = 348;
	public static final int m_pressureTarget_kPa_offset = 352;
	public static final int m_requested_pump_offset = 334;
	public static final int MAF_DECODING_COUNT = 256;
	public static final int mafEstimate_offset = 356;
	public static final int mafMeasured_offset = 28;
	public static final String MAIN_HELP_URL = "http://www.rusefi.com/";
	public static final int MAP_ANGLE_SIZE = 8;
	public static final int MAP_WINDOW_SIZE = 8;
	public static final int MAPValue_offset = 30;
	public static final int MAX_CYLINDER_COUNT = 12;
	public static final int maxLockedDuration_offset = 456;
	public static final int maxTriggerReentrant_offset = 458;
	public static final int MLQ_FIELD_HEADER_SIZE = 55;
	public static final int MLQ_HEADER_SIZE = 22;
	public static final String MOCK_PPS_POSITION_COMMAND = "mock_pps_position";
	public static final String MOCK_PPS_VOLTAGE_COMMAND = "mock_pps_voltage";
	public static final int mostRecentTimeBetweenIgnitionEvents_offset = 454;
	public static final int mostRecentTimeBetweenSparkEvents_offset = 452;
	public static final int multiSparkCounter_offset = 491;
	public static final int NARROW_BAND_WIDE_BAND_CONVERSION_SIZE = 8;
	public static final int needBurn_offset = 0;
	public static final int oilPressure_offset = 40;
	public static final int orderingErrorCounter_offset = 144;
	public static final int PACK_ADD_TEMPERATURE = 40;
	public static final int PACK_MULT_AFR = 1000;
	public static final int PACK_MULT_AFR_CFG = 10;
	public static final int PACK_MULT_ANGLE = 50;
	public static final int PACK_MULT_FUEL_FLOW = 200;
	public static final int PACK_MULT_FUEL_MASS = 100;
	public static final int PACK_MULT_HIGH_PRESSURE = 10;
	public static final int PACK_MULT_LAMBDA = 10000;
	public static final int PACK_MULT_LAMBDA_CFG = 147;
	public static final int PACK_MULT_MAP_ESTIMATE = 100;
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
	public static final int rawAfr_offset = 368;
	public static final int rawClt_offset = 238;
	public static final int rawHighFuelPressure_offset = 274;
	public static final int rawIat_offset = 240;
	public static final int rawIdlePositionSensor_offset = 264;
	public static final int rawLowFuelPressure_offset = 272;
	public static final int rawMaf_offset = 26;
	public static final int rawMap_offset = 366;
	public static final int rawOilPressure_offset = 242;
	public static final int rawPpsPrimary_offset = 236;
	public static final int rawPpsSecondary_offset = 248;
	public static final int rawTps1Primary_offset = 234;
	public static final int rawTps1Secondary_offset = 300;
	public static final int rawTps2Primary_offset = 302;
	public static final int rawTps2Secondary_offset = 304;
	public static final int rawWastegatePosition_offset = 266;
	public static final int recentErrorCode1_offset = 152;
	public static final int recentErrorCode2_offset = 154;
	public static final int recentErrorCode3_offset = 156;
	public static final int recentErrorCode4_offset = 158;
	public static final int recentErrorCode5_offset = 160;
	public static final int recentErrorCode6_offset = 162;
	public static final int recentErrorCode7_offset = 164;
	public static final int recentErrorCode8_offset = 166;
	public static final int revolutionCounterSinceStart_offset = 76;
	public static final int RPM_1_BYTE_PACKING_MULT = 50;
	public static final int rpmAcceleration_offset = 6;
	public static final int RPMValue_offset = 4;
	public static final int SCRIPT_CURVE_16 = 16;
	public static final int SCRIPT_CURVE_8 = 8;
	public static final int SCRIPT_CURVE_COUNT = 6;
	public static final int SCRIPT_SETTING_COUNT = 8;
	public static final int SCRIPT_TABLE_8 = 8;
	public static final int SCRIPT_TABLE_COUNT = 4;
	public static final int sd_logging_internal_offset = 0;
	public static final int sd_msd_offset = 0;
	public static final int sd_present_offset = 0;
	public static final int seconds_offset = 112;
	public static final int SERVO_COUNT = 8;
	public static final int sparkCutReason_offset = 338;
	public static final int sparkDwellValue_offset = 86;
	public static final int speedToRpmRatio_offset = 8;
	public static final int starterRelayDisable_offset = 490;
	public static final int starterState_offset = 489;
	public static final int startStopStateToggleCounter_offset = 488;
	public static final int STFT_BANK_COUNT = 2;
	public static final int STFT_CELL_COUNT = 4;
	public static final int systemEventReuse_offset = 364;
	public static final int targetLambda_offset = 280;
	public static final int tCharge_offset = 60;
	public static final int TCU_GEAR_COUNT = 10;
	public static final int TCU_SOLENOID_COUNT = 6;
	public static final int tcuCurrentGear_offset = 247;
	public static final int tcuDesiredGear_offset = 262;
	public static final int TEMPLOG_MAP_AT_CYCLE_COUNT_offset = 412;
	public static final int TEMPLOG_MAP_AT_DIFF_offset = 420;
	public static final int TEMPLOG_MAP_AT_SPECIAL_POINT_offset = 416;
	public static final int TEMPLOG_map_peak_offset = 413;
	public static final int testBenchIter_offset = 388;
	public static final int throttlePedalPosition_offset = 22;
	public static final int timingCltCorrection_offset = 444;
	public static final int timingIatCorrection_offset = 446;
	public static final int timingPidCorrection_offset = 448;
	public static final int TOOTH_DATA_LENGTH = 2000;
	public static final int TOOTH_PACKET_COUNT = 1000;
	public static final int TOOTH_PACKET_SIZE = 2;
	public static final int toothLogReady_offset = 0;
	public static final String TOP_DEAD_CENTER_MESSAGE = "r";
	public static final int TORQUE_CURVE_SIZE = 6;
	public static final int totalFuelConsumption_offset = 100;
	public static final int totalTriggerErrorCounter_offset = 140;
	public static final int tps12Split_offset = 330;
	public static final int tps1Split_offset = 326;
	public static final int tps2Split_offset = 328;
	public static final int TPS2Value_offset = 232;
	public static final int TPS_2_BYTE_PACKING_MULT = 100;
	public static final int TPS_TPS_ACCEL_TABLE = 8;
	public static final int tpsAccelActive_offset = 408;
	public static final int tpsAccelFrom_offset = 370;
	public static final int tpsAccelFuel_offset = 82;
	public static final int tpsAccelTo_offset = 371;
	public static final int tpsADC_offset = 24;
	public static final int TPSValue_offset = 20;
	public static final String TRIGGER_COMMENT = "#";
	public static final String TRIGGER_HARDCODED_OPERATION_MODE = "knownOperationMode";
	public static final String TRIGGER_HAS_SECOND_CHANNEL = "hasSecondChannel";
	public static final int TRIGGER_INPUT_PIN_COUNT = 3;
	public static final String TRIGGER_IS_CRANK_KEY = "crankBased";
	public static final String TRIGGER_IS_SECOND_WHEEL_CAM = "isSecondWheelCam";
	public static final int TRIGGER_SIMULATOR_PIN_COUNT = 3;
	public static final int triggerPrimaryFall_offset = 464;
	public static final int triggerPrimaryRise_offset = 468;
	public static final String TRIGGERS_FILE_NAME = "triggers.txt";
	public static final int triggerSecondaryFall_offset = 472;
	public static final int triggerSecondaryRise_offset = 476;
	public static final int triggerStateIndex_offset = 436;
	public static final int triggerSyncGapRatio_offset = 432;
	public static final int triggerVvtFall_offset = 480;
	public static final int triggerVvtRise_offset = 484;
	public static final int TRIM_SIZE = 4;
	public static final char TS_BURN_COMMAND = 'B';
	public static final char TS_CHUNK_WRITE_COMMAND = 'C';
	public static final char TS_COMMAND_F = 'F';
	public static final int TS_COMPOSITE_DISABLE = 2;
	public static final int TS_COMPOSITE_ENABLE = 1;
	public static final char TS_CRC_CHECK_COMMAND = 'k';
	public static final char TS_EXECUTE = 'E';
	public static final int TS_FILE_VERSION = 20200310;
	public static final char TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY = '8';
	public static final char TS_GET_CONFIG_ERROR = 'e';
	public static final char TS_GET_FIRMWARE_VERSION = 'V';
	public static final char TS_GET_LOGGER_GET_BUFFER = 'L';
	public static final char TS_GET_OUTPUTS_SIZE = '4';
	public static final char TS_GET_STRUCT = '9';
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
	public static final int tsConfigVersion_offset = 124;
	public static final int tuneCrc16_offset = 244;
	public static final int turboSpeed_offset = 318;
	public static final int unusedAtTheEnd10_offset = 577;
	public static final int unusedAtTheEnd11_offset = 578;
	public static final int unusedAtTheEnd12_offset = 579;
	public static final int unusedAtTheEnd13_offset = 580;
	public static final int unusedAtTheEnd14_offset = 581;
	public static final int unusedAtTheEnd15_offset = 582;
	public static final int unusedAtTheEnd16_offset = 583;
	public static final int unusedAtTheEnd17_offset = 584;
	public static final int unusedAtTheEnd18_offset = 585;
	public static final int unusedAtTheEnd19_offset = 586;
	public static final int unusedAtTheEnd1_offset = 568;
	public static final int unusedAtTheEnd20_offset = 587;
	public static final int unusedAtTheEnd21_offset = 588;
	public static final int unusedAtTheEnd22_offset = 589;
	public static final int unusedAtTheEnd23_offset = 590;
	public static final int unusedAtTheEnd24_offset = 591;
	public static final int unusedAtTheEnd25_offset = 592;
	public static final int unusedAtTheEnd26_offset = 593;
	public static final int unusedAtTheEnd27_offset = 594;
	public static final int unusedAtTheEnd28_offset = 595;
	public static final int unusedAtTheEnd29_offset = 596;
	public static final int unusedAtTheEnd2_offset = 569;
	public static final int unusedAtTheEnd30_offset = 597;
	public static final int unusedAtTheEnd31_offset = 598;
	public static final int unusedAtTheEnd32_offset = 599;
	public static final int unusedAtTheEnd33_offset = 600;
	public static final int unusedAtTheEnd34_offset = 601;
	public static final int unusedAtTheEnd35_offset = 602;
	public static final int unusedAtTheEnd36_offset = 603;
	public static final int unusedAtTheEnd37_offset = 604;
	public static final int unusedAtTheEnd38_offset = 605;
	public static final int unusedAtTheEnd39_offset = 606;
	public static final int unusedAtTheEnd3_offset = 570;
	public static final int unusedAtTheEnd40_offset = 607;
	public static final int unusedAtTheEnd41_offset = 608;
	public static final int unusedAtTheEnd42_offset = 609;
	public static final int unusedAtTheEnd43_offset = 610;
	public static final int unusedAtTheEnd44_offset = 611;
	public static final int unusedAtTheEnd45_offset = 612;
	public static final int unusedAtTheEnd46_offset = 613;
	public static final int unusedAtTheEnd47_offset = 614;
	public static final int unusedAtTheEnd48_offset = 615;
	public static final int unusedAtTheEnd49_offset = 616;
	public static final int unusedAtTheEnd4_offset = 571;
	public static final int unusedAtTheEnd50_offset = 617;
	public static final int unusedAtTheEnd51_offset = 618;
	public static final int unusedAtTheEnd52_offset = 619;
	public static final int unusedAtTheEnd53_offset = 620;
	public static final int unusedAtTheEnd54_offset = 621;
	public static final int unusedAtTheEnd55_offset = 622;
	public static final int unusedAtTheEnd56_offset = 623;
	public static final int unusedAtTheEnd57_offset = 624;
	public static final int unusedAtTheEnd58_offset = 625;
	public static final int unusedAtTheEnd59_offset = 626;
	public static final int unusedAtTheEnd5_offset = 572;
	public static final int unusedAtTheEnd60_offset = 627;
	public static final int unusedAtTheEnd61_offset = 628;
	public static final int unusedAtTheEnd62_offset = 629;
	public static final int unusedAtTheEnd63_offset = 630;
	public static final int unusedAtTheEnd64_offset = 631;
	public static final int unusedAtTheEnd65_offset = 632;
	public static final int unusedAtTheEnd66_offset = 633;
	public static final int unusedAtTheEnd67_offset = 634;
	public static final int unusedAtTheEnd68_offset = 635;
	public static final int unusedAtTheEnd69_offset = 636;
	public static final int unusedAtTheEnd6_offset = 573;
	public static final int unusedAtTheEnd70_offset = 637;
	public static final int unusedAtTheEnd71_offset = 638;
	public static final int unusedAtTheEnd72_offset = 639;
	public static final int unusedAtTheEnd7_offset = 574;
	public static final int unusedAtTheEnd8_offset = 575;
	public static final int unusedAtTheEnd9_offset = 576;
	public static final int unusedb4_offset = 0;
	public static final int unusedBit_182_10_offset = 376;
	public static final int unusedBit_182_11_offset = 376;
	public static final int unusedBit_182_12_offset = 376;
	public static final int unusedBit_182_13_offset = 376;
	public static final int unusedBit_182_14_offset = 376;
	public static final int unusedBit_182_15_offset = 376;
	public static final int unusedBit_182_16_offset = 376;
	public static final int unusedBit_182_17_offset = 376;
	public static final int unusedBit_182_18_offset = 376;
	public static final int unusedBit_182_19_offset = 376;
	public static final int unusedBit_182_20_offset = 376;
	public static final int unusedBit_182_21_offset = 376;
	public static final int unusedBit_182_22_offset = 376;
	public static final int unusedBit_182_23_offset = 376;
	public static final int unusedBit_182_24_offset = 376;
	public static final int unusedBit_182_25_offset = 376;
	public static final int unusedBit_182_26_offset = 376;
	public static final int unusedBit_182_27_offset = 376;
	public static final int unusedBit_182_28_offset = 376;
	public static final int unusedBit_182_29_offset = 376;
	public static final int unusedBit_182_2_offset = 376;
	public static final int unusedBit_182_30_offset = 376;
	public static final int unusedBit_182_31_offset = 376;
	public static final int unusedBit_182_3_offset = 376;
	public static final int unusedBit_182_4_offset = 376;
	public static final int unusedBit_182_5_offset = 376;
	public static final int unusedBit_182_6_offset = 376;
	public static final int unusedBit_182_7_offset = 376;
	public static final int unusedBit_182_8_offset = 376;
	public static final int unusedBit_182_9_offset = 376;
	public static final int unusedBit_227_10_offset = 408;
	public static final int unusedBit_227_11_offset = 408;
	public static final int unusedBit_227_12_offset = 408;
	public static final int unusedBit_227_13_offset = 408;
	public static final int unusedBit_227_14_offset = 408;
	public static final int unusedBit_227_15_offset = 408;
	public static final int unusedBit_227_16_offset = 408;
	public static final int unusedBit_227_17_offset = 408;
	public static final int unusedBit_227_18_offset = 408;
	public static final int unusedBit_227_19_offset = 408;
	public static final int unusedBit_227_20_offset = 408;
	public static final int unusedBit_227_21_offset = 408;
	public static final int unusedBit_227_22_offset = 408;
	public static final int unusedBit_227_23_offset = 408;
	public static final int unusedBit_227_24_offset = 408;
	public static final int unusedBit_227_25_offset = 408;
	public static final int unusedBit_227_26_offset = 408;
	public static final int unusedBit_227_27_offset = 408;
	public static final int unusedBit_227_28_offset = 408;
	public static final int unusedBit_227_29_offset = 408;
	public static final int unusedBit_227_30_offset = 408;
	public static final int unusedBit_227_31_offset = 408;
	public static final int unusedBit_227_9_offset = 408;
	public static final int unusedInTheMiddle10_offset = 398;
	public static final int unusedInTheMiddle11_offset = 399;
	public static final int unusedInTheMiddle12_offset = 400;
	public static final int unusedInTheMiddle13_offset = 401;
	public static final int unusedInTheMiddle14_offset = 402;
	public static final int unusedInTheMiddle15_offset = 403;
	public static final int unusedInTheMiddle1_offset = 389;
	public static final int unusedInTheMiddle2_offset = 390;
	public static final int unusedInTheMiddle3_offset = 391;
	public static final int unusedInTheMiddle4_offset = 392;
	public static final int unusedInTheMiddle5_offset = 393;
	public static final int unusedInTheMiddle6_offset = 394;
	public static final int unusedInTheMiddle7_offset = 395;
	public static final int unusedInTheMiddle8_offset = 396;
	public static final int unusedInTheMiddle9_offset = 397;
	public static final int VBAT_INJECTOR_CURVE_SIZE = 8;
	public static final int VBatt_offset = 38;
	public static final int VEHICLE_INFO_SIZE = 32;
	public static final int vehicleSpeedKph_offset = 10;
	public static final int veTableYAxis_offset = 104;
	public static final int veValue_offset = 57;
	public static final int VM_VVT_INACTIVE = 0;
	public static final int VR_THRESHOLD_COUNT = 2;
	public static final int VssAcceleration_offset = 284;
	public static final int vvtCounter_offset = 437;
	public static final int vvtCurrentPosition_offset = 428;
	public static final int vvtPositionB1E_offset = 290;
	public static final int vvtPositionB1I_offset = 42;
	public static final int vvtPositionB2E_offset = 294;
	public static final int vvtPositionB2I_offset = 292;
	public static final int vvtStateIndex_offset = 439;
	public static final int vvtSyncCounter_offset = 438;
	public static final int vvtSyncGapRatio_offset = 424;
	public static final int vvtTargets1_offset = 314;
	public static final int vvtTargets2_offset = 315;
	public static final int vvtTargets3_offset = 316;
	public static final int vvtTargets4_offset = 317;
	public static final int wallFuelAmount_offset = 72;
	public static final int wallFuelCorrection_offset = 74;
	public static final int warningCounter_offset = 148;
	public static final int wastegatePositionSensor_offset = 268;
	public static final int wbHeaterDuty1_offset = 324;
	public static final int wbHeaterDuty2_offset = 325;
	public static final int wbTemperature1_offset = 320;
	public static final int wbTemperature2_offset = 322;
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
	public static final Field CLUTCHUPSTATE = Field.create("CLUTCHUPSTATE", 0, FieldType.BIT, 11);
	public static final Field CLUTCHDOWNSTATE = Field.create("CLUTCHDOWNSTATE", 0, FieldType.BIT, 12);
	public static final Field ISFAN2ON = Field.create("ISFAN2ON", 0, FieldType.BIT, 13);
	public static final Field ALTERNATORONOFF = Field.create("ALTERNATORONOFF", 0, FieldType.BIT, 14);
	public static final Field BRAKEPEDALSTATE = Field.create("BRAKEPEDALSTATE", 0, FieldType.BIT, 15);
	public static final Field TOOTHLOGREADY = Field.create("TOOTHLOGREADY", 0, FieldType.BIT, 16);
	public static final Field ACSWITCHSTATE = Field.create("ACSWITCHSTATE", 0, FieldType.BIT, 17);
	public static final Field ISTPSERROR = Field.create("ISTPSERROR", 0, FieldType.BIT, 18);
	public static final Field ISCLTERROR = Field.create("ISCLTERROR", 0, FieldType.BIT, 19);
	public static final Field ISMAPERROR = Field.create("ISMAPERROR", 0, FieldType.BIT, 20);
	public static final Field ISIATERROR = Field.create("ISIATERROR", 0, FieldType.BIT, 21);
	public static final Field ACSTATE = Field.create("ACSTATE", 0, FieldType.BIT, 22);
	public static final Field ISTRIGGERERROR = Field.create("ISTRIGGERERROR", 0, FieldType.BIT, 23);
	public static final Field HASCRITICALERROR = Field.create("HASCRITICALERROR", 0, FieldType.BIT, 24);
	public static final Field ISWARNNOW = Field.create("ISWARNNOW", 0, FieldType.BIT, 25);
	public static final Field ISPEDALERROR = Field.create("ISPEDALERROR", 0, FieldType.BIT, 26);
	public static final Field ISKNOCKCHIPOK = Field.create("ISKNOCKCHIPOK", 0, FieldType.BIT, 27);
	public static final Field LAUNCHTRIGGERED = Field.create("LAUNCHTRIGGERED", 0, FieldType.BIT, 28);
	public static final Field ISTPS2ERROR = Field.create("ISTPS2ERROR", 0, FieldType.BIT, 29);
	public static final Field ISIDLECLOSEDLOOP = Field.create("ISIDLECLOSEDLOOP", 0, FieldType.BIT, 30);
	public static final Field ISIDLECOASTING = Field.create("ISIDLECOASTING", 0, FieldType.BIT, 31);
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
	public static final Field WBTEMPERATURE1 = Field.create("WBTEMPERATURE1", 320, FieldType.INT16).setScale(1.0);
	public static final Field WBTEMPERATURE2 = Field.create("WBTEMPERATURE2", 322, FieldType.INT16).setScale(1.0);
	public static final Field WBHEATERDUTY1 = Field.create("WBHEATERDUTY1", 324, FieldType.INT8).setScale(1.0);
	public static final Field WBHEATERDUTY2 = Field.create("WBHEATERDUTY2", 325, FieldType.INT8).setScale(1.0);
	public static final Field TPS1SPLIT = Field.create("TPS1SPLIT", 326, FieldType.INT16).setScale(0.01);
	public static final Field TPS2SPLIT = Field.create("TPS2SPLIT", 328, FieldType.INT16).setScale(0.01);
	public static final Field TPS12SPLIT = Field.create("TPS12SPLIT", 330, FieldType.INT16).setScale(0.01);
	public static final Field ACCPEDALSPLIT = Field.create("ACCPEDALSPLIT", 332, FieldType.INT16).setScale(0.01);
	public static final Field M_REQUESTED_PUMP = Field.create("M_REQUESTED_PUMP", 334, FieldType.INT16).setScale(0.02);
	public static final Field BOOSTCONTROLTARGET = Field.create("BOOSTCONTROLTARGET", 336, FieldType.INT16).setScale(0.03333333333333333);
	public static final Field SPARKCUTREASON = Field.create("SPARKCUTREASON", 338, FieldType.INT8).setScale(1.0);
	public static final Field FUELCUTREASON = Field.create("FUELCUTREASON", 339, FieldType.INT8).setScale(1.0);
	public static final Field FUEL_REQUESTED_PERCENT = Field.create("FUEL_REQUESTED_PERCENT", 340, FieldType.FLOAT);
	public static final Field FUEL_REQUESTED_PERCENT_PI = Field.create("FUEL_REQUESTED_PERCENT_PI", 344, FieldType.FLOAT);
	public static final Field M_I_SUM_PERCENT = Field.create("M_I_SUM_PERCENT", 348, FieldType.FLOAT);
	public static final Field M_PRESSURETARGET_KPA = Field.create("M_PRESSURETARGET_KPA", 352, FieldType.FLOAT);
	public static final Field MAFESTIMATE = Field.create("MAFESTIMATE", 356, FieldType.INT16).setScale(0.1);
	public static final Field INSTANTRPM = Field.create("INSTANTRPM", 358, FieldType.INT16).setScale(1.0);
	public static final Field DI_NEXTSTART = Field.create("DI_NEXTSTART", 360, FieldType.FLOAT);
	public static final Field SYSTEMEVENTREUSE = Field.create("SYSTEMEVENTREUSE", 364, FieldType.INT16).setScale(1.0);
	public static final Field RAWMAP = Field.create("RAWMAP", 366, FieldType.INT16).setScale(0.001);
	public static final Field RAWAFR = Field.create("RAWAFR", 368, FieldType.INT16).setScale(0.001);
	public static final Field TPSACCELFROM = Field.create("TPSACCELFROM", 370, FieldType.INT8).setScale(1.0);
	public static final Field TPSACCELTO = Field.create("TPSACCELTO", 371, FieldType.INT8).setScale(1.0);
	public static final Field CALIBRATIONVALUE2 = Field.create("CALIBRATIONVALUE2", 372, FieldType.FLOAT);
	public static final Field ISMAINRELAYON = Field.create("ISMAINRELAYON", 376, FieldType.BIT, 0);
	public static final Field ISUSBCONNECTED = Field.create("ISUSBCONNECTED", 376, FieldType.BIT, 1);
	public static final Field UNUSEDBIT_182_2 = Field.create("UNUSEDBIT_182_2", 376, FieldType.BIT, 2);
	public static final Field UNUSEDBIT_182_3 = Field.create("UNUSEDBIT_182_3", 376, FieldType.BIT, 3);
	public static final Field UNUSEDBIT_182_4 = Field.create("UNUSEDBIT_182_4", 376, FieldType.BIT, 4);
	public static final Field UNUSEDBIT_182_5 = Field.create("UNUSEDBIT_182_5", 376, FieldType.BIT, 5);
	public static final Field UNUSEDBIT_182_6 = Field.create("UNUSEDBIT_182_6", 376, FieldType.BIT, 6);
	public static final Field UNUSEDBIT_182_7 = Field.create("UNUSEDBIT_182_7", 376, FieldType.BIT, 7);
	public static final Field UNUSEDBIT_182_8 = Field.create("UNUSEDBIT_182_8", 376, FieldType.BIT, 8);
	public static final Field UNUSEDBIT_182_9 = Field.create("UNUSEDBIT_182_9", 376, FieldType.BIT, 9);
	public static final Field UNUSEDBIT_182_10 = Field.create("UNUSEDBIT_182_10", 376, FieldType.BIT, 10);
	public static final Field UNUSEDBIT_182_11 = Field.create("UNUSEDBIT_182_11", 376, FieldType.BIT, 11);
	public static final Field UNUSEDBIT_182_12 = Field.create("UNUSEDBIT_182_12", 376, FieldType.BIT, 12);
	public static final Field UNUSEDBIT_182_13 = Field.create("UNUSEDBIT_182_13", 376, FieldType.BIT, 13);
	public static final Field UNUSEDBIT_182_14 = Field.create("UNUSEDBIT_182_14", 376, FieldType.BIT, 14);
	public static final Field UNUSEDBIT_182_15 = Field.create("UNUSEDBIT_182_15", 376, FieldType.BIT, 15);
	public static final Field UNUSEDBIT_182_16 = Field.create("UNUSEDBIT_182_16", 376, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_182_17 = Field.create("UNUSEDBIT_182_17", 376, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_182_18 = Field.create("UNUSEDBIT_182_18", 376, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_182_19 = Field.create("UNUSEDBIT_182_19", 376, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_182_20 = Field.create("UNUSEDBIT_182_20", 376, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_182_21 = Field.create("UNUSEDBIT_182_21", 376, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_182_22 = Field.create("UNUSEDBIT_182_22", 376, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_182_23 = Field.create("UNUSEDBIT_182_23", 376, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_182_24 = Field.create("UNUSEDBIT_182_24", 376, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_182_25 = Field.create("UNUSEDBIT_182_25", 376, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_182_26 = Field.create("UNUSEDBIT_182_26", 376, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_182_27 = Field.create("UNUSEDBIT_182_27", 376, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_182_28 = Field.create("UNUSEDBIT_182_28", 376, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_182_29 = Field.create("UNUSEDBIT_182_29", 376, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_182_30 = Field.create("UNUSEDBIT_182_30", 376, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_182_31 = Field.create("UNUSEDBIT_182_31", 376, FieldType.BIT, 31);
	public static final Field LUAINVOCATIONCOUNTER = Field.create("LUAINVOCATIONCOUNTER", 380, FieldType.INT).setScale(1.0);
	public static final Field LUALASTCYCLEDURATION = Field.create("LUALASTCYCLEDURATION", 384, FieldType.INT).setScale(1.0);
	public static final Field TESTBENCHITER = Field.create("TESTBENCHITER", 388, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE1 = Field.create("UNUSEDINTHEMIDDLE1", 389, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE2 = Field.create("UNUSEDINTHEMIDDLE2", 390, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE3 = Field.create("UNUSEDINTHEMIDDLE3", 391, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE4 = Field.create("UNUSEDINTHEMIDDLE4", 392, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE5 = Field.create("UNUSEDINTHEMIDDLE5", 393, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE6 = Field.create("UNUSEDINTHEMIDDLE6", 394, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE7 = Field.create("UNUSEDINTHEMIDDLE7", 395, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE8 = Field.create("UNUSEDINTHEMIDDLE8", 396, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE9 = Field.create("UNUSEDINTHEMIDDLE9", 397, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE10 = Field.create("UNUSEDINTHEMIDDLE10", 398, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE11 = Field.create("UNUSEDINTHEMIDDLE11", 399, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE12 = Field.create("UNUSEDINTHEMIDDLE12", 400, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE13 = Field.create("UNUSEDINTHEMIDDLE13", 401, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE14 = Field.create("UNUSEDINTHEMIDDLE14", 402, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDINTHEMIDDLE15 = Field.create("UNUSEDINTHEMIDDLE15", 403, FieldType.INT8).setScale(1.0);
	public static final Field AUXLINEAR1 = Field.create("AUXLINEAR1", 404, FieldType.INT16).setScale(0.01);
	public static final Field AUXLINEAR2 = Field.create("AUXLINEAR2", 406, FieldType.INT16).setScale(0.01);
	public static final Field LAUNCHSPEEDCONDITION = Field.create("LAUNCHSPEEDCONDITION", 408, FieldType.BIT, 0);
	public static final Field LAUNCHRPMCONDITION = Field.create("LAUNCHRPMCONDITION", 408, FieldType.BIT, 1);
	public static final Field LAUNCHTPSCONDITION = Field.create("LAUNCHTPSCONDITION", 408, FieldType.BIT, 2);
	public static final Field LAUNCHACTIVATESWITCHCONDITION = Field.create("LAUNCHACTIVATESWITCHCONDITION", 408, FieldType.BIT, 3);
	public static final Field LAUNCHISLAUNCHCONDITION = Field.create("LAUNCHISLAUNCHCONDITION", 408, FieldType.BIT, 4);
	public static final Field LAUNCHCOMBINEDCONDITIONS = Field.create("LAUNCHCOMBINEDCONDITIONS", 408, FieldType.BIT, 5);
	public static final Field LAUNCHACTIVATEPINSTATE = Field.create("LAUNCHACTIVATEPINSTATE", 408, FieldType.BIT, 6);
	public static final Field DFCOACTIVE = Field.create("DFCOACTIVE", 408, FieldType.BIT, 7);
	public static final Field TPSACCELACTIVE = Field.create("TPSACCELACTIVE", 408, FieldType.BIT, 8);
	public static final Field UNUSEDBIT_227_9 = Field.create("UNUSEDBIT_227_9", 408, FieldType.BIT, 9);
	public static final Field UNUSEDBIT_227_10 = Field.create("UNUSEDBIT_227_10", 408, FieldType.BIT, 10);
	public static final Field UNUSEDBIT_227_11 = Field.create("UNUSEDBIT_227_11", 408, FieldType.BIT, 11);
	public static final Field UNUSEDBIT_227_12 = Field.create("UNUSEDBIT_227_12", 408, FieldType.BIT, 12);
	public static final Field UNUSEDBIT_227_13 = Field.create("UNUSEDBIT_227_13", 408, FieldType.BIT, 13);
	public static final Field UNUSEDBIT_227_14 = Field.create("UNUSEDBIT_227_14", 408, FieldType.BIT, 14);
	public static final Field UNUSEDBIT_227_15 = Field.create("UNUSEDBIT_227_15", 408, FieldType.BIT, 15);
	public static final Field UNUSEDBIT_227_16 = Field.create("UNUSEDBIT_227_16", 408, FieldType.BIT, 16);
	public static final Field UNUSEDBIT_227_17 = Field.create("UNUSEDBIT_227_17", 408, FieldType.BIT, 17);
	public static final Field UNUSEDBIT_227_18 = Field.create("UNUSEDBIT_227_18", 408, FieldType.BIT, 18);
	public static final Field UNUSEDBIT_227_19 = Field.create("UNUSEDBIT_227_19", 408, FieldType.BIT, 19);
	public static final Field UNUSEDBIT_227_20 = Field.create("UNUSEDBIT_227_20", 408, FieldType.BIT, 20);
	public static final Field UNUSEDBIT_227_21 = Field.create("UNUSEDBIT_227_21", 408, FieldType.BIT, 21);
	public static final Field UNUSEDBIT_227_22 = Field.create("UNUSEDBIT_227_22", 408, FieldType.BIT, 22);
	public static final Field UNUSEDBIT_227_23 = Field.create("UNUSEDBIT_227_23", 408, FieldType.BIT, 23);
	public static final Field UNUSEDBIT_227_24 = Field.create("UNUSEDBIT_227_24", 408, FieldType.BIT, 24);
	public static final Field UNUSEDBIT_227_25 = Field.create("UNUSEDBIT_227_25", 408, FieldType.BIT, 25);
	public static final Field UNUSEDBIT_227_26 = Field.create("UNUSEDBIT_227_26", 408, FieldType.BIT, 26);
	public static final Field UNUSEDBIT_227_27 = Field.create("UNUSEDBIT_227_27", 408, FieldType.BIT, 27);
	public static final Field UNUSEDBIT_227_28 = Field.create("UNUSEDBIT_227_28", 408, FieldType.BIT, 28);
	public static final Field UNUSEDBIT_227_29 = Field.create("UNUSEDBIT_227_29", 408, FieldType.BIT, 29);
	public static final Field UNUSEDBIT_227_30 = Field.create("UNUSEDBIT_227_30", 408, FieldType.BIT, 30);
	public static final Field UNUSEDBIT_227_31 = Field.create("UNUSEDBIT_227_31", 408, FieldType.BIT, 31);
	public static final Field TEMPLOG_MAP_AT_CYCLE_COUNT = Field.create("TEMPLOG_MAP_AT_CYCLE_COUNT", 412, FieldType.INT8).setScale(1.0);
	public static final Field TEMPLOG_MAP_PEAK = Field.create("TEMPLOG_MAP_PEAK", 413, FieldType.INT8).setScale(1.0);
	public static final Field BOOSTCONTROLLEROUTPUT = Field.create("BOOSTCONTROLLEROUTPUT", 414, FieldType.INT8).setScale(0.5);
	public static final Field BOOSTCONTROLLEROPENLOOPPART = Field.create("BOOSTCONTROLLEROPENLOOPPART", 415, FieldType.INT8).setScale(0.5);
	public static final Field TEMPLOG_MAP_AT_SPECIAL_POINT = Field.create("TEMPLOG_MAP_AT_SPECIAL_POINT", 416, FieldType.FLOAT);
	public static final Field TEMPLOG_MAP_AT_DIFF = Field.create("TEMPLOG_MAP_AT_DIFF", 420, FieldType.FLOAT);
	public static final Field VVTSYNCGAPRATIO = Field.create("VVTSYNCGAPRATIO", 424, FieldType.FLOAT);
	public static final Field VVTCURRENTPOSITION = Field.create("VVTCURRENTPOSITION", 428, FieldType.FLOAT);
	public static final Field TRIGGERSYNCGAPRATIO = Field.create("TRIGGERSYNCGAPRATIO", 432, FieldType.FLOAT);
	public static final Field TRIGGERSTATEINDEX = Field.create("TRIGGERSTATEINDEX", 436, FieldType.INT8).setScale(1.0);
	public static final Field VVTCOUNTER = Field.create("VVTCOUNTER", 437, FieldType.INT8).setScale(1.0);
	public static final Field VVTSYNCCOUNTER = Field.create("VVTSYNCCOUNTER", 438, FieldType.INT8).setScale(1.0);
	public static final Field VVTSTATEINDEX = Field.create("VVTSTATEINDEX", 439, FieldType.INT8).setScale(1.0);
	public static final Field FALLBACKMAP = Field.create("FALLBACKMAP", 440, FieldType.INT16).setScale(0.1);
	public static final Field BOOSTCONTROLLERCLOSEDLOOPPART = Field.create("BOOSTCONTROLLERCLOSEDLOOPPART", 442, FieldType.INT8).setScale(0.5);
	public static final Field DETECTEDGEAR = Field.create("DETECTEDGEAR", 443, FieldType.INT8).setScale(1.0);
	public static final Field TIMINGCLTCORRECTION = Field.create("TIMINGCLTCORRECTION", 444, FieldType.INT16).setScale(0.01);
	public static final Field TIMINGIATCORRECTION = Field.create("TIMINGIATCORRECTION", 446, FieldType.INT16).setScale(0.01);
	public static final Field TIMINGPIDCORRECTION = Field.create("TIMINGPIDCORRECTION", 448, FieldType.INT16).setScale(0.01);
	public static final Field INSTANTMAPVALUE = Field.create("INSTANTMAPVALUE", 450, FieldType.INT16).setScale(0.03333333333333333);
	public static final Field MOSTRECENTTIMEBETWEENSPARKEVENTS = Field.create("MOSTRECENTTIMEBETWEENSPARKEVENTS", 452, FieldType.INT16).setScale(1.0);
	public static final Field MOSTRECENTTIMEBETWEENIGNITIONEVENTS = Field.create("MOSTRECENTTIMEBETWEENIGNITIONEVENTS", 454, FieldType.INT16).setScale(1.0);
	public static final Field MAXLOCKEDDURATION = Field.create("MAXLOCKEDDURATION", 456, FieldType.INT16).setScale(1.0);
	public static final Field MAXTRIGGERREENTRANT = Field.create("MAXTRIGGERREENTRANT", 458, FieldType.INT16).setScale(1.0);
	public static final Field CANWRITEOK = Field.create("CANWRITEOK", 460, FieldType.INT16).setScale(1.0);
	public static final Field CANWRITENOTOK = Field.create("CANWRITENOTOK", 462, FieldType.INT16).setScale(1.0);
	public static final Field TRIGGERPRIMARYFALL = Field.create("TRIGGERPRIMARYFALL", 464, FieldType.INT).setScale(1.0);
	public static final Field TRIGGERPRIMARYRISE = Field.create("TRIGGERPRIMARYRISE", 468, FieldType.INT).setScale(1.0);
	public static final Field TRIGGERSECONDARYFALL = Field.create("TRIGGERSECONDARYFALL", 472, FieldType.INT).setScale(1.0);
	public static final Field TRIGGERSECONDARYRISE = Field.create("TRIGGERSECONDARYRISE", 476, FieldType.INT).setScale(1.0);
	public static final Field TRIGGERVVTFALL = Field.create("TRIGGERVVTFALL", 480, FieldType.INT).setScale(1.0);
	public static final Field TRIGGERVVTRISE = Field.create("TRIGGERVVTRISE", 484, FieldType.INT).setScale(1.0);
	public static final Field STARTSTOPSTATETOGGLECOUNTER = Field.create("STARTSTOPSTATETOGGLECOUNTER", 488, FieldType.INT8).setScale(1.0);
	public static final Field STARTERSTATE = Field.create("STARTERSTATE", 489, FieldType.INT8).setScale(1.0);
	public static final Field STARTERRELAYDISABLE = Field.create("STARTERRELAYDISABLE", 490, FieldType.INT8).setScale(1.0);
	public static final Field MULTISPARKCOUNTER = Field.create("MULTISPARKCOUNTER", 491, FieldType.INT8).setScale(1.0);
	public static final Field IDLESTATE = Field.create("IDLESTATE", 492, FieldType.INT).setScale(1.0);
	public static final Field INJECTORFLOWPRESSURERATIO = Field.create("INJECTORFLOWPRESSURERATIO", 496, FieldType.FLOAT);
	public static final Field INJECTORFLOWPRESSUREDELTA = Field.create("INJECTORFLOWPRESSUREDELTA", 500, FieldType.FLOAT);
	public static final Field ETBFEEDFORWARD = Field.create("ETBFEEDFORWARD", 504, FieldType.FLOAT);
	public static final Field ETBINTEGRALERROR = Field.create("ETBINTEGRALERROR", 508, FieldType.FLOAT);
	public static final Field ETBCURRENTTARGET = Field.create("ETBCURRENTTARGET", 512, FieldType.FLOAT);
	public static final Field ALTERNATORSTATUS_ITERM = Field.create("ALTERNATORSTATUS_ITERM", 516, FieldType.INT16).setScale(0.01);
	public static final Field ALTERNATORSTATUS_DTERM = Field.create("ALTERNATORSTATUS_DTERM", 518, FieldType.INT16).setScale(0.01);
	public static final Field ALTERNATORSTATUS_OUTPUT = Field.create("ALTERNATORSTATUS_OUTPUT", 520, FieldType.INT16).setScale(0.01);
	public static final Field ALTERNATORSTATUS_ERROR = Field.create("ALTERNATORSTATUS_ERROR", 522, FieldType.INT16).setScale(0.01);
	public static final Field ALTERNATORSTATUS_RESETCOUNTER = Field.create("ALTERNATORSTATUS_RESETCOUNTER", 524, FieldType.INT).setScale(1.0);
	public static final Field IDLESTATUS_ITERM = Field.create("IDLESTATUS_ITERM", 528, FieldType.INT16).setScale(0.01);
	public static final Field IDLESTATUS_DTERM = Field.create("IDLESTATUS_DTERM", 530, FieldType.INT16).setScale(0.01);
	public static final Field IDLESTATUS_OUTPUT = Field.create("IDLESTATUS_OUTPUT", 532, FieldType.INT16).setScale(0.01);
	public static final Field IDLESTATUS_ERROR = Field.create("IDLESTATUS_ERROR", 534, FieldType.INT16).setScale(0.01);
	public static final Field IDLESTATUS_RESETCOUNTER = Field.create("IDLESTATUS_RESETCOUNTER", 536, FieldType.INT).setScale(1.0);
	public static final Field ETBSTATUS_ITERM = Field.create("ETBSTATUS_ITERM", 540, FieldType.INT16).setScale(0.01);
	public static final Field ETBSTATUS_DTERM = Field.create("ETBSTATUS_DTERM", 542, FieldType.INT16).setScale(0.01);
	public static final Field ETBSTATUS_OUTPUT = Field.create("ETBSTATUS_OUTPUT", 544, FieldType.INT16).setScale(0.01);
	public static final Field ETBSTATUS_ERROR = Field.create("ETBSTATUS_ERROR", 546, FieldType.INT16).setScale(0.01);
	public static final Field ETBSTATUS_RESETCOUNTER = Field.create("ETBSTATUS_RESETCOUNTER", 548, FieldType.INT).setScale(1.0);
	public static final Field BOOSTSTATUS_ITERM = Field.create("BOOSTSTATUS_ITERM", 552, FieldType.INT16).setScale(0.01);
	public static final Field BOOSTSTATUS_DTERM = Field.create("BOOSTSTATUS_DTERM", 554, FieldType.INT16).setScale(0.01);
	public static final Field BOOSTSTATUS_OUTPUT = Field.create("BOOSTSTATUS_OUTPUT", 556, FieldType.INT16).setScale(0.01);
	public static final Field BOOSTSTATUS_ERROR = Field.create("BOOSTSTATUS_ERROR", 558, FieldType.INT16).setScale(0.01);
	public static final Field BOOSTSTATUS_RESETCOUNTER = Field.create("BOOSTSTATUS_RESETCOUNTER", 560, FieldType.INT).setScale(1.0);
	public static final Field AUXSPEED1 = Field.create("AUXSPEED1", 564, FieldType.INT16).setScale(1.0);
	public static final Field AUXSPEED2 = Field.create("AUXSPEED2", 566, FieldType.INT16).setScale(1.0);
	public static final Field UNUSEDATTHEEND1 = Field.create("UNUSEDATTHEEND1", 568, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND2 = Field.create("UNUSEDATTHEEND2", 569, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND3 = Field.create("UNUSEDATTHEEND3", 570, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND4 = Field.create("UNUSEDATTHEEND4", 571, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND5 = Field.create("UNUSEDATTHEEND5", 572, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND6 = Field.create("UNUSEDATTHEEND6", 573, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND7 = Field.create("UNUSEDATTHEEND7", 574, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND8 = Field.create("UNUSEDATTHEEND8", 575, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND9 = Field.create("UNUSEDATTHEEND9", 576, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND10 = Field.create("UNUSEDATTHEEND10", 577, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND11 = Field.create("UNUSEDATTHEEND11", 578, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND12 = Field.create("UNUSEDATTHEEND12", 579, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND13 = Field.create("UNUSEDATTHEEND13", 580, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND14 = Field.create("UNUSEDATTHEEND14", 581, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND15 = Field.create("UNUSEDATTHEEND15", 582, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND16 = Field.create("UNUSEDATTHEEND16", 583, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND17 = Field.create("UNUSEDATTHEEND17", 584, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND18 = Field.create("UNUSEDATTHEEND18", 585, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND19 = Field.create("UNUSEDATTHEEND19", 586, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND20 = Field.create("UNUSEDATTHEEND20", 587, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND21 = Field.create("UNUSEDATTHEEND21", 588, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND22 = Field.create("UNUSEDATTHEEND22", 589, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND23 = Field.create("UNUSEDATTHEEND23", 590, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND24 = Field.create("UNUSEDATTHEEND24", 591, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND25 = Field.create("UNUSEDATTHEEND25", 592, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND26 = Field.create("UNUSEDATTHEEND26", 593, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND27 = Field.create("UNUSEDATTHEEND27", 594, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND28 = Field.create("UNUSEDATTHEEND28", 595, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND29 = Field.create("UNUSEDATTHEEND29", 596, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND30 = Field.create("UNUSEDATTHEEND30", 597, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND31 = Field.create("UNUSEDATTHEEND31", 598, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND32 = Field.create("UNUSEDATTHEEND32", 599, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND33 = Field.create("UNUSEDATTHEEND33", 600, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND34 = Field.create("UNUSEDATTHEEND34", 601, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND35 = Field.create("UNUSEDATTHEEND35", 602, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND36 = Field.create("UNUSEDATTHEEND36", 603, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND37 = Field.create("UNUSEDATTHEEND37", 604, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND38 = Field.create("UNUSEDATTHEEND38", 605, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND39 = Field.create("UNUSEDATTHEEND39", 606, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND40 = Field.create("UNUSEDATTHEEND40", 607, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND41 = Field.create("UNUSEDATTHEEND41", 608, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND42 = Field.create("UNUSEDATTHEEND42", 609, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND43 = Field.create("UNUSEDATTHEEND43", 610, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND44 = Field.create("UNUSEDATTHEEND44", 611, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND45 = Field.create("UNUSEDATTHEEND45", 612, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND46 = Field.create("UNUSEDATTHEEND46", 613, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND47 = Field.create("UNUSEDATTHEEND47", 614, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND48 = Field.create("UNUSEDATTHEEND48", 615, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND49 = Field.create("UNUSEDATTHEEND49", 616, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND50 = Field.create("UNUSEDATTHEEND50", 617, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND51 = Field.create("UNUSEDATTHEEND51", 618, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND52 = Field.create("UNUSEDATTHEEND52", 619, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND53 = Field.create("UNUSEDATTHEEND53", 620, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND54 = Field.create("UNUSEDATTHEEND54", 621, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND55 = Field.create("UNUSEDATTHEEND55", 622, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND56 = Field.create("UNUSEDATTHEEND56", 623, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND57 = Field.create("UNUSEDATTHEEND57", 624, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND58 = Field.create("UNUSEDATTHEEND58", 625, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND59 = Field.create("UNUSEDATTHEEND59", 626, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND60 = Field.create("UNUSEDATTHEEND60", 627, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND61 = Field.create("UNUSEDATTHEEND61", 628, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND62 = Field.create("UNUSEDATTHEEND62", 629, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND63 = Field.create("UNUSEDATTHEEND63", 630, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND64 = Field.create("UNUSEDATTHEEND64", 631, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND65 = Field.create("UNUSEDATTHEEND65", 632, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND66 = Field.create("UNUSEDATTHEEND66", 633, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND67 = Field.create("UNUSEDATTHEEND67", 634, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND68 = Field.create("UNUSEDATTHEEND68", 635, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND69 = Field.create("UNUSEDATTHEEND69", 636, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND70 = Field.create("UNUSEDATTHEEND70", 637, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND71 = Field.create("UNUSEDATTHEEND71", 638, FieldType.INT8).setScale(1.0);
	public static final Field UNUSEDATTHEEND72 = Field.create("UNUSEDATTHEEND72", 639, FieldType.INT8).setScale(1.0);
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
	CLUTCHUPSTATE,
	CLUTCHDOWNSTATE,
	ISFAN2ON,
	ALTERNATORONOFF,
	BRAKEPEDALSTATE,
	TOOTHLOGREADY,
	ACSWITCHSTATE,
	ISTPSERROR,
	ISCLTERROR,
	ISMAPERROR,
	ISIATERROR,
	ACSTATE,
	ISTRIGGERERROR,
	HASCRITICALERROR,
	ISWARNNOW,
	ISPEDALERROR,
	ISKNOCKCHIPOK,
	LAUNCHTRIGGERED,
	ISTPS2ERROR,
	ISIDLECLOSEDLOOP,
	ISIDLECOASTING,
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
	WBTEMPERATURE1,
	WBTEMPERATURE2,
	WBHEATERDUTY1,
	WBHEATERDUTY2,
	TPS1SPLIT,
	TPS2SPLIT,
	TPS12SPLIT,
	ACCPEDALSPLIT,
	M_REQUESTED_PUMP,
	BOOSTCONTROLTARGET,
	SPARKCUTREASON,
	FUELCUTREASON,
	FUEL_REQUESTED_PERCENT,
	FUEL_REQUESTED_PERCENT_PI,
	M_I_SUM_PERCENT,
	M_PRESSURETARGET_KPA,
	MAFESTIMATE,
	INSTANTRPM,
	DI_NEXTSTART,
	SYSTEMEVENTREUSE,
	RAWMAP,
	RAWAFR,
	TPSACCELFROM,
	TPSACCELTO,
	CALIBRATIONVALUE2,
	ISMAINRELAYON,
	ISUSBCONNECTED,
	UNUSEDBIT_182_2,
	UNUSEDBIT_182_3,
	UNUSEDBIT_182_4,
	UNUSEDBIT_182_5,
	UNUSEDBIT_182_6,
	UNUSEDBIT_182_7,
	UNUSEDBIT_182_8,
	UNUSEDBIT_182_9,
	UNUSEDBIT_182_10,
	UNUSEDBIT_182_11,
	UNUSEDBIT_182_12,
	UNUSEDBIT_182_13,
	UNUSEDBIT_182_14,
	UNUSEDBIT_182_15,
	UNUSEDBIT_182_16,
	UNUSEDBIT_182_17,
	UNUSEDBIT_182_18,
	UNUSEDBIT_182_19,
	UNUSEDBIT_182_20,
	UNUSEDBIT_182_21,
	UNUSEDBIT_182_22,
	UNUSEDBIT_182_23,
	UNUSEDBIT_182_24,
	UNUSEDBIT_182_25,
	UNUSEDBIT_182_26,
	UNUSEDBIT_182_27,
	UNUSEDBIT_182_28,
	UNUSEDBIT_182_29,
	UNUSEDBIT_182_30,
	UNUSEDBIT_182_31,
	LUAINVOCATIONCOUNTER,
	LUALASTCYCLEDURATION,
	TESTBENCHITER,
	UNUSEDINTHEMIDDLE1,
	UNUSEDINTHEMIDDLE2,
	UNUSEDINTHEMIDDLE3,
	UNUSEDINTHEMIDDLE4,
	UNUSEDINTHEMIDDLE5,
	UNUSEDINTHEMIDDLE6,
	UNUSEDINTHEMIDDLE7,
	UNUSEDINTHEMIDDLE8,
	UNUSEDINTHEMIDDLE9,
	UNUSEDINTHEMIDDLE10,
	UNUSEDINTHEMIDDLE11,
	UNUSEDINTHEMIDDLE12,
	UNUSEDINTHEMIDDLE13,
	UNUSEDINTHEMIDDLE14,
	UNUSEDINTHEMIDDLE15,
	AUXLINEAR1,
	AUXLINEAR2,
	LAUNCHSPEEDCONDITION,
	LAUNCHRPMCONDITION,
	LAUNCHTPSCONDITION,
	LAUNCHACTIVATESWITCHCONDITION,
	LAUNCHISLAUNCHCONDITION,
	LAUNCHCOMBINEDCONDITIONS,
	LAUNCHACTIVATEPINSTATE,
	DFCOACTIVE,
	TPSACCELACTIVE,
	UNUSEDBIT_227_9,
	UNUSEDBIT_227_10,
	UNUSEDBIT_227_11,
	UNUSEDBIT_227_12,
	UNUSEDBIT_227_13,
	UNUSEDBIT_227_14,
	UNUSEDBIT_227_15,
	UNUSEDBIT_227_16,
	UNUSEDBIT_227_17,
	UNUSEDBIT_227_18,
	UNUSEDBIT_227_19,
	UNUSEDBIT_227_20,
	UNUSEDBIT_227_21,
	UNUSEDBIT_227_22,
	UNUSEDBIT_227_23,
	UNUSEDBIT_227_24,
	UNUSEDBIT_227_25,
	UNUSEDBIT_227_26,
	UNUSEDBIT_227_27,
	UNUSEDBIT_227_28,
	UNUSEDBIT_227_29,
	UNUSEDBIT_227_30,
	UNUSEDBIT_227_31,
	TEMPLOG_MAP_AT_CYCLE_COUNT,
	TEMPLOG_MAP_PEAK,
	BOOSTCONTROLLEROUTPUT,
	BOOSTCONTROLLEROPENLOOPPART,
	TEMPLOG_MAP_AT_SPECIAL_POINT,
	TEMPLOG_MAP_AT_DIFF,
	VVTSYNCGAPRATIO,
	VVTCURRENTPOSITION,
	TRIGGERSYNCGAPRATIO,
	TRIGGERSTATEINDEX,
	VVTCOUNTER,
	VVTSYNCCOUNTER,
	VVTSTATEINDEX,
	FALLBACKMAP,
	BOOSTCONTROLLERCLOSEDLOOPPART,
	DETECTEDGEAR,
	TIMINGCLTCORRECTION,
	TIMINGIATCORRECTION,
	TIMINGPIDCORRECTION,
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
	IDLESTATE,
	INJECTORFLOWPRESSURERATIO,
	INJECTORFLOWPRESSUREDELTA,
	ETBFEEDFORWARD,
	ETBINTEGRALERROR,
	ETBCURRENTTARGET,
	ALTERNATORSTATUS_ITERM,
	ALTERNATORSTATUS_DTERM,
	ALTERNATORSTATUS_OUTPUT,
	ALTERNATORSTATUS_ERROR,
	ALTERNATORSTATUS_RESETCOUNTER,
	IDLESTATUS_ITERM,
	IDLESTATUS_DTERM,
	IDLESTATUS_OUTPUT,
	IDLESTATUS_ERROR,
	IDLESTATUS_RESETCOUNTER,
	ETBSTATUS_ITERM,
	ETBSTATUS_DTERM,
	ETBSTATUS_OUTPUT,
	ETBSTATUS_ERROR,
	ETBSTATUS_RESETCOUNTER,
	BOOSTSTATUS_ITERM,
	BOOSTSTATUS_DTERM,
	BOOSTSTATUS_OUTPUT,
	BOOSTSTATUS_ERROR,
	BOOSTSTATUS_RESETCOUNTER,
	AUXSPEED1,
	AUXSPEED2,
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
	};
}
