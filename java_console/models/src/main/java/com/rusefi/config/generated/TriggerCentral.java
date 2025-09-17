package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_central.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TriggerCentral {
	public static final Field HWEVENTCOUNTERS1 = Field.create("HWEVENTCOUNTERS1", 0, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field HWEVENTCOUNTERS2 = Field.create("HWEVENTCOUNTERS2", 2, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field HWEVENTCOUNTERS3 = Field.create("HWEVENTCOUNTERS3", 4, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field HWEVENTCOUNTERS4 = Field.create("HWEVENTCOUNTERS4", 6, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field VVTCAMCOUNTER = Field.create("VVTCAMCOUNTER", 8, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field ALIGNMENTFILL_AT_10 = Field.create("ALIGNMENTFILL_AT_10", 10, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field MAPVVT_MAP_AT_SPECIAL_POINT = Field.create("MAPVVT_MAP_AT_SPECIAL_POINT", 12, FieldType.FLOAT).setBaseOffset(1344);
	public static final Field MAPVVT_MAP_AT_DIFF = Field.create("MAPVVT_MAP_AT_DIFF", 16, FieldType.FLOAT).setBaseOffset(1344);
	public static final Field MAPVVT_MAP_AT_CYCLE_COUNT = Field.create("MAPVVT_MAP_AT_CYCLE_COUNT", 20, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field MAPVVT_SYNC_COUNTER = Field.create("MAPVVT_SYNC_COUNTER", 21, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field ALIGNMENTFILL_AT_22 = Field.create("ALIGNMENTFILL_AT_22", 22, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field CURRENTENGINEDECODEDPHASE = Field.create("CURRENTENGINEDECODEDPHASE", 24, FieldType.FLOAT).setBaseOffset(1344);
	public static final Field TRIGGERTOOTHANGLEERROR = Field.create("TRIGGERTOOTHANGLEERROR", 28, FieldType.FLOAT).setBaseOffset(1344);
	public static final Field TRIGGERIGNOREDTOOTHCOUNT = Field.create("TRIGGERIGNOREDTOOTHCOUNT", 32, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field ALIGNMENTFILL_AT_33 = Field.create("ALIGNMENTFILL_AT_33", 33, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field MAPCAMPREVTOOTHANGLE = Field.create("MAPCAMPREVTOOTHANGLE", 36, FieldType.FLOAT).setBaseOffset(1344);
	public static final Field ISDECODINGMAPCAM = Field.create("ISDECODINGMAPCAM", 40, FieldType.BIT, 0).setBaseOffset(1344);
	public static final Field TRIGGERELAPSEDUS = Field.create("TRIGGERELAPSEDUS", 44, FieldType.INT).setScale(1.0).setBaseOffset(1344);
	public static final Field[] VALUES = {
	HWEVENTCOUNTERS1,
	HWEVENTCOUNTERS2,
	HWEVENTCOUNTERS3,
	HWEVENTCOUNTERS4,
	VVTCAMCOUNTER,
	ALIGNMENTFILL_AT_10,
	MAPVVT_MAP_AT_SPECIAL_POINT,
	MAPVVT_MAP_AT_DIFF,
	MAPVVT_MAP_AT_CYCLE_COUNT,
	MAPVVT_SYNC_COUNTER,
	ALIGNMENTFILL_AT_22,
	CURRENTENGINEDECODEDPHASE,
	TRIGGERTOOTHANGLEERROR,
	TRIGGERIGNOREDTOOTHCOUNT,
	ALIGNMENTFILL_AT_33,
	MAPCAMPREVTOOTHANGLE,
	ISDECODINGMAPCAM,
	TRIGGERELAPSEDUS,
	};
}
