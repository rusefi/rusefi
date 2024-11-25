package com.rusefi.config.generated;

// this file was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/trigger/trigger_central.txt

// by class com.rusefi.output.FileJavaFieldsConsumer
import com.rusefi.config.*;

public class TriggerCentral {
	public static final int HW_EVENT_TYPES = 6;
	public static final Field HWEVENTCOUNTERS1 = Field.create("HWEVENTCOUNTERS1", 0, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field HWEVENTCOUNTERS2 = Field.create("HWEVENTCOUNTERS2", 2, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field HWEVENTCOUNTERS3 = Field.create("HWEVENTCOUNTERS3", 4, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field HWEVENTCOUNTERS4 = Field.create("HWEVENTCOUNTERS4", 6, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field HWEVENTCOUNTERS5 = Field.create("HWEVENTCOUNTERS5", 8, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field HWEVENTCOUNTERS6 = Field.create("HWEVENTCOUNTERS6", 10, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field VVTCAMCOUNTER = Field.create("VVTCAMCOUNTER", 12, FieldType.INT16).setScale(1.0).setBaseOffset(1344);
	public static final Field ALIGNMENTFILL_AT_14 = Field.create("ALIGNMENTFILL_AT_14", 14, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field MAPVVT_MAP_AT_SPECIAL_POINT = Field.create("MAPVVT_MAP_AT_SPECIAL_POINT", 16, FieldType.FLOAT).setBaseOffset(1344);
	public static final Field MAPVVT_MAP_AT_DIFF = Field.create("MAPVVT_MAP_AT_DIFF", 20, FieldType.FLOAT).setBaseOffset(1344);
	public static final Field MAPVVT_MAP_AT_CYCLE_COUNT = Field.create("MAPVVT_MAP_AT_CYCLE_COUNT", 24, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field MAPVVT_SYNC_COUNTER = Field.create("MAPVVT_SYNC_COUNTER", 25, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field ALIGNMENTFILL_AT_26 = Field.create("ALIGNMENTFILL_AT_26", 26, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field CURRENTENGINEDECODEDPHASE = Field.create("CURRENTENGINEDECODEDPHASE", 28, FieldType.FLOAT).setBaseOffset(1344);
	public static final Field TRIGGERTOOTHANGLEERROR = Field.create("TRIGGERTOOTHANGLEERROR", 32, FieldType.FLOAT).setBaseOffset(1344);
	public static final Field TRIGGERIGNOREDTOOTHCOUNT = Field.create("TRIGGERIGNOREDTOOTHCOUNT", 36, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field ALIGNMENTFILL_AT_37 = Field.create("ALIGNMENTFILL_AT_37", 37, FieldType.INT8).setScale(1.0).setBaseOffset(1344);
	public static final Field MAPCAMPREVTOOTHANGLE = Field.create("MAPCAMPREVTOOTHANGLE", 40, FieldType.FLOAT).setBaseOffset(1344);
	public static final Field ISDECODINGMAPCAM = Field.create("ISDECODINGMAPCAM", 44, FieldType.BIT, 0).setBaseOffset(1344);
	public static final Field TRIGGERELAPSEDUS = Field.create("TRIGGERELAPSEDUS", 48, FieldType.INT).setScale(1.0).setBaseOffset(1344);
	public static final Field[] VALUES = {
	HWEVENTCOUNTERS1,
	HWEVENTCOUNTERS2,
	HWEVENTCOUNTERS3,
	HWEVENTCOUNTERS4,
	HWEVENTCOUNTERS5,
	HWEVENTCOUNTERS6,
	VVTCAMCOUNTER,
	ALIGNMENTFILL_AT_14,
	MAPVVT_MAP_AT_SPECIAL_POINT,
	MAPVVT_MAP_AT_DIFF,
	MAPVVT_MAP_AT_CYCLE_COUNT,
	MAPVVT_SYNC_COUNTER,
	ALIGNMENTFILL_AT_26,
	CURRENTENGINEDECODEDPHASE,
	TRIGGERTOOTHANGLEERROR,
	TRIGGERIGNOREDTOOTHCOUNT,
	ALIGNMENTFILL_AT_37,
	MAPCAMPREVTOOTHANGLE,
	ISDECODINGMAPCAM,
	TRIGGERELAPSEDUS,
	};
}
