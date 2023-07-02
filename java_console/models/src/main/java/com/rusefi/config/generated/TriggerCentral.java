package com.rusefi.config.generated;

import com.rusefi.config.*;

public class TriggerCentral {
	public static final int HW_EVENT_TYPES = 6;
	public static final Field HWEVENTCOUNTERS1 = Field.create("HWEVENTCOUNTERS1", 0, FieldType.INT).setScale(1.0).setBaseOffset(1144);
	public static final Field HWEVENTCOUNTERS2 = Field.create("HWEVENTCOUNTERS2", 4, FieldType.INT).setScale(1.0).setBaseOffset(1144);
	public static final Field HWEVENTCOUNTERS3 = Field.create("HWEVENTCOUNTERS3", 8, FieldType.INT).setScale(1.0).setBaseOffset(1144);
	public static final Field HWEVENTCOUNTERS4 = Field.create("HWEVENTCOUNTERS4", 12, FieldType.INT).setScale(1.0).setBaseOffset(1144);
	public static final Field HWEVENTCOUNTERS5 = Field.create("HWEVENTCOUNTERS5", 16, FieldType.INT).setScale(1.0).setBaseOffset(1144);
	public static final Field HWEVENTCOUNTERS6 = Field.create("HWEVENTCOUNTERS6", 20, FieldType.INT).setScale(1.0).setBaseOffset(1144);
	public static final Field VVTCAMCOUNTER = Field.create("VVTCAMCOUNTER", 24, FieldType.INT).setScale(1.0).setBaseOffset(1144);
	public static final Field MAPVVT_MAP_AT_SPECIAL_POINT = Field.create("MAPVVT_MAP_AT_SPECIAL_POINT", 28, FieldType.FLOAT).setBaseOffset(1144);
	public static final Field MAPVVT_MAP_AT_DIFF = Field.create("MAPVVT_MAP_AT_DIFF", 32, FieldType.FLOAT).setBaseOffset(1144);
	public static final Field MAPVVT_MAP_AT_CYCLE_COUNT = Field.create("MAPVVT_MAP_AT_CYCLE_COUNT", 36, FieldType.INT8).setScale(1.0).setBaseOffset(1144);
	public static final Field MAPVVT_MAP_PEAK = Field.create("MAPVVT_MAP_PEAK", 37, FieldType.INT8).setScale(1.0).setBaseOffset(1144);
	public static final Field ALIGNMENTFILL_AT_38 = Field.create("ALIGNMENTFILL_AT_38", 38, FieldType.INT8).setScale(1.0).setBaseOffset(1144);
	public static final Field CURRENTENGINEDECODEDPHASE = Field.create("CURRENTENGINEDECODEDPHASE", 40, FieldType.FLOAT).setBaseOffset(1144);
	public static final Field TRIGGERTOOTHANGLEERROR = Field.create("TRIGGERTOOTHANGLEERROR", 44, FieldType.FLOAT).setBaseOffset(1144);
	public static final Field TRIGGERIGNOREDTOOTHCOUNT = Field.create("TRIGGERIGNOREDTOOTHCOUNT", 48, FieldType.INT8).setScale(1.0).setBaseOffset(1144);
	public static final Field ALIGNMENTFILL_AT_49 = Field.create("ALIGNMENTFILL_AT_49", 49, FieldType.INT8).setScale(1.0).setBaseOffset(1144);
	public static final Field[] VALUES = {
	HWEVENTCOUNTERS1,
	HWEVENTCOUNTERS2,
	HWEVENTCOUNTERS3,
	HWEVENTCOUNTERS4,
	HWEVENTCOUNTERS5,
	HWEVENTCOUNTERS6,
	VVTCAMCOUNTER,
	MAPVVT_MAP_AT_SPECIAL_POINT,
	MAPVVT_MAP_AT_DIFF,
	MAPVVT_MAP_AT_CYCLE_COUNT,
	MAPVVT_MAP_PEAK,
	ALIGNMENTFILL_AT_38,
	CURRENTENGINEDECODEDPHASE,
	TRIGGERTOOTHANGLEERROR,
	TRIGGERIGNOREDTOOTHCOUNT,
	ALIGNMENTFILL_AT_49,
	};
}
