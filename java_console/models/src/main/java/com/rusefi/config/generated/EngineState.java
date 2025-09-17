package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/engine_state.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class EngineState {
	public static final Field LUA_FUELADD = Field.create("LUA_FUELADD", 0, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field LUA_FUELMULT = Field.create("LUA_FUELMULT", 4, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field LUA_CLUTCHUPSTATE = Field.create("LUA_CLUTCHUPSTATE", 8, FieldType.BIT, 0).setBaseOffset(1204);
	public static final Field LUA_BRAKEPEDALSTATE = Field.create("LUA_BRAKEPEDALSTATE", 8, FieldType.BIT, 1).setBaseOffset(1204);
	public static final Field LUA_ACREQUESTSTATE = Field.create("LUA_ACREQUESTSTATE", 8, FieldType.BIT, 2).setBaseOffset(1204);
	public static final Field LUA_LUADISABLEETB = Field.create("LUA_LUADISABLEETB", 8, FieldType.BIT, 3).setBaseOffset(1204);
	public static final Field LUA_LUAIGNCUT = Field.create("LUA_LUAIGNCUT", 8, FieldType.BIT, 4).setBaseOffset(1204);
	public static final Field LUA_LUAFUELCUT = Field.create("LUA_LUAFUELCUT", 8, FieldType.BIT, 5).setBaseOffset(1204);
	public static final Field LUA_CLUTCHDOWNSTATE = Field.create("LUA_CLUTCHDOWNSTATE", 8, FieldType.BIT, 6).setBaseOffset(1204);
	public static final Field LUA_DISABLEDECELERATIONFUELCUTOFF = Field.create("LUA_DISABLEDECELERATIONFUELCUTOFF", 8, FieldType.BIT, 7).setBaseOffset(1204);
	public static final Field LUA_TORQUEREDUCTIONSTATE = Field.create("LUA_TORQUEREDUCTIONSTATE", 8, FieldType.BIT, 8).setBaseOffset(1204);
	public static final Field SD_TCHARGE = Field.create("SD_TCHARGE", 12, FieldType.INT16).setScale(0.01).setBaseOffset(1204);
	public static final Field SD_ALIGNMENTFILL_AT_2 = Field.create("SD_ALIGNMENTFILL_AT_2", 14, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field SD_TCHARGEK = Field.create("SD_TCHARGEK", 16, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field CRANKINGFUEL_COOLANTTEMPERATURECOEFFICIENT = Field.create("CRANKINGFUEL_COOLANTTEMPERATURECOEFFICIENT", 20, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field CRANKINGFUEL_TPSCOEFFICIENT = Field.create("CRANKINGFUEL_TPSCOEFFICIENT", 24, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field CRANKINGFUEL_DURATIONCOEFFICIENT = Field.create("CRANKINGFUEL_DURATIONCOEFFICIENT", 28, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field CRANKINGFUEL_FUEL = Field.create("CRANKINGFUEL_FUEL", 32, FieldType.INT16).setScale(0.01).setBaseOffset(1204);
	public static final Field CRANKINGFUEL_ALIGNMENTFILL_AT_14 = Field.create("CRANKINGFUEL_ALIGNMENTFILL_AT_14", 34, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field BAROCORRECTION = Field.create("BAROCORRECTION", 36, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field HELLENBOARDID = Field.create("HELLENBOARDID", 40, FieldType.INT16).setScale(1.0).setBaseOffset(1204);
	public static final Field CLUTCHUPSTATE = Field.create("CLUTCHUPSTATE", 42, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field BRAKEPEDALSTATE = Field.create("BRAKEPEDALSTATE", 43, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field STARTSTOPSTATE = Field.create("STARTSTOPSTATE", 44, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field SMARTCHIPSTATE = Field.create("SMARTCHIPSTATE", 45, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field SMARTCHIPRESTARTCOUNTER = Field.create("SMARTCHIPRESTARTCOUNTER", 46, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field SMARTCHIPALIVECOUNTER = Field.create("SMARTCHIPALIVECOUNTER", 47, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field STARTSTOPPHYSICALSTATE = Field.create("STARTSTOPPHYSICALSTATE", 48, FieldType.BIT, 0).setBaseOffset(1204);
	public static final Field ACRACTIVE = Field.create("ACRACTIVE", 48, FieldType.BIT, 1).setBaseOffset(1204);
	public static final Field ACRENGINEMOVEDRECENTLY = Field.create("ACRENGINEMOVEDRECENTLY", 48, FieldType.BIT, 2).setBaseOffset(1204);
	public static final Field HEATERCONTROLENABLED = Field.create("HEATERCONTROLENABLED", 48, FieldType.BIT, 3).setBaseOffset(1204);
	public static final Field LUADIGITALSTATE0 = Field.create("LUADIGITALSTATE0", 48, FieldType.BIT, 4).setBaseOffset(1204);
	public static final Field LUADIGITALSTATE1 = Field.create("LUADIGITALSTATE1", 48, FieldType.BIT, 5).setBaseOffset(1204);
	public static final Field LUADIGITALSTATE2 = Field.create("LUADIGITALSTATE2", 48, FieldType.BIT, 6).setBaseOffset(1204);
	public static final Field LUADIGITALSTATE3 = Field.create("LUADIGITALSTATE3", 48, FieldType.BIT, 7).setBaseOffset(1204);
	public static final Field CLUTCHDOWNSTATE = Field.create("CLUTCHDOWNSTATE", 48, FieldType.BIT, 8).setBaseOffset(1204);
	public static final Field STARTSTOPSTATETOGGLECOUNTER = Field.create("STARTSTOPSTATETOGGLECOUNTER", 52, FieldType.INT).setScale(1.0).setBaseOffset(1204);
	public static final Field LUASOFTSPARKSKIP = Field.create("LUASOFTSPARKSKIP", 56, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field LUAHARDSPARKSKIP = Field.create("LUAHARDSPARKSKIP", 60, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field TRACTIONCONTROLSPARKSKIP = Field.create("TRACTIONCONTROLSPARKSKIP", 64, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field FUELINJECTIONCOUNTER = Field.create("FUELINJECTIONCOUNTER", 68, FieldType.INT).setScale(1.0).setBaseOffset(1204);
	public static final Field GLOBALSPARKCOUNTER = Field.create("GLOBALSPARKCOUNTER", 72, FieldType.INT).setScale(1.0).setBaseOffset(1204);
	public static final Field FUELINGLOAD = Field.create("FUELINGLOAD", 76, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field IGNITIONLOAD = Field.create("IGNITIONLOAD", 80, FieldType.FLOAT).setBaseOffset(1204);
	public static final Field VETABLEYAXIS = Field.create("VETABLEYAXIS", 84, FieldType.INT16).setScale(0.01).setBaseOffset(1204);
	public static final Field OVERDWELLCOUNTER = Field.create("OVERDWELLCOUNTER", 86, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field OVERDWELLNOTSCHEDULEDCOUNTER = Field.create("OVERDWELLNOTSCHEDULEDCOUNTER", 87, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field SPARKOUTOFORDERCOUNTER = Field.create("SPARKOUTOFORDERCOUNTER", 88, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field ALIGNMENTFILL_AT_89 = Field.create("ALIGNMENTFILL_AT_89", 89, FieldType.INT8).setScale(1.0).setBaseOffset(1204);
	public static final Field[] VALUES = {
	LUA_FUELADD,
	LUA_FUELMULT,
	LUA_CLUTCHUPSTATE,
	LUA_BRAKEPEDALSTATE,
	LUA_ACREQUESTSTATE,
	LUA_LUADISABLEETB,
	LUA_LUAIGNCUT,
	LUA_LUAFUELCUT,
	LUA_CLUTCHDOWNSTATE,
	LUA_DISABLEDECELERATIONFUELCUTOFF,
	LUA_TORQUEREDUCTIONSTATE,
	SD_TCHARGE,
	SD_ALIGNMENTFILL_AT_2,
	SD_TCHARGEK,
	CRANKINGFUEL_COOLANTTEMPERATURECOEFFICIENT,
	CRANKINGFUEL_TPSCOEFFICIENT,
	CRANKINGFUEL_DURATIONCOEFFICIENT,
	CRANKINGFUEL_FUEL,
	CRANKINGFUEL_ALIGNMENTFILL_AT_14,
	BAROCORRECTION,
	HELLENBOARDID,
	CLUTCHUPSTATE,
	BRAKEPEDALSTATE,
	STARTSTOPSTATE,
	SMARTCHIPSTATE,
	SMARTCHIPRESTARTCOUNTER,
	SMARTCHIPALIVECOUNTER,
	STARTSTOPPHYSICALSTATE,
	ACRACTIVE,
	ACRENGINEMOVEDRECENTLY,
	HEATERCONTROLENABLED,
	LUADIGITALSTATE0,
	LUADIGITALSTATE1,
	LUADIGITALSTATE2,
	LUADIGITALSTATE3,
	CLUTCHDOWNSTATE,
	STARTSTOPSTATETOGGLECOUNTER,
	LUASOFTSPARKSKIP,
	LUAHARDSPARKSKIP,
	TRACTIONCONTROLSPARKSKIP,
	FUELINJECTIONCOUNTER,
	GLOBALSPARKCOUNTER,
	FUELINGLOAD,
	IGNITIONLOAD,
	VETABLEYAXIS,
	OVERDWELLCOUNTER,
	OVERDWELLNOTSCHEDULEDCOUNTER,
	SPARKOUTOFORDERCOUNTER,
	ALIGNMENTFILL_AT_89,
	};
}
