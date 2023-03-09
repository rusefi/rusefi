package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/engine_state.txt Thu Mar 09 18:39:44 UTC 2023

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class EngineState {
	public static final int BANKS_COUNT = 2;
	public static final int CAM_INPUTS_COUNT = 4;
	public static final int CAMS_PER_BANK = 2;
	public static final int EGT_CHANNEL_COUNT = 8;
	public static final String GAUGE_CATEGORY_BOOST_CONTROL = "Boost Control";
	public static final String GAUGE_CATEGORY_ETB = "ETB more";
	public static final String GAUGE_CATEGORY_FUEL_MATH = "Fuel: math";
	public static final String GAUGE_CATEGORY_TIMING = "Timing";
	public static final String GAUGE_COIL_DWELL_TIME = "dwell: coil charge time";
	public static final String GAUGE_NAME_ACCEL_ROLL = "Acceleration: Roll";
	public static final String GAUGE_NAME_ACCEL_X = "Acceleration: X";
	public static final String GAUGE_NAME_ACCEL_Y = "Acceleration: Y";
	public static final String GAUGE_NAME_ACCEL_YAW = "Acceleration: Yaw";
	public static final String GAUGE_NAME_ACCEL_Z = "Acceleration: Z";
	public static final String GAUGE_NAME_AFR = "Air/Fuel Ratio";
	public static final String GAUGE_NAME_AFR2 = "Air/Fuel Ratio 2";
	public static final String GAUGE_NAME_AIR_FLOW_ESTIMATE = "MAF estimate";
	public static final String GAUGE_NAME_AIR_FLOW_MEASURED = "MAF";
	public static final String GAUGE_NAME_AIR_FLOW_MEASURED_2 = "MAF #2";
	public static final String GAUGE_NAME_AIR_MASS = "air mass";
	public static final String GAUGE_NAME_AUX_LINEAR_1 = "Aux Linear #1";
	public static final String GAUGE_NAME_AUX_LINEAR_2 = "Aux Linear #2";
	public static final String GAUGE_NAME_AUX_TEMP1 = "Aux temp 1";
	public static final String GAUGE_NAME_AUX_TEMP2 = "Aux temp 2";
	public static final String GAUGE_NAME_BARO_PRESSURE = "Barometric pressure";
	public static final String GAUGE_NAME_BOOST_CLOSED_LOOP = "Boost Closed Loop";
	public static final String GAUGE_NAME_BOOST_OPEN_LOOP = "Boost Open Loop";
	public static final String GAUGE_NAME_BOOST_OUTPUT = "Boost Output";
	public static final String GAUGE_NAME_BOOST_TARGET = "Boost Target";
	public static final String GAUGE_NAME_CLT = "CLT";
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
	public static final String GAUGE_NAME_FUEL_BARO_CORR = "fuel: Barometric pressure mult";
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
	public static final String GAUGE_NAME_FUEL_TPS_EXTRA = "fuel: TPS acceleration add fuel ms";
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
	public static final int IGN_BLEND_COUNT = 4;
	public static final String INDICATOR_NAME_AC_SWITCH = "AC switch";
	public static final String INDICATOR_NAME_BRAKE_DOWN = "brake: down";
	public static final String INDICATOR_NAME_CLUTCH_DOWN = "clutch: down";
	public static final String INDICATOR_NAME_CLUTCH_UP = "clutch: up";
	public static final int LUA_ANALOG_INPUT_COUNT = 8;
	public static final int LUA_GAUGE_COUNT = 2;
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
	public static final int TPS_2_BYTE_PACKING_MULT = 100;
	public static final int VE_BLEND_COUNT = 4;
	public static final Field LUA_FUELADD = Field.create("LUA_FUELADD", 0, FieldType.FLOAT).setBaseOffset(980);
	public static final Field LUA_FUELMULT = Field.create("LUA_FUELMULT", 4, FieldType.FLOAT).setBaseOffset(980);
	public static final Field LUA_CLUTCHUPSTATE = Field.create("LUA_CLUTCHUPSTATE", 8, FieldType.BIT, 0).setBaseOffset(980);
	public static final Field LUA_BRAKEPEDALSTATE = Field.create("LUA_BRAKEPEDALSTATE", 8, FieldType.BIT, 1).setBaseOffset(980);
	public static final Field LUA_ACREQUESTSTATE = Field.create("LUA_ACREQUESTSTATE", 8, FieldType.BIT, 2).setBaseOffset(980);
	public static final Field LUA_LUADISABLEETB = Field.create("LUA_LUADISABLEETB", 8, FieldType.BIT, 3).setBaseOffset(980);
	public static final Field LUA_LUAIGNCUT = Field.create("LUA_LUAIGNCUT", 8, FieldType.BIT, 4).setBaseOffset(980);
	public static final Field SD_TCHARGE = Field.create("SD_TCHARGE", 12, FieldType.FLOAT).setBaseOffset(980);
	public static final Field SD_TCHARGEK = Field.create("SD_TCHARGEK", 16, FieldType.FLOAT).setBaseOffset(980);
	public static final Field SD_AIRFLOW = Field.create("SD_AIRFLOW", 20, FieldType.FLOAT).setBaseOffset(980);
	public static final Field ENGINECYCLEDURATIONMS = Field.create("ENGINECYCLEDURATIONMS", 24, FieldType.FLOAT).setBaseOffset(980);
	public static final Field MINRPMKCURRENTTPS = Field.create("MINRPMKCURRENTTPS", 28, FieldType.FLOAT).setBaseOffset(980);
	public static final Field CURRENTTPSADC = Field.create("CURRENTTPSADC", 32, FieldType.INT).setScale(1.0).setBaseOffset(980);
	public static final Field TPSVOLTAGEMCU = Field.create("TPSVOLTAGEMCU", 36, FieldType.FLOAT).setBaseOffset(980);
	public static final Field TPSVOLTAGEBOARD = Field.create("TPSVOLTAGEBOARD", 40, FieldType.FLOAT).setBaseOffset(980);
	public static final Field CURRENTBAROCORRECTEDVE = Field.create("CURRENTBAROCORRECTEDVE", 44, FieldType.FLOAT).setBaseOffset(980);
	public static final Field INJECTORFLOWCORRECTION = Field.create("INJECTORFLOWCORRECTION", 48, FieldType.FLOAT).setBaseOffset(980);
	public static final Field BAROCORRECTION = Field.create("BAROCORRECTION", 52, FieldType.FLOAT).setBaseOffset(980);
	public static final Field CRANKINGFUEL_BASEFUEL = Field.create("CRANKINGFUEL_BASEFUEL", 56, FieldType.FLOAT).setBaseOffset(980);
	public static final Field CRANKINGFUEL_COOLANTTEMPERATURECOEFFICIENT = Field.create("CRANKINGFUEL_COOLANTTEMPERATURECOEFFICIENT", 60, FieldType.FLOAT).setBaseOffset(980);
	public static final Field CRANKINGFUEL_TPSCOEFFICIENT = Field.create("CRANKINGFUEL_TPSCOEFFICIENT", 64, FieldType.FLOAT).setBaseOffset(980);
	public static final Field CRANKINGFUEL_DURATIONCOEFFICIENT = Field.create("CRANKINGFUEL_DURATIONCOEFFICIENT", 68, FieldType.FLOAT).setBaseOffset(980);
	public static final Field CRANKINGFUEL_FUEL = Field.create("CRANKINGFUEL_FUEL", 72, FieldType.FLOAT).setBaseOffset(980);
	public static final Field HELLENBOARDID = Field.create("HELLENBOARDID", 76, FieldType.INT16).setScale(1.0).setBaseOffset(980);
	public static final Field ALIGNMENTFILL_AT_78 = Field.create("ALIGNMENTFILL_AT_78", 78, FieldType.INT8).setScale(1.0).setBaseOffset(980);
	public static final Field CLUTCHUPSTATE = Field.create("CLUTCHUPSTATE", 80, FieldType.BIT, 0).setBaseOffset(980);
	public static final Field CLUTCHDOWNSTATE = Field.create("CLUTCHDOWNSTATE", 80, FieldType.BIT, 1).setBaseOffset(980);
	public static final Field BRAKEPEDALSTATE = Field.create("BRAKEPEDALSTATE", 80, FieldType.BIT, 2).setBaseOffset(980);
	public static final Field STARTSTOPSTATE = Field.create("STARTSTOPSTATE", 80, FieldType.BIT, 3).setBaseOffset(980);
	public static final Field STARTSTOPSTATETOGGLECOUNTER = Field.create("STARTSTOPSTATETOGGLECOUNTER", 84, FieldType.INT).setScale(1.0).setBaseOffset(980);
	public static final Field EGTVALUE1 = Field.create("EGTVALUE1", 88, FieldType.FLOAT).setBaseOffset(980);
	public static final Field EGTVALUE2 = Field.create("EGTVALUE2", 92, FieldType.FLOAT).setBaseOffset(980);
	public static final Field DESIREDRPMLIMIT = Field.create("DESIREDRPMLIMIT", 96, FieldType.INT16).setScale(1.0).setBaseOffset(980);
	public static final Field ALIGNMENTFILL_AT_98 = Field.create("ALIGNMENTFILL_AT_98", 98, FieldType.INT8).setScale(1.0).setBaseOffset(980);
	public static final Field FUELINJECTIONCOUNTER = Field.create("FUELINJECTIONCOUNTER", 100, FieldType.INT).setScale(1.0).setBaseOffset(980);
	public static final Field SPARKCOUNTER = Field.create("SPARKCOUNTER", 104, FieldType.INT).setScale(1.0).setBaseOffset(980);
	public static final Field FUELINGLOAD = Field.create("FUELINGLOAD", 108, FieldType.FLOAT).setBaseOffset(980);
	public static final Field IGNITIONLOAD = Field.create("IGNITIONLOAD", 112, FieldType.FLOAT).setBaseOffset(980);
	public static final Field VETABLEYAXIS = Field.create("VETABLEYAXIS", 116, FieldType.INT16).setScale(0.01).setBaseOffset(980);
	public static final Field ALIGNMENTFILL_AT_118 = Field.create("ALIGNMENTFILL_AT_118", 118, FieldType.INT8).setScale(1.0).setBaseOffset(980);
	public static final Field[] VALUES = {
	LUA_FUELADD,
	LUA_FUELMULT,
	LUA_CLUTCHUPSTATE,
	LUA_BRAKEPEDALSTATE,
	LUA_ACREQUESTSTATE,
	LUA_LUADISABLEETB,
	LUA_LUAIGNCUT,
	SD_TCHARGE,
	SD_TCHARGEK,
	SD_AIRFLOW,
	ENGINECYCLEDURATIONMS,
	MINRPMKCURRENTTPS,
	CURRENTTPSADC,
	TPSVOLTAGEMCU,
	TPSVOLTAGEBOARD,
	CURRENTBAROCORRECTEDVE,
	INJECTORFLOWCORRECTION,
	BAROCORRECTION,
	CRANKINGFUEL_BASEFUEL,
	CRANKINGFUEL_COOLANTTEMPERATURECOEFFICIENT,
	CRANKINGFUEL_TPSCOEFFICIENT,
	CRANKINGFUEL_DURATIONCOEFFICIENT,
	CRANKINGFUEL_FUEL,
	HELLENBOARDID,
	ALIGNMENTFILL_AT_78,
	CLUTCHUPSTATE,
	CLUTCHDOWNSTATE,
	BRAKEPEDALSTATE,
	STARTSTOPSTATE,
	STARTSTOPSTATETOGGLECOUNTER,
	EGTVALUE1,
	EGTVALUE2,
	DESIREDRPMLIMIT,
	ALIGNMENTFILL_AT_98,
	FUELINJECTIONCOUNTER,
	SPARKCOUNTER,
	FUELINGLOAD,
	IGNITIONLOAD,
	VETABLEYAXIS,
	ALIGNMENTFILL_AT_118,
	};
}
