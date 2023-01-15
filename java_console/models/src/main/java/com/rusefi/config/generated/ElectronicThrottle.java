package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Sun Jan 15 04:27:57 UTC 2023

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class ElectronicThrottle {
	public static final Field IDLEPOSITION = Field.create("IDLEPOSITION", 0, FieldType.FLOAT).setBaseOffset(1240);
	public static final Field TRIM = Field.create("TRIM", 4, FieldType.FLOAT).setBaseOffset(1240);
	public static final Field LUAADJUSTMENT = Field.create("LUAADJUSTMENT", 8, FieldType.FLOAT).setBaseOffset(1240);
	public static final Field ETBFEEDFORWARD = Field.create("ETBFEEDFORWARD", 12, FieldType.FLOAT).setBaseOffset(1240);
	public static final Field ETBINTEGRALERROR = Field.create("ETBINTEGRALERROR", 16, FieldType.FLOAT).setBaseOffset(1240);
	public static final Field ETBCURRENTTARGET = Field.create("ETBCURRENTTARGET", 20, FieldType.FLOAT).setBaseOffset(1240);
	public static final Field ETBCURRENTADJUSTEDTARGET = Field.create("ETBCURRENTADJUSTEDTARGET", 24, FieldType.FLOAT).setBaseOffset(1240);
	public static final Field ETBREVLIMITACTIVE = Field.create("ETBREVLIMITACTIVE", 28, FieldType.BIT, 0).setBaseOffset(1240);
	public static final Field JAMDETECTED = Field.create("JAMDETECTED", 28, FieldType.BIT, 1).setBaseOffset(1240);
	public static final Field ETBDUTYRATEOFCHANGE = Field.create("ETBDUTYRATEOFCHANGE", 32, FieldType.FLOAT).setBaseOffset(1240);
	public static final Field ETBDUTYAVERAGE = Field.create("ETBDUTYAVERAGE", 36, FieldType.FLOAT).setBaseOffset(1240);
	public static final Field ETBTPSERRORCOUNTER = Field.create("ETBTPSERRORCOUNTER", 40, FieldType.INT16).setScale(1.0).setBaseOffset(1240);
	public static final Field ETBPPSERRORCOUNTER = Field.create("ETBPPSERRORCOUNTER", 42, FieldType.INT16).setScale(1.0).setBaseOffset(1240);
	public static final Field ETBERRORCODE = Field.create("ETBERRORCODE", 44, FieldType.INT8).setScale(1.0).setBaseOffset(1240);
	public static final Field ALIGNMENTFILL_AT_45 = Field.create("ALIGNMENTFILL_AT_45", 45, FieldType.INT8).setScale(1.0).setBaseOffset(1240);
	public static final Field JAMTIMER = Field.create("JAMTIMER", 46, FieldType.INT16).setScale(0.01).setBaseOffset(1240);
	public static final Field[] VALUES = {
	IDLEPOSITION,
	TRIM,
	LUAADJUSTMENT,
	ETBFEEDFORWARD,
	ETBINTEGRALERROR,
	ETBCURRENTTARGET,
	ETBCURRENTADJUSTEDTARGET,
	ETBREVLIMITACTIVE,
	JAMDETECTED,
	ETBDUTYRATEOFCHANGE,
	ETBDUTYAVERAGE,
	ETBTPSERRORCOUNTER,
	ETBPPSERRORCOUNTER,
	ETBERRORCODE,
	ALIGNMENTFILL_AT_45,
	JAMTIMER,
	};
}
