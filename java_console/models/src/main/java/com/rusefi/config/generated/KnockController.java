package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class KnockController {
	public static final int BANKS_COUNT = 2;
	public static final int BOOST_BLEND_COUNT = 2;
	public static final int CAM_INPUTS_COUNT = 4;
	public static final int CAMS_PER_BANK = 2;
	public static final int EGT_CHANNEL_COUNT = 8;
	public static final String GAUGE_CATEGORY_BOOST_CONTROL = "Boost Control";
	public static final String GAUGE_CATEGORY_ETB = "ETB more";
	public static final String GAUGE_CATEGORY_FUEL_MATH = "Fuel: math";
	public static final String GAUGE_CATEGORY_SYNC = "Sync";
	public static final String GAUGE_CATEGORY_TIMING = "Timing";
	public static final String GAUGE_COIL_DWELL_TIME = "Ignition: coil charge time";
	public static final String GAUGE_NAME_AC_PRESSURE = "A/C pressure";
	public static final String GAUGE_NAME_ACCEL_LAT = "Accel: Lateral";
	public static final String GAUGE_NAME_ACCEL_LON = "Accel: Longitudinal";
	public static final String GAUGE_NAME_ACCEL_VERT = "Accel: Vertical";
	public static final String GAUGE_NAME_ADJUSTED_TIMING = "Timing: ignition";
	public static final String GAUGE_NAME_AFR = "Air/Fuel Ratio";
	public static final String GAUGE_NAME_AFR2 = "Air/Fuel Ratio 2";
	public static final String GAUGE_NAME_AFR2_GAS_SCALE = "Air/Fuel Ratio 2 (Gas Scale)";
	public static final String GAUGE_NAME_AFR_GAS_SCALE = "Air/Fuel Ratio (Gas Scale)";
	public static final String GAUGE_NAME_AIR_FLOW_ESTIMATE = "Air: Flow estimate";
	public static final String GAUGE_NAME_AIR_FLOW_MEASURED = "MAF";
	public static final String GAUGE_NAME_AIR_FLOW_MEASURED_2 = "MAF #2";
	public static final String GAUGE_NAME_AIR_MASS = "Air: Cylinder airmass";
	public static final String GAUGE_NAME_AUX_LINEAR_1 = "Aux linear #1";
	public static final String GAUGE_NAME_AUX_LINEAR_2 = "Aux linear #2";
	public static final String GAUGE_NAME_AUX_LINEAR_3 = "Aux linear #3";
	public static final String GAUGE_NAME_AUX_LINEAR_4 = "Aux linear #4";
	public static final String GAUGE_NAME_AUX_TEMP1 = "Aux temp 1";
	public static final String GAUGE_NAME_AUX_TEMP2 = "Aux temp 2";
	public static final String GAUGE_NAME_BARO_PRESSURE = "Barometric pressure";
	public static final String GAUGE_NAME_BOOST_CLOSED_LOOP = "Boost: Closed loop";
	public static final String GAUGE_NAME_BOOST_OPEN_LOOP = "Boost: Open loop";
	public static final String GAUGE_NAME_BOOST_OUTPUT = "Boost: Output";
	public static final String GAUGE_NAME_BOOST_TARGET = "Boost: Target";
	public static final String GAUGE_NAME_CAN_READ_OK = "CAN: Rx";
	public static final String GAUGE_NAME_CAN_WRITE_ERR = "CAN: Tx err";
	public static final String GAUGE_NAME_CAN_WRITE_OK = "CAN: Tx OK";
	public static final String GAUGE_NAME_CLT = "CLT";
	public static final String GAUGE_NAME_CPU_TEMP = "CPU Temperature";
	public static final String GAUGE_NAME_CURRENT_GEAR = "TCU: Current Gear";
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
	public static final String GAUGE_NAME_DESIRED_GEAR = "TCU: Desired Gear";
	public static final String GAUGE_NAME_DETECTED_GEAR = "Detected Gear";
	public static final String GAUGE_NAME_DWELL_DUTY = "Ignition: coil duty cycle";
	public static final String GAUGE_NAME_ECU_TEMPERATURE = "ECU temperature";
	public static final String GAUGE_NAME_ENGINE_CRC16 = "Engine CRC16";
	public static final String GAUGE_NAME_ETB_DUTY = "ETB: Duty";
	public static final String GAUGE_NAME_ETB_ERROR = "ETB: position error";
	public static final String GAUGE_NAME_ETB_TARGET = "ETB: position target";
	public static final String GAUGE_NAME_FLEX = "Flex Ethanol %";
	public static final String GAUGE_NAME_FUEL_BARO_CORR = "Fuel: Barometric pressure mult";
	public static final String GAUGE_NAME_FUEL_BASE = "Fuel: base cycle mass";
	public static final String GAUGE_NAME_FUEL_CHARGE_TEMP = "Fuel: Estimated charge temperature";
	public static final String GAUGE_NAME_FUEL_CLT_CORR = "Fuel: CLT correction";
	public static final String GAUGE_NAME_FUEL_CONSUMPTION = "Fuel: Total consumed";
	public static final String GAUGE_NAME_FUEL_CRANKING = "Fuel: cranking";
	public static final String GAUGE_NAME_FUEL_FLOW = "Fuel: Flow rate";
	public static final String GAUGE_NAME_FUEL_IAT_CORR = "Fuel: IAT correction";
	public static final String GAUGE_NAME_FUEL_INJ_DUTY = "Fuel: injector duty cycle";
	public static final String GAUGE_NAME_FUEL_INJ_DUTY_STAGE_2 = "Fuel: injector duty cycle stage 2";
	public static final String GAUGE_NAME_FUEL_INJECTION_TIMING = "Fuel: Injection timing SOI";
	public static final String GAUGE_NAME_FUEL_LAST_INJECTION = "Fuel: Last inj pulse width";
	public static final String GAUGE_NAME_FUEL_LAST_INJECTION_STAGE_2 = "Fuel: Last inj pulse width stg 2";
	public static final String GAUGE_NAME_FUEL_LEVEL = "Fuel level";
	public static final String GAUGE_NAME_FUEL_LOAD = "Fuel: Load";
	public static final String GAUGE_NAME_FUEL_PRESSURE_HIGH = "Fuel pressure (high)";
	public static final String GAUGE_NAME_FUEL_PRESSURE_HIGH_UNITS = "bar";
	public static final String GAUGE_NAME_FUEL_PRESSURE_LOW = "Fuel pressure (low)";
	public static final String GAUGE_NAME_FUEL_PRESSURE_LOW_UNITS = "kPa";
	public static final String GAUGE_NAME_FUEL_RUNNING = "Fuel: running";
	public static final String GAUGE_NAME_FUEL_TEMPERATURE = "Fuel Temperature";
	public static final String GAUGE_NAME_FUEL_TPS_EXTRA = "Fuel: TPS AE add fuel ms";
	public static final String GAUGE_NAME_FUEL_TRIM = "Fuel: fuel trim";
	public static final String GAUGE_NAME_FUEL_TRIM_2 = "Fuel: fuel trim 2";
	public static final String GAUGE_NAME_FUEL_VE = "Fuel: VE";
	public static final String GAUGE_NAME_FUEL_WALL_AMOUNT = "Fuel: wall amount";
	public static final String GAUGE_NAME_FUEL_WALL_CORRECTION = "Fuel: wall correction";
	public static final String GAUGE_NAME_FW_VERSION = "ECU Software Version";
	public static final String GAUGE_NAME_GEAR_RATIO = "Gearbox Ratio";
	public static final String GAUGE_NAME_GYRO_YAW = "Gyro: Yaw rate";
	public static final String GAUGE_NAME_IAC = "Idle: Position";
	public static final String GAUGE_NAME_IAT = "IAT";
	public static final String GAUGE_NAME_IDLE_POSITION = "Idle: Position sensor";
	public static final String GAUGE_NAME_IGNITION_LOAD = "Ignition: load";
	public static final String GAUGE_NAME_IGNITION_MODE = "Ignition: Mode";
	public static final String GAUGE_NAME_INJECTION_MODE = "Fuel: Injection mode";
	public static final String GAUGE_NAME_INJECTOR_LAG = "Fuel: injector lag";
	public static final String GAUGE_NAME_ISS = "TCU: Input Shaft Speed";
	public static final String GAUGE_NAME_KNOCK_COUNTER = "Knock: Count";
	public static final String GAUGE_NAME_KNOCK_FREQ = "knock: frequency";
	public static final String GAUGE_NAME_KNOCK_LEVEL = "Knock: Current level";
	public static final String GAUGE_NAME_KNOCK_RETARD = "Knock: Retard";
	public static final String GAUGE_NAME_LAMBDA = "Lambda";
	public static final String GAUGE_NAME_LAMBDA2 = "Lambda 2";
	public static final String GAUGE_NAME_LAST_ERROR = "Warning: last";
	public static final String GAUGE_NAME_MAF = "MAF";
	public static final String GAUGE_NAME_MAP = "MAP";
	public static final String GAUGE_NAME_OIL_PRESSURE = "Oil Pressure";
	public static final String GAUGE_NAME_OIL_PRESSURE_UNITS = "kPa";
	public static final String GAUGE_NAME_RAW_FUEL_PRESSURE_HIGH = "Fuel pressure raw (high)";
	public static final String GAUGE_NAME_RAW_FUEL_PRESSURE_LOW = "Fuel pressure raw (low)";
	public static final String GAUGE_NAME_RPM = "RPM";
	public static final String GAUGE_NAME_TARGET_AFR = "Fuel: target AFR";
	public static final String GAUGE_NAME_TARGET_LAMBDA = "Fuel: target lambda";
	public static final String GAUGE_NAME_TC_RATIO = "TCU: Torque Converter Ratio";
	public static final String GAUGE_NAME_TCHARGE = "Air: SD tCharge";
	public static final String GAUGE_NAME_THROTTLE_PEDAL = "Throttle pedal position";
	public static final String GAUGE_NAME_TIME = "Time";
	public static final String GAUGE_NAME_TIMING_ADVANCE = "Timing: base ignition";
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
	public static final String GAUGE_NAME_WARNING_COUNTER = "Warning: counter";
	public static final String GAUGE_NAME_WARNING_LAST = "Warning: last";
	public static final String GAUGE_NAME_WG_POSITION = "Wastegate position sensor";
	public static final int IGN_BLEND_COUNT = 4;
	public static final String INDICATOR_NAME_AC_SWITCH = "AC switch";
	public static final String INDICATOR_NAME_BRAKE_DOWN = "Brake switch";
	public static final String INDICATOR_NAME_CLUTCH_DOWN = "Clutch: down";
	public static final String INDICATOR_NAME_CLUTCH_UP = "Clutch: up";
	public static final int LUA_ANALOG_INPUT_COUNT = 8;
	public static final int LUA_GAUGE_COUNT = 8;
	public static final int MAX_CYLINDER_COUNT = 12;
	public static final int PACK_ADD_TEMPERATURE = 40;
	public static final int PACK_MULT_AFR = 1000;
	public static final int PACK_MULT_AFR_CFG = 10;
	public static final int PACK_MULT_ANGLE = 50;
	public static final int PACK_MULT_EGTEMPERATURE = 4;
	public static final int PACK_MULT_FLEX = 2;
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
	public static final int PACK_MULT_VSS = 100;
	public static final int STFT_BANK_COUNT = 2;
	public static final int TCU_GEAR_COUNT = 10;
	public static final int TPS_2_BYTE_PACKING_MULT = 100;
	public static final int VE_BLEND_COUNT = 4;
	public static final Field M_KNOCKLEVEL = Field.create("M_KNOCKLEVEL", 0, FieldType.FLOAT).setBaseOffset(940);
	public static final Field M_KNOCKCYL1 = Field.create("M_KNOCKCYL1", 4, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL2 = Field.create("M_KNOCKCYL2", 5, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL3 = Field.create("M_KNOCKCYL3", 6, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL4 = Field.create("M_KNOCKCYL4", 7, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL5 = Field.create("M_KNOCKCYL5", 8, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL6 = Field.create("M_KNOCKCYL6", 9, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL7 = Field.create("M_KNOCKCYL7", 10, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL8 = Field.create("M_KNOCKCYL8", 11, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL9 = Field.create("M_KNOCKCYL9", 12, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL10 = Field.create("M_KNOCKCYL10", 13, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL11 = Field.create("M_KNOCKCYL11", 14, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKCYL12 = Field.create("M_KNOCKCYL12", 15, FieldType.INT8).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKRETARD = Field.create("M_KNOCKRETARD", 16, FieldType.FLOAT).setBaseOffset(940);
	public static final Field M_KNOCKTHRESHOLD = Field.create("M_KNOCKTHRESHOLD", 20, FieldType.FLOAT).setBaseOffset(940);
	public static final Field M_KNOCKCOUNT = Field.create("M_KNOCKCOUNT", 24, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_MAXIMUMRETARD = Field.create("M_MAXIMUMRETARD", 28, FieldType.FLOAT).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM1 = Field.create("M_KNOCKSPECTRUM1", 32, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM2 = Field.create("M_KNOCKSPECTRUM2", 36, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM3 = Field.create("M_KNOCKSPECTRUM3", 40, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM4 = Field.create("M_KNOCKSPECTRUM4", 44, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM5 = Field.create("M_KNOCKSPECTRUM5", 48, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM6 = Field.create("M_KNOCKSPECTRUM6", 52, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM7 = Field.create("M_KNOCKSPECTRUM7", 56, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM8 = Field.create("M_KNOCKSPECTRUM8", 60, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM9 = Field.create("M_KNOCKSPECTRUM9", 64, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM10 = Field.create("M_KNOCKSPECTRUM10", 68, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM11 = Field.create("M_KNOCKSPECTRUM11", 72, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM12 = Field.create("M_KNOCKSPECTRUM12", 76, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM13 = Field.create("M_KNOCKSPECTRUM13", 80, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM14 = Field.create("M_KNOCKSPECTRUM14", 84, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM15 = Field.create("M_KNOCKSPECTRUM15", 88, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUM16 = Field.create("M_KNOCKSPECTRUM16", 92, FieldType.INT).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKSPECTRUMCHANNELCYL = Field.create("M_KNOCKSPECTRUMCHANNELCYL", 96, FieldType.INT16).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKFREQUENCYSTART = Field.create("M_KNOCKFREQUENCYSTART", 98, FieldType.INT16).setScale(1.0).setBaseOffset(940);
	public static final Field M_KNOCKFREQUENCYSTEP = Field.create("M_KNOCKFREQUENCYSTEP", 100, FieldType.FLOAT).setBaseOffset(940);
	public static final Field M_KNOCKFUELTRIMMULTIPLIER = Field.create("M_KNOCKFUELTRIMMULTIPLIER", 104, FieldType.FLOAT).setBaseOffset(940);
	public static final Field[] VALUES = {
	M_KNOCKLEVEL,
	M_KNOCKCYL1,
	M_KNOCKCYL2,
	M_KNOCKCYL3,
	M_KNOCKCYL4,
	M_KNOCKCYL5,
	M_KNOCKCYL6,
	M_KNOCKCYL7,
	M_KNOCKCYL8,
	M_KNOCKCYL9,
	M_KNOCKCYL10,
	M_KNOCKCYL11,
	M_KNOCKCYL12,
	M_KNOCKRETARD,
	M_KNOCKTHRESHOLD,
	M_KNOCKCOUNT,
	M_MAXIMUMRETARD,
	M_KNOCKSPECTRUM1,
	M_KNOCKSPECTRUM2,
	M_KNOCKSPECTRUM3,
	M_KNOCKSPECTRUM4,
	M_KNOCKSPECTRUM5,
	M_KNOCKSPECTRUM6,
	M_KNOCKSPECTRUM7,
	M_KNOCKSPECTRUM8,
	M_KNOCKSPECTRUM9,
	M_KNOCKSPECTRUM10,
	M_KNOCKSPECTRUM11,
	M_KNOCKSPECTRUM12,
	M_KNOCKSPECTRUM13,
	M_KNOCKSPECTRUM14,
	M_KNOCKSPECTRUM15,
	M_KNOCKSPECTRUM16,
	M_KNOCKSPECTRUMCHANNELCYL,
	M_KNOCKFREQUENCYSTART,
	M_KNOCKFREQUENCYSTEP,
	M_KNOCKFUELTRIMMULTIPLIER,
	};
}
